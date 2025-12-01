#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "differentiation_functions.h"

#include "array.h"
#include "colors.h"
#include "common.h"
#include "differentiator.h"
#include "tree.h"


void StartDifferntiator(Differentiator* differentiator) {
    assert(differentiator);

    CreateTexFileForDump(differentiator);

    UploadTree(differentiator);

    color_printf(COLOR_PURPLE, " - By which variable do you want to differentiate?\n");
    char* variable = ReadAnswer();;

    double value = ReadDoubleNumber("Enter value of variable");
    for (size_t i = 0; i < differentiator->array_with_variables.size; ++i) {
        if (strcmp(variable, differentiator->array_with_variables.data[i]->name) == 0)
            differentiator->array_with_variables.data[i]->value = value;
    }

    Differentiation(differentiator, variable);

    CalculateValueAllFunctions(differentiator, variable, value);

    FormulaTaylora(differentiator, variable, value);

    GNUPlot(differentiator);

    ClostTexFileForDump(differentiator);

    free(variable);
}


Tree_status Differentiation(Differentiator* differentiator, char* variable) {
    assert(differentiator);

    int number = ReadIntNumber("Enter, please, number of differentiations");
    if (number == -1)
        return INPUT_ERROR;

    int cur_cnt = 0;
    while (cur_cnt++ != number){

        Tree_node* old_tree_root = PointerOnTree(differentiator)->root;

        About_tree* about_new_tree = (About_tree*)calloc(1, sizeof(About_tree));
        about_new_tree->tree = (Tree*)calloc(1, sizeof(Tree));
        if (about_new_tree == NULL)
            TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR,      free(variable););

        TREE_CHECK_AND_RETURN_ERRORS(ArrayPushtrees(&differentiator->array_with_trees, about_new_tree),   free(variable););

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}");
        fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Промуррифицируем следующее выражение %dй раз:}\n", cur_cnt);
        fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Промуррифицируем следующее выражение %dй раз}\n", cur_cnt);
        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n\\underline{");
        PrintExpressionToTex(differentiator, old_tree_root, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
        fprintf(differentiator->dump_info.tex_dump_file, "}\\end{math}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}");
        
        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
        about_new_tree->tree->root = DifferentiationFunctions(differentiator, old_tree_root, variable);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");

        TreeHTMLDump(differentiator, PointerOnTreeRoot(differentiator), DUMP_INFO, NOT_ERROR_DUMP);

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{Результат %dго муррифицирования:}\n\n", cur_cnt);
        fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{Результат %dго муррифицирования}\n", cur_cnt);
        TreeTexDump(differentiator, old_tree_root, PointerOnTreeRoot(differentiator), variable);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}\n");

        OptimizationTree(differentiator, &about_new_tree->tree->root, variable);

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{После оптимизациии:}\n\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{После оптимизациии}\n");
        TreeTexDump(differentiator, old_tree_root, PointerOnTreeRoot(differentiator), variable);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}\n");
    }

    return SUCCESS;
}

#define dL DifferentiationFunctions(differentiator, tree_node->left_node, variable)
#define dR DifferentiationFunctions(differentiator, tree_node->right_node, variable)
#define cL CopyPartOfTree(differentiator, tree_node->left_node)
#define cR CopyPartOfTree(differentiator, tree_node->right_node)
#define NUMBER_NODE_CTOR(num) NodeCtor(tree, NUMBER, (type_t){.number = num}, NULL, NULL)
#define ADD_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ADD}, left, right)
#define SUB_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SUB}, left, right)
#define MUL_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_MUL}, left, right)
#define DIV_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_DIV}, left, right)
#define POW_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_POW}, left, right)
#define LN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_LN}, left, NULL)
#define LOG_BASE_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_LOG}, left, right)
#define SIN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SIN}, left, NULL)
#define COS_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_COS}, left, NULL)
#define TAN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_TG}, left, NULL)
#define COT_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CTG}, left, NULL)
#define ASIN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCSIN}, left, NULL)
#define ACOS_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCCOS}, left, NULL)
#define ATAN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCTG}, left, NULL)
#define ACOT_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCCTG}, left, NULL)
#define SH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SH}, left, NULL)
#define CH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CH}, left, NULL)
#define TH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_TH}, left, NULL)
#define CTH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CTH}, left, NULL)

#define DIFFERENTIANTIONS_TO_TEX
#include "tex.h"
                                                                               

Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    assert(differentiator);
    assert(tree_node);
    assert(variable);

    Tree* tree = PointerOnTree(differentiator);
    Tree_node* new_node = NULL;
    
    TEX_CONNECTING_PHRASES;
    fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);
    PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ") = ");

    if (tree_node->type == VARIABLE) {
        int result_compare = strcmp(NameOfVariable(differentiator, tree_node), variable);

        if (result_compare != 0) {
            TEX_CONST;
            new_node = NodeCtor(tree, NUMBER, (type_t){.number = 0}, tree_node->left_node, tree_node->right_node);
        }
        else {
            TEX_VARIABLE;
            new_node = NodeCtor(tree, NUMBER, (type_t){.number = 1}, tree_node->left_node, tree_node->right_node);
        }
    }        

    if (tree_node->type == NUMBER) {
        TEX_CONST;
        new_node = NodeCtor(tree, NUMBER, (type_t){.number = 0}, tree_node->left_node, tree_node->right_node);
    }

    if (tree_node->type == OPERATOR) {
        switch(tree_node->value.operators) {
            case OPERATOR_ADD:
                TEX_ADD; new_node = ADD_(dL, dR); break;
            case OPERATOR_SUB:
                TEX_SUB; new_node = SUB_(dL, dR); break;
            case OPERATOR_MUL:
                TEX_MUL; new_node = ADD_(MUL_(dL, cR), MUL_(cL, dR)); break;
            case OPERATOR_DIV:
                TEX_DIV; new_node = DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), POW_(cR, NUMBER_NODE_CTOR(2))); break;
            case OPERATOR_POW:
                {
                Status_of_finding find_in_left_node = ContainsVariable(differentiator, tree_node->left_node, variable);
                Status_of_finding find_in_right_node = ContainsVariable(differentiator, tree_node->right_node, variable);
                if (find_in_left_node && !find_in_right_node) {
                    TEX_POW_X_N; new_node = MUL_(cR, MUL_(POW_(cL, SUB_(cR, NUMBER_NODE_CTOR(1))), dL));
                }
                else {
                    TEX_POW_X_X; new_node = MUL_(POW_(cL, cR), ADD_(MUL_(dR, LN_(cL)), DIV_(MUL_(cR, dL), cL)));
                }
                break;
                }
            case OPERATOR_LN:
                TEX_LN; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), cL), dL); break;
            case OPERATOR_LOG:
                TEX_LOG; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), MUL_(cR, LN_(cL))), dR); break;
            case OPERATOR_SIN:
                TEX_SIN; new_node = MUL_(COS_(cL), dL); break;
            case OPERATOR_COS:
                TEX_COS; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), SIN_(cL)), dL); break;
            case OPERATOR_TG:
                TEX_TG; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(COS_(cL), NUMBER_NODE_CTOR(2))), dL); break;
            case OPERATOR_CTG:
                TEX_CTG; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SIN_(cL), NUMBER_NODE_CTOR(2)))), dL); break;
            case OPERATOR_ARCSIN:
                TEX_ARCSIN; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(SUB_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))), NUMBER_NODE_CTOR(0.5))), dL); break;
            case OPERATOR_ARCCOS:
                TEX_ARCCOS; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SUB_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))), NUMBER_NODE_CTOR(0.5)))), dL); break;
            case OPERATOR_ARCTG:
                TEX_ARCTG; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), ADD_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2)))), dL); break;
            case OPERATOR_ARCCTG:
                TEX_ARCCTG; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), ADD_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))))), dL); break;
            case OPERATOR_SH:
                TEX_SH; new_node = MUL_(CH_(cL), dL); break;
            case OPERATOR_CH:
                TEX_CH; new_node = MUL_(SH_(cL), dL); break;
            case OPERATOR_TH:
                TEX_TH; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(CH_(cL), NUMBER_NODE_CTOR(2))), dL); break;
            case OPERATOR_CTH:
                TEX_CTH; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SH_(cL), NUMBER_NODE_CTOR(2)))), dL); break;
            case WRONG_OPERATOR:
            default: return NULL;
        }
    }

    if (tree_node->type == OPERATOR) {
        TEX_CONNECTING_PHRASES;
        fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);
        PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
        fprintf(differentiator->dump_info.tex_dump_file, ") = ");
        PrintExpressionToTex(differentiator, new_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
        fprintf(differentiator->dump_info.tex_dump_file, "\\\\\n");
    }

    return new_node;
}

#undef DIFFERENTIATION_TO_TEX

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node) {
    assert(differentiator);

    if (old_node == NULL) {
        return NULL;
    }

    Tree_node* new_node = NodeCtor(PointerOnTree(differentiator), old_node->type, old_node->value, NULL, NULL);
    if (new_node == NULL) {
        return NULL;
    }

    new_node->type = old_node->type;
    new_node->value = old_node->value;

    new_node->left_node = CopyPartOfTree(differentiator, old_node->left_node);
    new_node->right_node = CopyPartOfTree(differentiator, old_node->right_node);

    return new_node;
}    

void PrintfConnectingPhrases(Differentiator* differentiator) {
    assert(differentiator);

    size_t cur_index = (size_t)rand() % CNT_OF_PHRASES;
    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\\noindent %s\n\n", CONNECTING_PHRASES[cur_index]);
}

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    assert(differentiator);
    assert(variable);

    if (tree_node == NULL) return FIND_NO;
    
    switch (tree_node->type) {
        case VARIABLE:
            if (strcmp(NameOfVariable(differentiator, tree_node), variable) == 0)
                return FIND_YES;
            return FIND_NO;
        case NUMBER:
        case OPERATOR:
            if (ContainsVariable(differentiator, tree_node->left_node, variable) == FIND_YES ||
                ContainsVariable(differentiator, tree_node->right_node, variable) == FIND_YES)
                    return FIND_YES;
            else
                return FIND_NO;
        case WRONG_TYPE:
        default:
            return FIND_NO;
    }

    return FIND_NO;
}

double ReadDoubleNumber(const char* string) {
    assert(string);
    
    double number = -1;

    int cnt_attempts = CNT_ATTEMPTS;
    while (cnt_attempts-- > 0) {
        color_printf(COLOR_PURPLE, " - %s\n", string);

        if (scanf("%lg%*c", &number) != 1)
            scanf("%*[^\n]%*c");

        if (number >= 0)
            return number;
    }

    return number;
}

int ReadIntNumber(const char* string) {
    assert(string);

    int number = -1;

    int cnt_attempts = CNT_ATTEMPTS;
    while (cnt_attempts-- > 0) {
        color_printf(COLOR_PURPLE, " - %s\n", string);

        if (scanf("%d%*c", &number) != 1)
            scanf("%*[^\n]%*c");

        if (number >= 0)
            return number;
    }

    return number;
}

void OptimizationTree(Differentiator* differentiator, Tree_node** old_node, const char* variable) {
    assert(differentiator);
    assert(variable);

    if (old_node == NULL) 
        return;

    if ((*old_node)->left_node)
        OptimizationTree(differentiator, &(*old_node)->left_node, variable);
    if ((*old_node)->right_node)
        OptimizationTree(differentiator, &(*old_node)->right_node, variable);

    if ((*old_node)->type == OPERATOR)
        OptimizationNode(differentiator, old_node, variable);
}

void OptimizationNode(Differentiator* differentiator, Tree_node** old_node, const char* variable) {
    assert(differentiator);
    assert(old_node);
    assert(variable);

    Tree* tree = PointerOnTree(differentiator);

    if (IsConstantNode(differentiator, *old_node, variable)) {
        double result = Calculating(differentiator, (*old_node));
        DifferentiatorNodeDtor(differentiator, *old_node);
        *old_node = NUMBER_NODE_CTOR(result);
        return;
    }
    
    if ((*old_node)->value.operators == OPERATOR_MUL && 
        (IsZeroNode((*old_node)->left_node) || IsZeroNode((*old_node)->right_node))) {
        DifferentiatorNodeDtor(differentiator, (*old_node));
        (*old_node) = NUMBER_NODE_CTOR(0.0);
        return;
    }

    OptimizationOneNode(differentiator, old_node);

    OptimizationZeroNode(differentiator, tree, old_node);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

void OptimizationZeroNode(Differentiator* differentiator, Tree* tree, Tree_node** old_node) {
    assert(differentiator);
    assert(old_node);

    if (IsZeroNode((*old_node)->left_node)) {
        switch ((*old_node)->value.operators) {
                case OPERATOR_ADD:
                case OPERATOR_SUB:
                    {
                        Tree_node* right_child = (*old_node)->right_node;
                        DifferentiatorNodeDtor(differentiator, (*old_node)->left_node);
                        free((*old_node));
                        (*old_node) = right_child;
                        return;
                    }
                case OPERATOR_POW:
                    {
                        DifferentiatorNodeDtor(differentiator, (*old_node)->left_node);
                        (*old_node) = NUMBER_NODE_CTOR(1.0);
                        return;
                    }
                default:
                    break;
                }
    }

    if (IsZeroNode((*old_node)->right_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_ADD:
            case OPERATOR_SUB:
                {
                    Tree_node* left_child = (*old_node)->left_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->right_node);
                    free((*old_node));
                    (*old_node) = left_child;
                    return;
                }
            case OPERATOR_POW:
                {
                    DifferentiatorNodeDtor(differentiator, (*old_node)->right_node);
                    free((*old_node));
                    (*old_node) = NUMBER_NODE_CTOR(1.0);
                    return;
                }
            default:
                break;
            }
    }
}

void OptimizationOneNode(Differentiator* differentiator, Tree_node** old_node) {
    assert(differentiator);
    assert(old_node);

    if (IsOneNode((*old_node)->left_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_MUL:
            case OPERATOR_DIV:
            case OPERATOR_POW:
                {
                    Tree_node* right_child = (*old_node)->right_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->left_node);
                    free((*old_node));
                    (*old_node) = right_child;
                    return;
                }            
            default:
                break;
        }
    }

    if (IsOneNode((*old_node)->right_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_MUL:
            case OPERATOR_DIV:
            case OPERATOR_POW:
                {
                    Tree_node* left_child = (*old_node)->left_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->right_node);
                    free((*old_node));
                    (*old_node) = left_child;
                    return;
                }
            default:
                break;
        }
    }
}

#pragma GCC diagnostic pop

bool IsConstantNode(Differentiator* differentiator, Tree_node* node, const char* variable) {
    assert(differentiator);
    assert(variable);

    if (node == NULL) return true;
    
    switch (node->type) {
        case NUMBER:
            return true;
            
        case VARIABLE:
            return strcmp(NameOfVariable(differentiator, node), variable) != 0;
            
        case OPERATOR:
            return IsConstantNode(differentiator, node->left_node, variable) && 
                   IsConstantNode(differentiator, node->right_node, variable);
        case WRONG_TYPE:
        default:
            return false;
    }
}

bool IsZeroNode(Tree_node* node) {
    if (node == NULL) 
        return false;

    return (node->type == NUMBER && fabs(node->value.number) < 1e-10);
}

bool IsOneNode(Tree_node* node) {
    if (node == NULL) 
        return false;

    return (node->type == NUMBER && fabs(node->value.number - 1.0) < 1e-10);
}

void CalculateValueAllFunctions(Differentiator* differentiator, char* variable, double value) {
    assert(differentiator);
    assert(variable);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Посчитаем значение функции в миске:}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Посчитаем значение функции в миске}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}\n");

    for (size_t i = 0; i < differentiator->array_with_trees.size; ++i)
        differentiator->array_with_trees.data[i]->value = CalculateValueOfFunction(differentiator, PointerOnTreeRootFromIndex(differentiator, i), variable, value, i);
}

double CalculateValueOfFunction(Differentiator* differentiator, Tree_node* tree_node, char* variable, double value, size_t number) {
    assert(differentiator);

    TreeHTMLDump(differentiator, tree_node, DUMP_INFO, NOT_ERROR_DUMP);

    double value_of_function = Calculating(differentiator, tree_node);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "f(%s)^{(%zu)} = (", variable, number);
    PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ")(%s = %lg) = %lg\\\\\n", variable, value, value_of_function);
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");

    return value_of_function;
}

Tree_status FormulaTaylora(Differentiator* differentiator, char* variable, double value) {
    assert(differentiator);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Разложим по формуле Тейлоррррррра:}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Разложим по формуле Тейлоррррррра}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}\n");

    size_t cnt_tree = differentiator->array_with_trees.size;

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "f(%s) = f(%s_0)", variable, variable);
    for (size_t i = 1; i < cnt_tree; ++i) {
        fprintf(differentiator->dump_info.tex_dump_file, " + \\frac{f^{(%zu)}(%s_0)}{%zu!} \\cdot (%s - %s_0)^{%zu}", i, variable, i, variable, variable, i);
    }
    fprintf(differentiator->dump_info.tex_dump_file, "+ o((x - %s_0)^%zu)\\\\\n", variable, cnt_tree - 1);

    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\nf(%s) = %lg", variable, ValueOfTreeFromIndex(differentiator, 0));
    for (size_t i = 1; i < cnt_tree; ++i) {
        fprintf(differentiator->dump_info.tex_dump_file, " + \\frac{%lg}{%zu} \\cdot (%s - %lg)^{%zu}", ValueOfTreeFromIndex(differentiator, i), Factorial(i),
                                                                                           variable, value, i);
    }
    fprintf(differentiator->dump_info.tex_dump_file, "+ o((x - %lg)^%zu)\n", value, cnt_tree - 1);

    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\nf(%s) = %lg", variable, ValueOfTreeFromIndex(differentiator, 0));
    for (size_t i = 1; i < cnt_tree; ++i) {
        fprintf(differentiator->dump_info.tex_dump_file, " + (%lg) \\cdot (%s - %lg)^{%zu}", ValueOfTreeFromIndex(differentiator, i) / Factorial(i),
                                                                                           variable, value, i);
    }
    fprintf(differentiator->dump_info.tex_dump_file, "+ o((x - %lg)^%zu)\\end{math}\n", value, cnt_tree - 1);

    return SUCCESS;
}

size_t Factorial(size_t n) {
    if (n == 0 || n == 1) 
        return 1;

    return n * Factorial(n - 1);
}

Tree_status GNUPlot(Differentiator* differentiator) {
    assert(differentiator);

    fprintf(differentiator->dump_info.tex_dump_file, "\\newpage\\section*{Построим следы хвоста кота на бумаге}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Построим следы хвоста кота на бумаге}\n");

    for (size_t i = 0; i < differentiator->array_with_trees.size; ++i) {
        char data_file_name[MAX_LEN_NAME] = {};
        char image[MAX_LEN_NAME] = {};
        char GNUPlot_file[MAX_LEN_NAME] = {};
        char command[MAX_LEN_NAME] = {};

        snprintf(data_file_name, MAX_LEN_NAME, "GNUPlot/data_file_name_%zu.dat", i);
        snprintf(image, MAX_LEN_NAME, "GNUPlot/image_%zu.png", i);
        snprintf(GNUPlot_file, MAX_LEN_NAME, "GNUPlot/graphic_%zu.plt", i);

        CreateFileWithData(differentiator, PointerOnTreeRootFromIndex(differentiator, i), data_file_name);

        CreateGNUPlotFile(GNUPlot_file, data_file_name, image);

        snprintf(command, MAX_LEN_NAME, "gnuplot %s", GNUPlot_file);
        fprintf(stderr, "%s\n", command);
        if (system(command) != 0)
            TREE_CHECK_AND_RETURN_ERRORS(EXECUTION_FAILED,      fprintf(stderr, "Error with GNUPlot\n"));

        if (i == 0) {
            fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{Изначальные следы хвоста кота на бумаге}\n");
            fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{Изначальные следы хвоста кота на бумаге}\n");
        }
        else {
            fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{Следы хвоста кота на бумаге после %zu муррифицирования}\n", i);
            fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{Следы хвоста кота на бумаге после %zu муррифицирования}\n", i);
        }

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{figure}[h]\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\centering\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\includegraphics[width=0.6\\textwidth]{%s}\n", image);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{figure}\n\n");
    }

    return SUCCESS;
}

Tree_status CreateFileWithData(Differentiator* differentiator, Tree_node* tree_node, const char* data_file_name) {
    assert(differentiator);
    assert(tree_node);
    assert(data_file_name);

    FILE* data_file = fopen(data_file_name, "w");
    if (data_file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    double step = 0.5;
    double left_point = -50;
    int cnt_points = 200;
    
    for (int i = 0; i <= cnt_points; i++) {
        double x = left_point + i * step;
        differentiator->array_with_variables.data[0]->value = x;

        double y = Calculating(differentiator, tree_node);
        fprintf(data_file, "%lg %lg\n", x, y);
    }
    
    if (fclose(data_file) == EOF)                                             
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    return SUCCESS;
}

Tree_status CreateGNUPlotFile(const char* GNUPlot_file, const char* data_file, const char* image) {
    assert(GNUPlot_file);
    assert(data_file);
    assert(image);

    FILE* file = fopen(GNUPlot_file, "w");

    if (file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);
    
    fprintf(file, "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n");
    fprintf(file, "set output '%s'\n", image);
    fprintf(file, "set grid\n");
    fprintf(file, "unset key\n");
    fprintf(file, "plot '%s' with lines linewidth 2\n", data_file);
    
    if (fclose(file) == EOF)                                             
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    return SUCCESS;
}