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

    differentiator->tree = {};
    TREE_CHECK_AND_RETURN_ERRORS(TreeCtor(&differentiator->tree, html_dump_filename, directory));
    // TREE_CHECK_AND_RETURN_ERRORS(CreateRoot(&akinator->tree, "Bla-bla-bla"));

    differentiator->begin_buffer = NULL;
    differentiator->end_buffer   = NULL;
    differentiator->size_buffer  = 0;

    differentiator->dump_info.html_dump_filename = html_dump_filename;
    differentiator->dump_info.tex_dump_filename  = tex_dump_filename;
    differentiator->dump_info.directory          = directory;

    TREE_CHECK_AND_RETURN_ERRORS(ArrayCtorvariables(&differentiator->array_with_variables, DEFAULT_START_CAPACITY));

    return SUCCESS;
}


Tree_status UploadTree(Differentiator* differentiator) {
    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator, &differentiator->tree)); 

    color_printf(COLOR_PURPLE, " - Please, enter the name of the file from which the tree will be uploaded\n");

    char* file_name = ReadAnswer();

    if (file_name == NULL)
        color_printf(COLOR_RED, " - Sorry, I couldn't do it, I have some problems. Try later please\n");
    else
        TREE_CHECK_AND_RETURN_ERRORS(ReadTree(differentiator, file_name));

    color_printf(COLOR_GREEN, " - All right, tree is uploaded from your file %s\n", file_name);

    TREE_CHECK_AND_RETURN_ERRORS(TreeHTMLDump(differentiator, &differentiator->tree, differentiator->tree.root, DUMP_INFO, NOT_ERROR_DUMP));

    free(file_name);

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator, &differentiator->tree));

    return SUCCESS;
}

Tree_status ReadTree(Differentiator* differentiator, const char* file_with_tree) {
    assert(file_with_tree);
    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator, &differentiator->tree));

    TREE_CHECK_AND_RETURN_ERRORS(ReadOnegin(differentiator, file_with_tree));

    differentiator->begin_buffer = differentiator->buffer_with_tree;
    differentiator->end_buffer   = differentiator->begin_buffer + differentiator->size_buffer;

    differentiator->tree.root = NULL;
    TREE_CHECK_AND_RETURN_ERRORS(ReadNode(differentiator, &differentiator->tree.root, &differentiator->buffer_with_tree, NULL));

    TREE_CHECK_AND_RETURN_ERRORS(TreeHTMLDump(differentiator, &differentiator->tree, differentiator->tree.root, DUMP_INFO, NOT_ERROR_DUMP));

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator, &differentiator->tree));

    return SUCCESS;
}

#define DUMP_CURRENT_SITUATION(tree, tree_node, current_pos)                  \
    TreeHTMLDump(&tree, tree_node, DUMP_INFO, NOT_ERROR_DUMP);                \
    dump_file = fopen(tree.dump_info.html_dump_filename, "a");                \
    if (dump_file == NULL)                                                    \
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);                             \
    fprintf(dump_file, "Current situation: \":%s\"\n", *current_pos);         \
    if (fclose(dump_file) == EOF)                                             \
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

Tree_status ReadNode(Differentiator* differentiator, Tree_node** tree_node, char** current_pos, Tree_node* parent) {    
    assert(differentiator);
    assert(tree_node);
    assert(current_pos);
    assert(*current_pos);

    ON_DEBUG(FILE* dump_file = NULL);

    SkipSpaces(current_pos);

    if (*current_pos > differentiator->end_buffer)
        TREE_CHECK_AND_RETURN_ERRORS(BUFFER_OVERFLOW);

    if (**current_pos == '(') {
        NodeCtor(&differentiator->tree, tree_node);
        ON_DEBUG(DUMP_CURRENT_SITUATION(differentiator->tree, *tree_node, current_pos));

        (*current_pos)++;
        ON_DEBUG(DUMP_CURRENT_SITUATION(differentiator->tree, *tree_node, current_pos));

        int read_bytes = 0;
        sscanf(*current_pos, "\"%*[^\"]\"%n", &read_bytes);
        *(*current_pos + read_bytes - 1) = '\0'; // close_" -> '\0'

        type_t value = {.number = 0};

        Type_node type = FindOutType(differentiator, *current_pos + 1, &value); // +1 because skip open_"

        FillNodeInfo(*tree_node, type, value, NULL, NULL, parent);

        ON_DEBUG(DUMP_CURRENT_SITUATION(differentiator->tree, *tree_node, current_pos));

        *current_pos += read_bytes;
        ON_DEBUG(DUMP_CURRENT_SITUATION(differentiator->tree, *tree_node, current_pos));

        TREE_CHECK_AND_RETURN_ERRORS(ReadNode(differentiator, &(*tree_node)->left_node, current_pos, *tree_node));

        TREE_CHECK_AND_RETURN_ERRORS(ReadNode(differentiator, &(*tree_node)->right_node, current_pos, *tree_node));

        (*current_pos)++; // ++ because skip ')'
        ON_DEBUG(DUMP_CURRENT_SITUATION(differentiator->tree, *tree_node, current_pos));

        SkipSpaces(current_pos);
    }

    else if (strncmp(*current_pos, "nil", LEN_NIL) == 0) {
        *current_pos += LEN_NIL;
        ON_DEBUG(DUMP_CURRENT_SITUATION(differentiator->tree, *tree_node, current_pos));

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

    Type_node type_operator = ItIsOperator(buffer, value);
    if (type_operator != WRONG_TYPE)
        return type_operator;

    if (ItIsNumber(buffer, value) == FIND_YES)
        return NUMBER;

    if (ItIsVariable(differentiator, buffer, value) == FIND_YES)
        return VARIABLE;

    return WRONG_TYPE;
}

Type_node ItIsOperator(char* buffer, type_t* value) {
    assert(buffer);

    Type_node type_node = WRONG_TYPE;

    if (strcmp(buffer, "+") == 0)
        type_node = OPERATOR_ADD;

    else if (strcmp(buffer, "-") == 0)
        type_node = OPERATOR_SUB;

    else if (strcmp(buffer, "*") == 0)
        type_node = OPERATOR_MUL;

    else if (strcmp(buffer, "\\") == 0)
        type_node = OPERATOR_DIV;

    else if (strcmp(buffer, "^") == 0)
        type_node = OPERATOR_POW;

    value->operators = type_node;

    return type_node;
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

Tree_status FillNodeInfo(Tree_node* tree_node, Type_node type, type_t value,
                         Tree_node* left_node, Tree_node* right_node, Tree_node* parent) {
    assert(tree_node);

    tree_node->left_node  = left_node;
    tree_node->right_node = right_node;
    tree_node->parent     = parent;

    if (type == NUMBER)
        tree_node->value.number = value.number;

    if (type == VARIABLE)
        tree_node->value.index_variable = value.index_variable;

    // if type == OPERATOR
    if (type != WRONG_TYPE)
        tree_node->value.operators = value.operators;

    tree_node->type = type;

    return SUCCESS;
}

void SkipSpaces(char** buffer) {
    assert(buffer);
    assert(*buffer);

    while (isspace(**buffer) && (**buffer) != '\0')
        (*buffer)++;
}

Tree_status CalculateValueOfExample(Differentiator* differentiator) {
    assert(differentiator);

    for (size_t i = 0; i < differentiator->array_with_variables.size; ++i)
        TREE_CHECK_AND_RETURN_ERRORS(FillValueOfVariables(differentiator, i));

    TreeHTMLDump(differentiator, &differentiator->tree, differentiator->tree.root, DUMP_INFO, NOT_ERROR_DUMP);

    double result = Calculating(differentiator, differentiator->tree.root);

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
        case WRONG_TYPE: return 0.0;
        default: break;
    }

    return 0.0;
}


Tree_status TreeTexDump(Differentiator* differentiator, Tree* tree) {
    FILE* tex_dump_file = NULL;
    tex_dump_file = fopen(differentiator->dump_info.tex_dump_filename, "w");
    if (tex_dump_file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    fprintf(tex_dump_file, "\\documentclass[12pt]{article}\n");
    fprintf(tex_dump_file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(tex_dump_file, "\\usepackage{amsmath}\n");
    fprintf(tex_dump_file, "\\begin{document}\n");
    fprintf(tex_dump_file, "\\[\n");

    PrintExpressionToTex(differentiator, tree->root, tex_dump_file, NO_PRIOTITET);

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

void PrintExpressionToTex(Differentiator* differentiator, Tree_node* tree_node, FILE* tex_dump_file, Prioritets parent_prioritet) {
    Prioritets current_prioritet = GetPrioritet(tree_node);

    if (current_prioritet < parent_prioritet && current_prioritet != NO_PRIOTITET)
        fprintf(tex_dump_file, "(");

    switch (tree_node->type) {
        case NUMBER:
            if (tree_node->value.number < 0)
                fprintf(tex_dump_file, "(%lg)", tree_node->value.number);
            else
                fprintf(tex_dump_file, "%lg", tree_node->value.number);
            break; 
        case VARIABLE:
            fprintf(tex_dump_file, "%lg", ValueOfVariable(differentiator, tree_node));
            break;
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
        case WRONG_TYPE:
        default:
            break;
    }

    if (current_prioritet < parent_prioritet && current_prioritet != NO_PRIOTITET)
        fprintf(tex_dump_file, ")");
}

Prioritets GetPrioritet(Tree_node* tree_node) {
    switch(tree_node->type) {
        case OPERATOR_ADD:
        case OPERATOR_SUB:
            return LOW_PRIOTITET;
        case OPERATOR_MUL:
        case OPERATOR_DIV:
            return MIDDLE_PRIORITET;
        case OPERATOR_POW:
            return HIGH_PRIORITET;
        case NUMBER:
        case VARIABLE:
        case WRONG_TYPE:
        default:
            return NO_PRIOTITET;
    }

    return NO_PRIOTITET;
}


Tree_status DifferentiatorDtor(Differentiator* differentiator) {
    Tree_status status = TreeVerify(differentiator, &differentiator->tree);

    free(differentiator->begin_buffer);

    DifferentiatorNodeDtor(differentiator, differentiator->tree.root);
    differentiator->tree.root = NULL;

    ArrayDtorVariables(&differentiator->array_with_variables);
    free(differentiator->array_with_variables.data);

    differentiator->tree.size = 0;

    return status;
}

void DifferentiatorNodeDtor(Differentiator* differentiator, Tree_node* tree_node) {
    if (tree_node == NULL)
        return;

    DifferentiatorNodeDtor(differentiator, tree_node->left_node);

    DifferentiatorNodeDtor(differentiator, tree_node->right_node);

    free(tree_node);
}