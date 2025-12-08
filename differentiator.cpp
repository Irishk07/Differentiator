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
#include "read_expression.h"
#include "onegin.h"
#include "tree.h"


Tree_status DifferentiatorCtor(Differentiator* differentiator, const char* html_dump_filename, const char* tex_dump_filename, const char *directory) {
    assert(differentiator);
    assert(html_dump_filename);
    assert(tex_dump_filename);
    assert(directory);

    differentiator->begin_buffer = NULL;
    differentiator->end_buffer   = NULL;
    differentiator->size_buffer  = 0;

    differentiator->dump_info.html_dump_filename = html_dump_filename;
    differentiator->dump_info.tex_dump_filename  = tex_dump_filename;
    differentiator->dump_info.directory          = directory;

    differentiator->array_with_variables = {};
    ArrayCtor(&(differentiator->array_with_variables), sizeof(About_variable), DEFAULT_START_CAPACITY);
    differentiator->array_with_trees = {};
    ArrayCtor(&(differentiator->array_with_trees), sizeof(About_tree), DEFAULT_START_CAPACITY);

    // differentiator->array_with_variables.data = (About_variable*)ArrayCtor(sizeof(About_variable), DEFAULT_START_CAPACITY, 
    //                                                                        &differentiator->array_with_variables.capacity, &differentiator->array_with_variables.size);
    // differentiator->array_with_variables.elem_size = sizeof(About_variable);
    // differentiator->array_with_trees.data     = (About_tree*)ArrayCtor(sizeof(About_tree), DEFAULT_START_CAPACITY, 
    //                                                                    &differentiator->array_with_trees.capacity, &differentiator->array_with_trees.size);
    // differentiator->array_with_variables.elem_size = sizeof(About_tree);

    // TREE_CHECK_AND_RETURN_ERRORS(ArrayCtorvariables(&differentiator->array_with_variables, DEFAULT_START_CAPACITY));
    // TREE_CHECK_AND_RETURN_ERRORS(ArrayCtortrees(&differentiator->array_with_trees, DEFAULT_START_CAPACITY));

    return SUCCESS;
}


Tree_status UploadTree(Differentiator* differentiator) {
    assert(differentiator);

    color_printf(COLOR_PURPLE, " - Please, enter the name of the file from which the tree will be uploaded\n");

    char* file_name = ReadAnswer();
    differentiator->file_with_function = file_name;

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
    assert(differentiator);
    assert(file_with_tree);

    TREE_CHECK_AND_RETURN_ERRORS(ReadOnegin(differentiator, file_with_tree));

    differentiator->begin_buffer = differentiator->buffer_with_tree;
    differentiator->end_buffer   = differentiator->begin_buffer + differentiator->size_buffer;

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    About_tree about_tree = {.tree = tree};

    ArrayPush(&(differentiator->array_with_trees), &about_tree);
    Tree_status status = SUCCESS;

    about_tree.tree->root = GetComandir(differentiator, &status, &differentiator->buffer_with_tree);

    Tree_node* tree_root = PointerOnTreeRoot(differentiator);

    TREE_CHECK_AND_RETURN_ERRORS(TreeHTMLDump(differentiator, tree_root, DUMP_INFO, NOT_ERROR_DUMP));

    TREE_CHECK_AND_RETURN_ERRORS(TreeVerify(differentiator));

    return SUCCESS;
}

Tree_status CalculateValueOfExample(Differentiator* differentiator, Tree_node* tree_node) {
    assert(differentiator);

    for (size_t i = 0; i < differentiator->array_with_variables.size; ++i)
        TREE_CHECK_AND_RETURN_ERRORS(FillValueOfVariables(differentiator, i));

    TreeHTMLDump(differentiator, tree_node, DUMP_INFO, NOT_ERROR_DUMP);

    double result = Calculating(differentiator, tree_node);

    color_printf(COLOR_GREEN, " - Answer is: %lg\n", result);

    return SUCCESS;
}

Tree_status FillValueOfVariables(Differentiator* differentiator, size_t index_of_variable) {
    assert(differentiator);

    int cnt_attempts = CNT_ATTEMPTS;
    double value = DEFAULT_VALUE;

    while (cnt_attempts-- > 0) {
        About_variable about_variable = {};
        ArrayGetElement(&(differentiator->array_with_variables), &about_variable, index_of_variable);
        color_printf(COLOR_PURPLE, " - Enter the value of %s\n", about_variable.name);

        if (scanf("%lg%*c", &value) != 1) {
            scanf("%*[^\n]%*c");
            continue;
        }

        about_variable.value = value;
        ArrayChangeElement(&(differentiator->array_with_variables), &about_variable, index_of_variable);

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

#define CALCULATING_DIV                                                 \
{                                                                       \
    double second_num = Calculating(differentiator, tree_node->right_node); \
    if (fabs(second_num) < 1e-10)                                           \
        return 0.0;                                                         \
    return Calculating(differentiator, tree_node->left_node) / second_num;  \
}

#define CALCULATING_CTG                                             \
{                                                                   \
    double ans = Calculating(differentiator, tree_node->left_node); \
    double tg = tan(ans);                                           \
    if (fabs(tg) < 1e-10)                                           \
        return 0.0;                                                 \
    return 1.0 / tg;                                                \
}

#define CALCULATING_LN                                              \
{                                                                   \
    double ans = Calculating(differentiator, tree_node->left_node); \
    if (ans <= 0)                                                   \
        return 0.0;                                                 \
    return log(ans);                                                \
}

#define CALCULATING_LOG                                                     \
{                                                                           \
    double base = Calculating(differentiator, tree_node->left_node);        \
    double argument = Calculating(differentiator, tree_node->right_node);   \
                                                                            \
    if (base <= 0 || fabs(base - 1) < 1e-10)                                \
        return 0.0;                                                         \
    if (argument <= 0)                                                      \
        return 0.0;                                                         \
                                                                            \
    return log(argument) / log(base);                                       \
}

#define CALCULATING_ARCSIN                                          \
{                                                                   \
    double ans = Calculating(differentiator, tree_node->left_node); \
    if (ans < -1.0 || ans > 1.0)                                    \
        return 0.0;                                                 \
    return asin(ans);                                               \
}

#define CALCULATING_ARCCOS                                          \
{                                                                   \
    double ans = Calculating(differentiator, tree_node->left_node); \
    if (ans < -1.0 || ans > 1.0)                                    \
        return 0.0;                                                 \
    return acos(ans);                                               \
}

#define CALCULATING_ARCCTG                                          \
{                                                                   \
    double ans = Calculating(differentiator, tree_node->left_node); \
    return M_PI_2 - atan(ans);                                      \
}

#define CALCULATING_CTH                                             \
{                                                                   \
    double ans = Calculating(differentiator, tree_node->left_node); \
    if (fabs(sinh(ans)) < 1e-10)                                    \
        return 0.0;                                                 \
    return cosh(ans) / sinh(ans);                                   \
}

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
                    CALCULATING_DIV;
                case OPERATOR_POW:
                    return pow(Calculating(differentiator, tree_node->left_node), Calculating(differentiator, tree_node->right_node));
                case OPERATOR_SIN:
                    return sin(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_COS:
                    return cos(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_TG:
                    return tan(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_CTG: 
                    CALCULATING_CTG;
                case OPERATOR_LN: 
                    CALCULATING_LN;
                case OPERATOR_LOG:
                    CALCULATING_LOG;
                case OPERATOR_ARCSIN: 
                    CALCULATING_ARCSIN;
                case OPERATOR_ARCCOS: 
                    CALCULATING_ARCCOS;
                case OPERATOR_ARCTG:
                    return atan(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_ARCCTG: 
                    CALCULATING_ARCCTG;
                case OPERATOR_SH:
                    return sinh(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_CH:
                    return cosh(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_TH:
                    return tanh(Calculating(differentiator, tree_node->left_node));
                case OPERATOR_CTH: 
                    CALCULATING_CTH;
                case WRONG_OPERATOR:
                default: break;
            }
        case WRONG_TYPE: break;
        default: break;
    }

    return 0.0;
}


Tree_status DifferentiatorDtor(Differentiator* differentiator) {
    Tree_status status = TreeVerify(differentiator);

    free(differentiator->begin_buffer);
    
    for (size_t i = 0; i < differentiator->array_with_trees.size; ++i) {
        DifferentiatorNodeDtor(differentiator, PointerOnTreeRootFromIndex(differentiator, i));
    }

    ArrayDtorTrees(differentiator, &(differentiator->array_with_trees));
    free(differentiator->array_with_trees.data);

    ArrayDtorVariables(differentiator, &differentiator->array_with_variables);
    free(differentiator->array_with_variables.data);

    return status;
}

void DifferentiatorNodeDtor(Differentiator* differentiator, Tree_node* tree_node) {
    if (tree_node == NULL)
        return;

    DifferentiatorNodeDtor(differentiator, tree_node->left_node);

    DifferentiatorNodeDtor(differentiator, tree_node->right_node);

    free(tree_node);
}

void ArrayDtorVariables(Differentiator* differentiator, Array_with_data* array_with_data) {
    for (size_t i = 0; i < array_with_data->size; ++i) {
        free(NameOfVariableFromIndex(differentiator, i));
        // free(array_with_data->data[i]);
    }
}

void ArrayDtorTrees(Differentiator* differentiator, Array_with_data* array_with_data) {
    for (size_t i = 0; i < array_with_data->size; ++i) {
        free(PointerOnTreeFromIndex(differentiator, i));
        // free(array_with_trees->data[i]);
    }
}