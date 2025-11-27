#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "common.h"
#include "spusk.h"
#include "tree.h"

// TODO написать в текстовый файл грамматику

Tree_node* GetComandir(Tree_status* status, const char* file_name) {
    assert(status);
    assert(file_name);

    char* str = NULL;
    SpuskReadOnegin(&str, file_name);
    char* temp = str;

    Tree_node* new_node = GetExpression(&str, status);

    if (*str != '$') {
        *status = NOT_END_SYMBOL;
        return NULL;
    }

    free(temp);

    return new_node;
}

Tree_node* GetExpression(char** str, Tree_status* status) {
    assert(str);
    assert(*str);
    assert(status);

    Tree_node* new_node = GetTerm(str, status);

    while (**str == '+' || **str == '-') {
        int cur_operator = (**str);
        (*str)++;

        Tree_node* new_node_2 = GetTerm(str, status);

        if (cur_operator == '+')
            return SpuskNodeCtor(OPERATOR, (type_t){.operators = OPERATOR_ADD}, new_node, new_node_2);
        else 
            return SpuskNodeCtor(OPERATOR, (type_t){.operators = OPERATOR_SUB}, new_node, new_node_2);

        new_node = new_node_2;
    }

    return NULL;
}

Tree_node* GetTerm(char** str, Tree_status* status) {
    assert(str);
    assert(*str);

    Tree_node* new_node = GetP(str, status);

    fprintf(stderr, "%lg\n", new_node->value.number);

    while (**str == '*' || **str == '/') {
        int cur_operator = (**str);
        (*str)++;

        Tree_node* new_node_2 = GetP(str, status);
        if (cur_operator == '*')
            return SpuskNodeCtor(OPERATOR, (type_t){.operators = OPERATOR_MUL}, new_node, new_node_2);
        else 
            return SpuskNodeCtor(OPERATOR, (type_t){.operators = OPERATOR_DIV}, new_node, new_node_2);
    }

    return new_node;
}

Tree_node* GetP(char** str, Tree_status* status) {
    assert(str);
    assert(*str);
    
    Tree_node* tree_node = NULL;

    if (**str == '(') {
        (*str)++;
        tree_node = GetExpression(str, status);

        if (**str != ')')
            *status = NOT_END_SKOBKA;

        (*str)++;
    }

    else {
        tree_node = GetNumber(str, status);
        fprintf(stderr, "%lg\n", tree_node->value.number);
    }

    return tree_node;
}

Tree_node* GetNumber(char** str, Tree_status* status) {
    assert(str);
    assert(*str);

    char* old_str = *str;
    double val = 0;

    Tree_node* tree_node = NULL;

    while ('0' <= **str && **str <= '9') {
        val = val * 10 + (**str - '0');
        (*str)++;
    }

    if (old_str == *str)
        *status = NOT_NUMBER;

    else 
        tree_node = SpuskNodeCtor(NUMBER, (type_t){.number = val}, NULL, NULL);

    return tree_node;
}


Tree_node* SpuskNodeCtor(Type_node type, type_t value,
                    Tree_node* left_node, Tree_node* right_node) {

    Tree_node* new_node = (Tree_node*)calloc(1, sizeof(Tree_node));
    if (new_node == NULL)
        return NULL;

    new_node->left_node  = left_node;
    new_node->right_node = right_node;
    new_node->type       = type;
    new_node->value      = value;

    return new_node;
}

int SpuskSizeOfText(const char *text_name) {
    assert(text_name);

    struct stat text_info = {};

    if (stat(text_name, &text_info) == -1) {
        perror("Error is");

        return -1;
    }

    return (int)text_info.st_size;
}

Tree_status SpuskReadOnegin(char** str, const char* name_file) {
    assert(name_file != NULL);
    assert(str);    

    FILE *text = fopen(name_file, "r");
    if (text == NULL) {
        perror("Error is");
        return OPEN_ERROR;
    }

    int size = SpuskSizeOfText(name_file);
    if (size == -1) {
        return STAT_ERROR;
    }

    *str = (char*)calloc((size_t)size, sizeof(char));
    if (str == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR);

    fread((char *)*str, sizeof(char), (size_t)size, text);
    if (ferror(text) != 0) {
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR,    free(*str));
    }

    return SUCCESS;
}