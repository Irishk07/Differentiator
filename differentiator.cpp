#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "differentiator.h"

#include "common.h"
#include "colors.h"
#include "debug.h"
#include "onegin.h"
#include "tree.h"


Tree_status DifferentiatorCtor(Differentiator* differentiator, const char* html_dump_filename, const char* tex_dump_filename, const char *directory) {
    assert(differentiator);
    assert(html_dump_filename);
    assert(directory);

    differentiator->cnt_trees = 0;

    differentiator->begin_buffer = NULL;
    differentiator->end_buffer   = NULL;
    differentiator->size_buffer  = 0;

    differentiator->dump_info.html_dump_filename = html_dump_filename;
    differentiator->dump_info.tex_dump_filename  = tex_dump_filename;
    differentiator->dump_info.directory          = directory;

    TREE_CHECK_AND_RETURN_ERRORS(ArrayCtorvariables(&differentiator->array_with_variables, DEFAULT_START_CAPACITY));
    TREE_CHECK_AND_RETURN_ERRORS(ArrayCtortrees(&differentiator->array_with_trees, DEFAULT_START_CAPACITY));

    return SUCCESS;
}


Tree_status UploadTree(Differentiator* differentiator) {
    color_printf(COLOR_PURPLE, " - Please, enter the name of the file from which the tree will be uploaded\n");

    char* file_name = ReadAnswer();

    if (file_name == NULL)
        color_printf(COLOR_RED, " - Sorry, I couldn't do it, I have some problems. Try later please\n");
    else
        TREE_CHECK_AND_RETURN_ERRORS(ReadTree(differentiator, file_name));

    color_printf(COLOR_GREEN, " - All right, tree is uploaded from your file %s\n", file_name);

    TREE_CHECK_AND_RETURN_ERRORS(TreeHTMLDump(differentiator, PointerOnTree(differentiator)->root, DUMP_INFO, NOT_ERROR_DUMP));

    free(file_name);

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator));

    return SUCCESS;
}

Tree_status ReadTree(Differentiator* differentiator, const char* file_with_tree) {
    assert(file_with_tree);

    TREE_CHECK_AND_RETURN_ERRORS(ReadOnegin(differentiator, file_with_tree));

    differentiator->begin_buffer = differentiator->buffer_with_tree;
    differentiator->end_buffer   = differentiator->begin_buffer + differentiator->size_buffer;

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    ArrayPushtrees(&differentiator->array_with_trees, tree);

    TREE_CHECK_AND_RETURN_ERRORS(ReadNode(differentiator, &tree->root, &differentiator->buffer_with_tree));

    TREE_CHECK_AND_RETURN_ERRORS(TreeHTMLDump(differentiator, tree->root, DUMP_INFO, NOT_ERROR_DUMP));

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator));

    return SUCCESS;
}

#define DUMP_CURRENT_SITUATION                  \
    TreeHTMLDump(differentiator, PointerOnTree(differentiator)->root, DUMP_INFO, NOT_ERROR_DUMP);                \
    dump_file = fopen(differentiator->dump_info.html_dump_filename, "a");                \
    if (dump_file == NULL)                                                    \
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);                             \
    fprintf(dump_file, "Current situation: \":%s\"\n", *current_pos);         \
    if (fclose(dump_file) == EOF)                                             \
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

Tree_status ReadNode(Differentiator* differentiator, Tree_node** tree_node, char** current_pos) {    
    assert(differentiator);
    assert(tree_node);
    assert(current_pos);
    assert(*current_pos);

    ON_DEBUG(FILE* dump_file = NULL);

    SkipSpaces(current_pos);

    if (*current_pos > differentiator->end_buffer)
        TREE_CHECK_AND_RETURN_ERRORS(BUFFER_OVERFLOW);

    if (**current_pos == '(') {
        (*current_pos)++;
        ON_DEBUG(DUMP_CURRENT_SITUATION);

        if (strncmp(*current_pos, "nil", LEN_NIL) == 0) {
            *current_pos += LEN_NIL;
            *tree_node = NULL;
        }

        else {
            int read_bytes = 0;
            sscanf(*current_pos, "\"%*[^\"]\"%n", &read_bytes);
            *(*current_pos + read_bytes - 1) = '\0'; // close_" -> '\0'

            type_t value = {.number = 0};

            Type_node type = FindOutType(differentiator, *current_pos + 1, &value); // +1 because skip open_"

            *tree_node = NodeCtor(PointerOnTree(differentiator), type, value, NULL, NULL);
            ON_DEBUG(DUMP_CURRENT_SITUATION);

            *current_pos += read_bytes;
            ON_DEBUG(DUMP_CURRENT_SITUATION);

            TREE_CHECK_AND_RETURN_ERRORS(ReadNode(differentiator, &(*tree_node)->left_node, current_pos));

            TREE_CHECK_AND_RETURN_ERRORS(ReadNode(differentiator, &(*tree_node)->right_node, current_pos));

            (*current_pos)++; // ++ because skip ')'
            ON_DEBUG(DUMP_CURRENT_SITUATION);

            SkipSpaces(current_pos);
        }
    }

    else if (strncmp(*current_pos, "nil", LEN_NIL) == 0) {
        *current_pos += LEN_NIL;
        ON_DEBUG(DUMP_CURRENT_SITUATION);

        *tree_node = NULL;
    }

    else {
        TREE_CHECK_AND_RETURN_ERRORS(SYNTAX_ERROR);
    }

    return SUCCESS;
}

#undef DUMP_CURRENT_SITUATION

Type_node FindOutType(Differentiator* differentiator, char* buffer, type_t* value) {
    assert(buffer);

    if (ItIsOperator(buffer, value) == FIND_YES)
        return OPERATOR;

    if (ItIsNumber(buffer, value) == FIND_YES)
        return NUMBER;

    if (ItIsVariable(differentiator, buffer, value) == FIND_YES)
        return VARIABLE;

    return WRONG_TYPE;
}

Status_of_finding ItIsOperator(char* buffer, type_t* value) {
    assert(buffer);

    if (strcmp(buffer, "+") == 0)
        value->operators = OPERATOR_ADD;

    else if (strcmp(buffer, "-") == 0)
        value->operators = OPERATOR_SUB;

    else if (strcmp(buffer, "*") == 0)
        value->operators = OPERATOR_MUL;

    else if (strcmp(buffer, "/") == 0)
        value->operators = OPERATOR_DIV;

    else if (strcmp(buffer, "^") == 0)
        value->operators = OPERATOR_POW;

    else if (strcmp(buffer, "ln") == 0)
        value->operators = OPERATOR_LN;

    else if (strcmp(buffer, "log") == 0)
        value->operators = OPERATOR_LOG;

    else if (strcmp(buffer, "sin") == 0)
        value->operators = OPERATOR_SIN;

    else if (strcmp(buffer, "cos") == 0)
        value->operators = OPERATOR_COS;

    else if (strcmp(buffer, "tg") == 0)
        value->operators = OPERATOR_TG;

    else if (strcmp(buffer, "ctg") == 0)
        value->operators = OPERATOR_CTG;

    else if (strcmp(buffer, "arcsin") == 0)
        value->operators = OPERATOR_ARCSIN;

    else if (strcmp(buffer, "arccos") == 0)
        value->operators = OPERATOR_ARCCOS;

    else if (strcmp(buffer, "arctg") == 0)
        value->operators = OPERATOR_ARCTG;
    
    else if (strcmp(buffer, "arcctg") == 0)
        value->operators = OPERATOR_ARCCTG;

    else
        return FIND_NO;

    return FIND_YES;
}



Status_of_finding ItIsNumber(char* buffer, type_t* value) {
    assert(buffer);

    char* endptr = NULL;
    double result = strtod(buffer, &endptr);

    if (endptr == buffer || *endptr != '\0')
        return FIND_NO;

    value->number = result;

    return FIND_YES;
}

Status_of_finding ItIsVariable(Differentiator* differentiator, char* buffer, type_t* value) {
    assert(buffer);

    char* temp_buffer = buffer;
    if (isdigit(*temp_buffer))
        return FIND_NO;

    while (*temp_buffer != '\0') {
        if (!isalpha(*temp_buffer) && !isdigit(*temp_buffer))
            return FIND_NO;

        (temp_buffer)++;
    }

    value->index_variable = (int)differentiator->array_with_variables.size;
    
    About_variable* about_variable = (About_variable*)calloc(1, sizeof(About_variable));
    *about_variable = {.name = strdup(buffer), .value = DEFAULT_VALUE};

    if (ArrayPushvariables(&differentiator->array_with_variables, about_variable) != SUCCESS)
        return FIND_NO;
    
    return FIND_YES;
}

void SkipSpaces(char** buffer) {
    assert(buffer);
    assert(*buffer);

    while (isspace(**buffer) && (**buffer) != '\0')
        (*buffer)++;
}

Tree_status CalculateValueOfExample(Differentiator* differentiator) {
    assert(differentiator);

    Tree* tree = PointerOnTree(differentiator);

    for (size_t i = 0; i < differentiator->array_with_variables.size; ++i)
        TREE_CHECK_AND_RETURN_ERRORS(FillValueOfVariables(differentiator, i));

    TreeHTMLDump(differentiator, tree->root, DUMP_INFO, NOT_ERROR_DUMP);

    double result = Calculating(differentiator, tree->root);

    color_printf(COLOR_GREEN, " - Answer is: %lg\n", result);

    return SUCCESS;
}

Tree_status FillValueOfVariables(Differentiator* differentiator, size_t index_of_variable) {
    assert(differentiator);

    int cnt_attempts = CNT_ATTEMPTS;
    int value = DEFAULT_VALUE;

    while (cnt_attempts-- > 0) {
        color_printf(COLOR_PURPLE, " - Enter the value of %s\n", (differentiator->array_with_variables.data)[index_of_variable]->name);

        if (scanf("%d%*c", &value) != 1) {
            scanf("%*[^\n]%*c");
            continue;
        }

        (differentiator->array_with_variables.data)[index_of_variable]->value = value;

        break;
    }

    if (cnt_attempts == CNT_ATTEMPTS) {
        color_printf(COLOR_RED, " - You try to do it %d times... What's wrong with you?\n", CNT_ATTEMPTS);
        TREE_CHECK_AND_RETURN_ERRORS(SYNTAX_ERROR);
    }

    return SUCCESS;
}

#define CALCULATING_OPERATORS(sign)                                                                                      \
        return Calculating(differentiator, tree_node->left_node) sign Calculating(differentiator, tree_node->right_node);

double Calculating(Differentiator* differentiator, Tree_node* tree_node) {
    if (tree_node == NULL)
        return 0.0;

    switch (tree_node->type) {
        case NUMBER:
            return tree_node->value.number; break;
        case VARIABLE:
            return ValueOfVariable(differentiator, tree_node); break;
        case OPERATOR:
            switch (tree_node->value.operators) {
                case OPERATOR_ADD:
                    CALCULATING_OPERATORS(ADD);
                case OPERATOR_SUB:
                    CALCULATING_OPERATORS(SUB);  
                case OPERATOR_MUL:
                    CALCULATING_OPERATORS(MUL);
                case OPERATOR_DIV:
                    CALCULATING_OPERATORS(DIV);
                case OPERATOR_POW:
                    return pow(Calculating(differentiator, tree_node->left_node), Calculating(differentiator, tree_node->right_node));
                case WRONG_OPERATOR:
                default: break;
            }
        case WRONG_TYPE: break;
        default: break;
    }

    return 0.0;
}


Tree_status TreeTexDump(Differentiator* differentiator) {
    FILE* tex_dump_file = NULL;
    tex_dump_file = fopen(differentiator->dump_info.tex_dump_filename, "w");
    if (tex_dump_file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    fprintf(tex_dump_file, "\\documentclass[12pt]{article}\n");
    fprintf(tex_dump_file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(tex_dump_file, "\\usepackage{amsmath}\n");
    fprintf(tex_dump_file, "\\begin{document}\n");
    fprintf(tex_dump_file, "\\[\n");

    TreeHTMLDump(differentiator, PointerOnTree(differentiator)->root, DUMP_INFO, NOT_ERROR_DUMP);

    PrintExpressionToTex(differentiator, PointerOnTree(differentiator)->root, tex_dump_file, NO_PRIORITET);

    fprintf(tex_dump_file, "\n\\]\n");
    fprintf(tex_dump_file, "\\end{document}\n");

    if (fclose(tex_dump_file) == EOF)                                             
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    char command[MAX_LEN_NAME] = {};
    snprintf(command, MAX_LEN_NAME, "pdflatex %s", differentiator->dump_info.tex_dump_filename);
    if (system(command) != 0)
        TREE_CHECK_AND_RETURN_ERRORS(EXECUTION_FAILED);

    return SUCCESS;
}

#define PRINT_OPERATOR_TO_TEX(sign)                                                                 \
{                                                                                                   \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet);   \
    fprintf(tex_dump_file, sign);                                                                   \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet);  \
    break;                                                                                          \
}                                                                                                   

#define PRINT_DIV_OPERATOR_TO_TEX                                                                   \
{                                                                                                   \
    fprintf(tex_dump_file, "\\frac{");                                                              \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet);   \
    fprintf(tex_dump_file, "}{");                                                                   \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet);  \
    fprintf(tex_dump_file, "}");                                                                    \
    break;                                                                                          \
}                                                                                       

#define PRINT_POW_OPERATOR_TO_TEX                                                                   \
{                                                                                                   \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet);   \
    fprintf(tex_dump_file, "^{");                                                                   \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet);  \
    fprintf(tex_dump_file, "}");                                                                    \
    break;                                                                                          \
}         

#define PRINT_LN_OPERATOR_TO_TEX                                                            \
{                                                                                           \
    fprintf(tex_dump_file, "\\ln\\left(");                                                  \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet);\
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}    

#define PRINTF_LOG_OPERATOR_TO_TEX                                                          \
{                                                                                           \
    fprintf(tex_dump_file, "\\log\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet);\
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_SIN_OPERATOR_TO_TEX                                                          \
{                                                                                           \
    fprintf(tex_dump_file, "\\sin\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet);\
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}                                                                                           \

#define PRINTF_COS_OPERATOR_TO_TEX                                                          \
{                                                                                           \
    fprintf(tex_dump_file, "\\cos\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_TG_OPERATOR_TO_TEX                                                           \
{                                                                                           \
    fprintf(tex_dump_file, "\\tan\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_CTG_OPERATOR_TO_TEX                                                          \
{                                                                                           \
    fprintf(tex_dump_file, "\\cot\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_ARCSIN_OPERATOR_TO_TEX                                                       \
{                                                                                           \
    fprintf(tex_dump_file, "\\arcsin\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_ARCCOS_OPERATOR_TO_TEX                                                       \
{                                                                                           \
    fprintf(tex_dump_file, "\\arccos\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_ARCTG_OPERATOR_TO_TEX                                                        \
{                                                                                           \
    fprintf(tex_dump_file, "\\arctan\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}

#define PRINTF_ARCCTG_OPERATOR_TO_TEX                                                       \
{                                                                                           \
    fprintf(tex_dump_file, "\\arccot\\left(");                                                 \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet); \
    fprintf(tex_dump_file, "\\right)");                                                     \
    break;                                                                                  \
}



void PrintExpressionToTex(Differentiator* differentiator, Tree_node* tree_node, FILE* tex_dump_file, Prioritets parent_prioritet) {
    Prioritets current_prioritet = GetPrioritet(tree_node);

    if (current_prioritet < parent_prioritet && current_prioritet != NO_PRIORITET)
        fprintf(tex_dump_file, "(");

    switch (tree_node->type) {
        case NUMBER:
            if (tree_node->value.number < 0)
                fprintf(tex_dump_file, "(%lg)", tree_node->value.number);
            else
                fprintf(tex_dump_file, "%lg", tree_node->value.number);
            break; 
        case VARIABLE:
            fprintf(tex_dump_file, "%s", NameOfVariable(differentiator, tree_node));
            break;
        case OPERATOR:
            switch(tree_node->value.operators) {
                case OPERATOR_ADD:
                    PRINT_OPERATOR_TO_TEX(" + ");
                case OPERATOR_SUB:
                    PRINT_OPERATOR_TO_TEX(" - ");
                case OPERATOR_MUL:
                    PRINT_OPERATOR_TO_TEX(" \\cdot ");
                case OPERATOR_DIV:
                    PRINT_DIV_OPERATOR_TO_TEX;
                case OPERATOR_POW:
                    PRINT_POW_OPERATOR_TO_TEX;
                case OPERATOR_LN:
                    PRINT_LN_OPERATOR_TO_TEX;
                case OPERATOR_LOG:
                    PRINTF_LOG_OPERATOR_TO_TEX;
                case OPERATOR_SIN:
                    PRINTF_SIN_OPERATOR_TO_TEX;
                case OPERATOR_COS:
                    PRINTF_COS_OPERATOR_TO_TEX;
                case OPERATOR_TG:
                    PRINTF_TG_OPERATOR_TO_TEX;
                case OPERATOR_CTG:
                    PRINTF_CTG_OPERATOR_TO_TEX;
                case OPERATOR_ARCSIN:
                    PRINTF_ARCSIN_OPERATOR_TO_TEX;
                case OPERATOR_ARCCOS:
                    PRINTF_ARCCOS_OPERATOR_TO_TEX;
                case OPERATOR_ARCTG:
                    PRINTF_ARCTG_OPERATOR_TO_TEX;
                case OPERATOR_ARCCTG:
                    PRINTF_ARCCTG_OPERATOR_TO_TEX;
                case WRONG_OPERATOR:
                default: break;
            }
        case WRONG_TYPE:
        default:
            break;
    }

    if (current_prioritet < parent_prioritet && current_prioritet != NO_PRIORITET)
        fprintf(tex_dump_file, ")");
}

Prioritets GetPrioritet(Tree_node* tree_node) {
    switch(tree_node->type) {
        case OPERATOR:
            switch (tree_node->value.operators) {
                case OPERATOR_ADD:
                case OPERATOR_SUB:
                    return LOW_PRIOTITET;
                case OPERATOR_MUL:
                case OPERATOR_DIV:
                    return MIDDLE_PRIORITET;
                case OPERATOR_POW:
                    return HIGH_PRIORITET;
                case OPERATOR_LN: 
                case OPERATOR_LOG:
                case OPERATOR_SIN:
                case OPERATOR_COS:
                case OPERATOR_TG: 
                case OPERATOR_CTG:
                case OPERATOR_ARCSIN: 
                case OPERATOR_ARCCOS: 
                case OPERATOR_ARCTG: 
                case OPERATOR_ARCCTG: 
                    return HIGHEST_PRIORITET; 
                case WRONG_OPERATOR:
                default: break;
            }
            break;
        case NUMBER:
        case VARIABLE:
        case WRONG_TYPE:
        default:
            return NO_PRIORITET;
    }

    return NO_PRIORITET;
}


Tree_status DifferentiatorDtor(Differentiator* differentiator) {
    Tree_status status = TreeVerify(differentiator);

    free(differentiator->begin_buffer);
    
    for (size_t i = 0; i < differentiator->array_with_trees.size; ++i) {
        DifferentiatorNodeDtor(differentiator, (differentiator->array_with_trees.data[i])->root);
    }
    ArrayDtorTrees(&differentiator->array_with_trees);
    free(differentiator->array_with_trees.data);

    ArrayDtorVariables(&differentiator->array_with_variables);
    free(differentiator->array_with_variables.data);

    return status;
}

void DifferentiatorNodeDtor(Differentiator* differentiator, Tree_node* tree_node) {
    if (tree_node == NULL)
        return;

    if (tree_node->need_dtor_childs == 1) {
        DifferentiatorNodeDtor(differentiator, tree_node->left_node);

        DifferentiatorNodeDtor(differentiator, tree_node->right_node);
    }

    free(tree_node);
}