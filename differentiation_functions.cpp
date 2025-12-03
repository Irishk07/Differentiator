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
    assert(variable);

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

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{center}\n"
                                                         "\\section*{Промуррифицируем следующее выражение %dй раз:}\n"
                                                         "\\addcontentsline{toc}{section}{Промуррифицируем следующее выражение %dй раз}\n"
                                                         "\\begin{math}\n\\underline{", cur_cnt, cur_cnt);
        PrintExpressionToTex(differentiator, old_tree_root, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
        fprintf(differentiator->dump_info.tex_dump_file, "}\\end{math}\n"
                                                         "\\end{center}\n");
        
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
#define SIN_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SIN}, left, NULL)
#define COS_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_COS}, left, NULL)
#define TAN_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_TG}, left, NULL)
#define COT_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CTG}, left, NULL)
#define ASIN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCSIN}, left, NULL)
#define ACOS_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCCOS}, left, NULL)
#define ATAN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCTG}, left, NULL)
#define ACOT_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCCTG}, left, NULL)
#define SH_(left)   NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SH}, left, NULL)
#define CH_(left)   NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CH}, left, NULL)
#define TH_(left)   NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_TH}, left, NULL)
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
                new_node = DoOperatorPow(differentiator, tree, tree_node, variable); break;
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

Tree_node* DoOperatorPow(Differentiator* differentiator, Tree* tree, Tree_node* tree_node, const char* variable) {
    Status_of_finding find_in_left_node = ContainsVariable(differentiator, tree_node->left_node, variable);
    Status_of_finding find_in_right_node = ContainsVariable(differentiator, tree_node->right_node, variable);
    if (find_in_left_node && !find_in_right_node) {
        TEX_POW_X_N; 
        return MUL_(cR, MUL_(POW_(cL, SUB_(cR, NUMBER_NODE_CTOR(1))), dL));
    }
    else {
        TEX_POW_X_X; 
        return MUL_(POW_(cL, cR), ADD_(MUL_(dR, LN_(cL)), DIV_(MUL_(cR, dL), cL)));
    }
}

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
    fprintf(differentiator->dump_info.tex_dump_file, "\n\n\\noindent %s\n\n", CONNECTING_PHRASES[cur_index]);
}

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    assert(differentiator);
    assert(variable);

    if (tree_node == NULL)
        return FIND_NO;
    
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

    OptimizationOneNode(differentiator, old_node, (*old_node)->left_node, (*old_node)->right_node);
    OptimizationOneNode(differentiator, old_node, (*old_node)->right_node, (*old_node)->left_node);

    OptimizationZeroNode(differentiator, tree, old_node, (*old_node)->left_node, (*old_node)->right_node);
    OptimizationZeroNode(differentiator, tree, old_node, (*old_node)->right_node, (*old_node)->left_node);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

void OptimizationZeroNode(Differentiator* differentiator, Tree* tree, Tree_node** tree_node, Tree_node* first_child, Tree_node* second_child) {
    assert(differentiator);
    assert(tree_node);

    if (IsZeroNode(first_child)) {
        switch ((*tree_node)->value.operators) {
                case OPERATOR_ADD:
                case OPERATOR_SUB:
                    {
                        Tree_node* child = second_child;
                        DifferentiatorNodeDtor(differentiator, first_child);
                        free(*tree_node);
                        (*tree_node) = child;
                        return;
                    }
                case OPERATOR_MUL:
                    {
                        DifferentiatorNodeDtor(differentiator, (*tree_node));
                        (*tree_node) = NUMBER_NODE_CTOR(0.0);
                        return;
                    }
                case OPERATOR_POW:
                    {
                        DifferentiatorNodeDtor(differentiator, (*tree_node));
                        (*tree_node) = NUMBER_NODE_CTOR(1.0);
                        return;
                    }
                default:
                    break;
                }
    }
}

void OptimizationOneNode(Differentiator* differentiator, Tree_node** tree_node, Tree_node* first_child, Tree_node* second_child) {
    assert(differentiator);
    assert(tree_node);

    if (IsOneNode(first_child)) {
        switch ((*tree_node)->value.operators) {
            case OPERATOR_MUL:
            case OPERATOR_DIV:
            case OPERATOR_POW:
                {
                    Tree_node* child = second_child;
                    DifferentiatorNodeDtor(differentiator, first_child);
                    free((*tree_node));
                    (*tree_node) = child;
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

    return (node->type == NUMBER && fabs(node->value.number) < EPS);
}

bool IsOneNode(Tree_node* node) {
    if (node == NULL) 
        return false;

    return (node->type == NUMBER && fabs(node->value.number - 1.0) < EPS);
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
    assert(tree_node);
    assert(variable);

    TreeHTMLDump(differentiator, tree_node, DUMP_INFO, NOT_ERROR_DUMP);

    double value_of_function = Calculating(differentiator, tree_node);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "f(%s)^{(%zu)} = (", variable, number);
    PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ")(%s = %lg) = %lg\\\\\n", variable, value, value_of_function);
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");

    return value_of_function;
}


Tree_status CreateTaylorsTree(Differentiator* differentiator, char* varible, double value) {
    assert(differentiator);
    assert(varible);

    About_tree* about_Taylor_tree = (About_tree*)calloc(1, sizeof(About_tree));
    Tree* Taylor_tree = (Tree*)calloc(1, sizeof(Tree));

    about_Taylor_tree->tree = Taylor_tree;

    Tree_node* variable_node = NodeCtor(Taylor_tree, VARIABLE, (type_t){.index_variable = 0}, NULL, NULL);
    Tree_node* value_node = NodeCtor(Taylor_tree, NUMBER, (type_t){.number = value}, NULL, NULL);
    Tree_node* x_minus_x0 = NodeCtor(Taylor_tree, OPERATOR, (type_t){.operators = OPERATOR_SUB}, variable_node, value_node);

    Tree_node* tree_node = NodeCtor(Taylor_tree, NUMBER, (type_t){.number = ValueOfTreeFromIndex(differentiator, 0)}, NULL, NULL);

    for (size_t i = 1; i < differentiator->array_with_trees.size; ++i) {
        tree_node = NodeCtor(Taylor_tree, OPERATOR, (type_t){.operators = OPERATOR_ADD}, 
                    tree_node, CreateTaylorNode(Taylor_tree, (double)i, ValueOfTreeFromIndex(differentiator, i), (double)Factorial(i), x_minus_x0));
    }

    Taylor_tree->root = tree_node;
    Taylor_tree->size -= 3;

    free(variable_node);
    free(value_node);
    free(x_minus_x0);

    ArrayPushtrees(&differentiator->array_with_trees, about_Taylor_tree);

    TreeHTMLDump(differentiator, Taylor_tree->root, DUMP_INFO, NOT_ERROR_DUMP);
    
    return SUCCESS;
}

Tree_node* CreateTaylorNode(Tree* tree, double exp, double dif_value, double factorial, Tree_node* x_minus_x0) {
    assert(tree);
    assert(x_minus_x0);

    Tree_node* tree_exp = NodeCtor(tree, NUMBER, (type_t){.number = exp}, NULL, NULL);
    Tree_node* tree_pow = NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_POW}, CopyNode(x_minus_x0), tree_exp);
    Tree_node* tree_dif = NodeCtor(tree, NUMBER, (type_t){.number = dif_value}, NULL, NULL);
    Tree_node* tree_fact = NodeCtor(tree, NUMBER, (type_t){.number = factorial}, NULL, NULL);
    Tree_node* tree_div = NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_DIV}, tree_dif, tree_fact);

    return NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_MUL}, tree_div, tree_pow);
}

Tree_node* CopyNode(Tree_node* tree_node) {
    if (tree_node == NULL) 
        return NULL;
    
    Tree_node* new_node = (Tree_node*)calloc(1, sizeof(Tree_node));
    if (new_node == NULL) 
        return NULL;

    new_node->type = tree_node->type;
    new_node->value = tree_node->value;

    new_node->left_node = CopyNode(tree_node->left_node);
    new_node->right_node = CopyNode(tree_node->right_node);
    
    return new_node;
}

Tree_status FormulaTaylora(Differentiator* differentiator, char* variable, double value) {
    assert(differentiator);
    assert(variable);

    CreateTaylorsTree(differentiator, variable, value);

    fprintf(differentiator->dump_info.tex_dump_file, "\\newpage \\begin{center}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\section*{Разложим по формуле Тейлоррррррра:}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Разложим по формуле Тейлоррррррра}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{center}\n");

    fprintf(differentiator->dump_info.tex_dump_file, "Разложим по формуле Тейлора в точке $%s_0$ = %lg\\\\\n", variable, value);

    size_t cnt_tree = differentiator->array_with_trees.size;

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "f(%s) = f(%s_0)", variable, variable);
    for (size_t i = 1; i < cnt_tree - 1; ++i) {
        fprintf(differentiator->dump_info.tex_dump_file, " + \\frac{f^{(%zu)}(%s_0)}{%zu!} \\cdot (%s - %s_0)^{%zu}", i, variable, i, variable, variable, i);
    }
    fprintf(differentiator->dump_info.tex_dump_file, "+ o((%s - %s_0)^%zu)\\\\\n", variable, variable, cnt_tree - 2);

    Tree_node* root = PointerOnTreeRoot(differentiator);

    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\nf(%s) = ", variable);
    PrintExpressionToTex(differentiator, root, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, "+ o((%s - %s_0)^%zu)\\\\\n", variable, variable, cnt_tree - 2);

    OptimizationTree(differentiator, &(differentiator->array_with_trees.data[differentiator->array_with_trees.size - 1]->tree->root), variable);

    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\nf(%s) = ", variable);
    PrintExpressionToTex(differentiator, PointerOnTreeRoot(differentiator), differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, "+ o((%s - %s_0)^%zu)\\\\\n", variable, variable, cnt_tree - 2);
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");

    return SUCCESS;
}

size_t Factorial(size_t n) {
    if (n <= 1) 
        return 1;

    return n * Factorial(n - 1);
}

Tree_status GNUPlot(Differentiator* differentiator) {
    assert(differentiator);

    double point_tangency = differentiator->array_with_variables.data[0]->value;

    fprintf(differentiator->dump_info.tex_dump_file, "\\newpage\\section*{Построим следы хвоста кота на бумаге}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{section}{Построим следы хвоста кота на бумаге}\n");

    for (size_t i = 0; i < differentiator->array_with_trees.size - 1; ++i) {
        char data_file_name[MAX_LEN_NAME] = {};
        char image[MAX_LEN_NAME] = {};
        char GNUPlot_file[MAX_LEN_NAME] = {};
        char command[MAX_LEN_NAME] = {};

        snprintf(data_file_name, MAX_LEN_NAME, "GNUPlot/data_file_name_%zu.dat", i);
        snprintf(image, MAX_LEN_NAME, "GNUPlot/image_%zu.png", i);
        snprintf(GNUPlot_file, MAX_LEN_NAME, "GNUPlot/graphic_%zu.plt", i); 

        CreateFileWithData(differentiator, PointerOnTreeRootFromIndex(differentiator, i), data_file_name);

        if (i == 0) {
            char data_file_2_name[MAX_LEN_NAME] = {};
            char data_file_tangency_name[MAX_LEN_NAME] = {};

            snprintf(data_file_2_name, MAX_LEN_NAME, "GNUPlot/data_file_2_name.dat");
            snprintf(data_file_tangency_name, MAX_LEN_NAME, "GNUPlot/data_file_tangency_name.dat");

            CreateFileWithData(differentiator, PointerOnTreeRoot(differentiator), data_file_2_name);
            CreateFileWithTangency(differentiator, data_file_tangency_name, point_tangency);

            CreateGNUPlotFileThreeFunction(GNUPlot_file, data_file_name, data_file_2_name, data_file_tangency_name, image);

            fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{Изначальные следы хвоста кота на бумаге и формула Тейлорррррра}\n");
            fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{Изначальные следы хвоста кота на бумаге и формула Тейлорррррра}\n");

            fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
            fprintf(differentiator->dump_info.tex_dump_file, "f_T(x) = ");
            PrintExpressionToTex(differentiator, PointerOnTreeRoot(differentiator), differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
            fprintf(differentiator->dump_info.tex_dump_file, "\n\\end{math} - \\textcolor{blue}{Формула Тейлорррррррррра}\\\\\n");
            
            fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
            fprintf(differentiator->dump_info.tex_dump_file, "f_k(x) = %lg + (%lg) \\cdot (x - %lg)", 
                                                              ValueOfTreeFromIndex(differentiator, 0), ValueOfTreeFromIndex(differentiator, 1), point_tangency); 
            fprintf(differentiator->dump_info.tex_dump_file, "\n\\end{math} - \\textcolor{red}{Касательная в точке x = %lg}\\\\\n", point_tangency);
        }
        else {
            CreateGNUPlotFile(GNUPlot_file, data_file_name, image);

            fprintf(differentiator->dump_info.tex_dump_file, "\\subsection*{Следы хвоста кота на бумаге после %zu муррифицирования}\n", i);
            fprintf(differentiator->dump_info.tex_dump_file, "\\addcontentsline{toc}{subsection}{Следы хвоста кота на бумаге после %zu муррифицирования}\n", i);
        }
            
        snprintf(command, MAX_LEN_NAME, "gnuplot %.*s", MAX_LEN_NAME - 10, GNUPlot_file);
        fprintf(stderr, "%s\n", command);
        if (system(command) != 0)
            TREE_CHECK_AND_RETURN_ERRORS(EXECUTION_FAILED,      fprintf(stderr, "Error with GNUPlot\n"));

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
        fprintf(differentiator->dump_info.tex_dump_file, "f(x) = ");
        PrintExpressionToTex(differentiator, PointerOnTreeRootFromIndex(differentiator, i), differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
        fprintf(differentiator->dump_info.tex_dump_file, "\n\\end{math} - \\textcolor{purple}{Функция}\\\\\n");

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{figure}[h]\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\centering\n");
        fprintf(differentiator->dump_info.tex_dump_file, "\\includegraphics[width=0.8\\textwidth]{%s}\n", image);
        fprintf(differentiator->dump_info.tex_dump_file, "\\end{figure}\n\\newpage\n");
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

    double step = 0.05;
    double left_point = 0.5;
    int cnt_points = 60;
    
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

Tree_status CreateFileWithTangency(Differentiator* differentiator, const char* data_file_tangency_name, double point_tangency) {
    assert(differentiator);
    assert(data_file_tangency_name);

    FILE* data_file = fopen(data_file_tangency_name, "w");
    if (data_file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    double step = 0.05;
    double left_point = 0.5;
    int cnt_points = 60;
    
    for (int i = 0; i <= cnt_points; i++) {
        double x = left_point + i * step;

        double y_0 = ValueOfTreeFromIndex(differentiator, 0);
        double y_dif_0 = ValueOfTreeFromIndex(differentiator, 1);
        double x_minus_x_0 = x - point_tangency;

        double y = y_0 + y_dif_0 * x_minus_x_0;
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

Tree_status CreateGNUPlotFileThreeFunction(const char* GNUPlot_file, const char* data_file, const char* data_2_file, const char* data_file_tangency_name, const char* image) {
    assert(GNUPlot_file);
    assert(data_file);
    assert(data_2_file);
    assert(data_file_tangency_name);
    assert(image);

    FILE* file = fopen(GNUPlot_file, "w");

    if (file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);
    
    fprintf(file, "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n");
    fprintf(file, "set output '%s'\n", image);
    fprintf(file, "set grid\n");
    fprintf(file, "set yrange [-3:4]\n");
    fprintf(file, "unset key\n");
    fprintf(file, "plot '%s' with lines lc rgb \"purple\" linewidth 2," 
                    "\\\n '%s' with lines lc rgb \"blue\" linewidth 2," 
                    "\\\n '%s' with lines lc rgb \"red\" linewidth 2", data_file, data_2_file, data_file_tangency_name);
    
    if (fclose(file) == EOF)                                             
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    return SUCCESS;
}