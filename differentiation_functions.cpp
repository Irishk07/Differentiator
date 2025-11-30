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
    char* variable = ReadAnswer();

    double value = ReadDoubleNumber("Enter value of variable");

    Differentiation(differentiator, variable);

    CalculateValueAllFunctions(differentiator, variable, value);

    FormulaTaylora(differentiator, variable, value);

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

        Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
        if (new_tree == NULL)
            TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR,      free(variable););

        TREE_CHECK_AND_RETURN_ERRORS(ArrayPushtrees(&differentiator->array_with_trees, new_tree),   free(variable););

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}");
        fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Промуррифицируем следующее выражение %dй раз:}\n", cur_cnt);
        fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Промуррифицируем следующее выражение %dй раз}\n", cur_cnt);
        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n\\underline{");
        PrintExpressionToTex(differentiator, old_tree_root, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
        fprintf(differentiator->dump_info.tex_dump_file, "}\\end{math}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}");
        
        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
        new_tree->root = DifferentiationFunctions(differentiator, old_tree_root, variable);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");

        TreeHTMLDump(differentiator, PointerOnTree(differentiator)->root, DUMP_INFO, NOT_ERROR_DUMP);

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{Результат %dго муррифицирования:}\n\n", cur_cnt);
        fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{Результат %dго муррифицирования}\n", cur_cnt);
        TreeTexDump(differentiator, old_tree_root, new_tree->root, variable);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}\n");

        OptimizationTree(differentiator, &new_tree->root, variable);

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{После оптимизациии:}\n\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{После оптимизациии}\n");
        TreeTexDump(differentiator, old_tree_root, new_tree->root, variable);
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
    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}");
    fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Посчитаем значение функции в миске:}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Посчитаем значение функции в миске}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}");

    for (size_t i = 0; i < differentiator->array_with_trees.size; ++i)
        CalculateValueOfFunction(differentiator, differentiator->array_with_trees.data[i]->root, variable, value, i);
}

Tree_status CalculateValueOfFunction(Differentiator* differentiator, Tree_node* tree_node, char* variable, double value, size_t number) {
    assert(differentiator);

    TreeHTMLDump(differentiator, tree_node, DUMP_INFO, NOT_ERROR_DUMP);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}");
    fprintf(differentiator->dump_info.tex_dump_file, "f(%s)^{(%zu)} = (", variable, number);
    PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ")(%s = %lg) = %lg\\\\\n", variable, value, Calculating(differentiator, tree_node));
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}");

    return SUCCESS;
}

Tree_status FormulaTaylora(Differentiator* differentiator, char* variable, double value) {
    assert(differentiator);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}");
    fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Разложим по формуле Тейлоррррррра:}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Разложим по формуле Тейлоррррррра}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}");

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}");
    fprintf(differentiator->dump_info.tex_dump_file, "f(%s) = f(%s_0)", variable, variable);
    for (size_t i = 1; i < differentiator->array_with_trees.size; ++i) {
        fprintf(differentiator->dump_info.tex_dump_file, " + \\frac{f^{(%zu)}(%s_0)}{%zu!} \\cdot (%s - %s_0)^{%zu}", i, variable, i, variable, variable, i);
    }

    fprintf(differentiator->dump_info.tex_dump_file, "f(%s) = %lg", variable, Calculating(differentiator, differentiator->array_with_trees.data[0]->root));
    for (size_t i = 1; i < differentiator->array_with_trees.size; ++i) {
        fprintf(differentiator->dump_info.tex_dump_file, " + %lg \\cdot (%s - %lg)^{%zu}", Calculating(differentiator, differentiator->array_with_trees.data[i]->root) / Factorial(i),
                                                                                           variable, value, i);
    }
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}");

    // for (size_t i = 0; i <= differentiator->array_with_trees.size; ++i) {
    //     int cur_fact = Factorial(i + 1);

    // }

    return SUCCESS;
}

size_t Factorial(size_t n) {
    if (n == 0 || n == 1) 
        return 1;

    return n * Factorial(n - 1);
}