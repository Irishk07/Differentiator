#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "array.h"
#include "common.h"
#include "read_expression.h"
#include "tree.h"

// G ::= E'$'
// E ::= T{[+ -]T}*
// T ::= P{[* /]P}*
// P ::= '('E')' | N | V | F
// N ::= [0-9]+
// V ::= [a-z _][a-z 0-9 _]*
// F ::= [ln, sin, cos, tg, ctg, arcsin, arccos, arctg, arcctg, sh, ch, th, cth]'('E')'

Tree_node* GetComandir(Differentiator* differentiator, Tree_status* status, char** str) {
    assert(status);
    assert(str);

    Tree_node* new_node = GetExpression(differentiator, str, status);

    SkipSpaces(str);
    if (**str != '$') {
        *status = NOT_END_SYMBOL;
        return NULL;
    }

    return new_node;
}

Tree_node* GetExpression(Differentiator* differentiator, char** str, Tree_status* status) {
    assert(str);
    assert(*str);
    assert(status);

    Tree_node* new_node = GetTerm(differentiator, str, status);

    SkipSpaces(str);
    while (**str == '+' || **str == '-') {
        int cur_operator = (**str);
        (*str)++;

        Tree_node* new_node_2 = GetTerm(differentiator, str, status);

        if (cur_operator == '+')
            new_node = NodeCtor(PointerOnTree(differentiator), OPERATOR, (type_t){.operators = OPERATOR_ADD}, new_node, new_node_2);
        else 
            new_node = NodeCtor(PointerOnTree(differentiator), OPERATOR, (type_t){.operators = OPERATOR_SUB}, new_node, new_node_2);

        SkipSpaces(str);
    }

    return new_node;
}

Tree_node* GetTerm(Differentiator* differentiator, char** str, Tree_status* status) {
    assert(str);
    assert(*str);

    Tree_node* new_node = GetPrimaryExpression(differentiator, str, status);

    SkipSpaces(str);
    while (**str == '*' || **str == '/') {
        int cur_operator = (**str);
        (*str)++;

        Tree_node* new_node_2 = GetPrimaryExpression(differentiator, str, status);
        if (cur_operator == '*')
            new_node = NodeCtor(PointerOnTree(differentiator), OPERATOR, (type_t){.operators = OPERATOR_MUL}, new_node, new_node_2);
        else 
            new_node = NodeCtor(PointerOnTree(differentiator), OPERATOR, (type_t){.operators = OPERATOR_DIV}, new_node, new_node_2);
        
        SkipSpaces(str);
    }

    return new_node;
}

Tree_node* GetPrimaryExpression(Differentiator* differentiator, char** str, Tree_status* status) {
    assert(str);
    assert(*str);
    
    Tree_node* tree_node = NULL;

    SkipSpaces(str);
    if (**str == '(') {
        (*str)++;
        tree_node = GetExpression(differentiator, str, status);

        SkipSpaces(str);
        if (**str != ')')
            *status = NOT_END_SKOBKA;

        (*str)++;
    }

    else {
        tree_node = GetNumber(differentiator, str, status);
        if (tree_node != NULL)
            return tree_node;

        tree_node = GetVariable(differentiator, str, status);
        if (tree_node != NULL)
            return tree_node;

        tree_node = GetFunction(differentiator, str, status);
        if (tree_node != NULL)
            return tree_node;
    }

    return tree_node;
}

Tree_node* GetNumber(Differentiator* differentiator, char** str, Tree_status* status) {
    assert(str);
    assert(*str);

    char* old_str = *str;
    double val = 0;

    Tree_node* tree_node = NULL;

    SkipSpaces(str);
    while ('0' <= **str && **str <= '9') {
        val = val * 10 + (**str - '0');
        (*str)++;

        SkipSpaces(str);
    }

    if (old_str == *str)
        *status = NOT_NUMBER;

    else 
        tree_node = NodeCtor(PointerOnTree(differentiator), NUMBER, (type_t){.number = val}, NULL, NULL);

    return tree_node;
}

Tree_node* GetVariable(Differentiator* differentiator, char** str, Tree_status*) {
    assert(str);
    assert(*str);

    char* first_symbol = *str;

    Tree_node* tree_node = NULL;

    SkipSpaces(str);
    if (('a' <= **str && **str <= 'z') || **str == '_') {
        (*str)++;

        SkipSpaces(str);
        while (('a' <= **str && **str <= 'z') || **str == '_' || ('0' <= **str && **str <= '9'))
            (*str)++;

        char* name_variable = strndup(first_symbol, (size_t)(*str - first_symbol));

        for (size_t i = 0; i < sizeof(about_functions) / sizeof(about_functions[0]); ++i) {
            if (strcmp(name_variable, about_functions[i].name) == 0) {
                *str = first_symbol;
                free(name_variable);
                return NULL;
            }    
        }

        for (size_t i = 0; i < differentiator->array_with_variables.size; ++i) {
            if (strcmp(name_variable, differentiator->array_with_variables.data[i]->name) == 0) {
                free(name_variable);
                return NodeCtor(PointerOnTree(differentiator), VARIABLE, (type_t){.index_variable = (int)i}, NULL, NULL);
            }
        }
    
        About_variable* about_variable = (About_variable*)calloc(1, sizeof(About_variable));
        *about_variable = {.name = name_variable, .value = DEFAULT_VALUE};

        ArrayPushvariables(&differentiator->array_with_variables, about_variable);

        return NodeCtor(PointerOnTree(differentiator), VARIABLE, (type_t){.index_variable = (int)(differentiator->array_with_variables.size - 1)}, NULL, NULL);
    }

    return tree_node;
}

Tree_node* GetFunction(Differentiator* differentiator, char** str, Tree_status* status) {
    assert(differentiator);
    assert(str);

    for (size_t i = 0; i < sizeof(about_functions) / sizeof(about_functions[0]); ++i) {
        size_t len = strlen(about_functions[i].name);
        if (strncmp(about_functions[i].name, *str, len) == 0) {
            *str += len;

            SkipSpaces(str);
            if (**str == '(') {
                (*str)++;

                Tree_node* tree_node = GetExpression(differentiator, str, status);

                SkipSpaces(str);
                if (**str == ')') {
                    (*str)++;
                    return NodeCtor(PointerOnTree(differentiator), OPERATOR, (type_t){.operators = about_functions[i].type}, tree_node, NULL);
                }

                else {
                    *status = NOT_END_SKOBKA;
                }
            }
        }
    }

    return NULL;
}