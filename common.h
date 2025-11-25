#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>

#define TREE_CHECK_AND_RETURN_ERRORS(error, ...)                          \
        {                                                                 \
            Tree_status now_error = (error);                              \
            if (now_error != SUCCESS) {                                   \
                fprintf(stderr, "Error is: %d, %d\n", (error), __LINE__); \
                __VA_ARGS__;                                              \
                return now_error;                                         \
            }                                                             \
        }


enum Type_node {
    OPERATOR_ADD   = 0,
    OPERATOR_SUB   = 1,
    OPERATOR_MUL   = 2,
    OPERATOR_DIV   = 3,
    OPERATOR_POW   = 4,
    VARIABLE       = 5,
    NUMBER         = 6,
    WRONG_TYPE     = 7
};

union type_t {
    double    number;
    int       index_variable;
    Type_node operators;
};

enum Tree_status {
    SUCCESS                  = 0,
    MEMORY_ERROR             = 1,
    OPEN_ERROR               = 2,
    EXECUTION_FAILED         = 3,
    CLOSE_ERROR              = 4,
    WRONG_SITUATION          = 5,
    READ_ERROR               = 6,
    NULL_POINTER_ON_TREE     = 7,
    WRONG_SIZE               = 8,
    NULL_POINTER_ON_NODE     = 9,
    PARENT_AND_CHILD_UNEQUAL = 10,
    WRONG_ROOT               = 11,
    WRONG_NODE               = 12,
    NULL_POINTER_ON_LEAF     = 13,
    SYNTAX_ERROR             = 14,
    BUFFER_OVERFLOW          = 15,
    STAT_ERROR               = 16,
};

enum Status_of_finding {
    FIND_YES = 0,
    FIND_NO  = 1
};

enum Prioritets {
    NO_PRIOTITET     = 0,
    LOW_PRIOTITET    = 1,
    MIDDLE_PRIORITET = 2,
    HIGH_PRIORITET   = 3
};


struct Tree_node {
    Type_node type;
    type_t value;
    Tree_node* left_node;
    Tree_node* right_node;
    Tree_node* parent;
};

struct Dump_information {
    const char* html_dump_filename;
    const char* tex_dump_filename;
    const char* directory;
    int num_dump = 0;
};

struct Tree {
    Tree_node* root;
    size_t size;
};

typedef Tree* trees_type_t;

struct About_variable {
    char* name;
    double value;
};

typedef About_variable* variables_type_t;

struct Array_with_variables {
    variables_type_t* data = NULL;
    size_t size = 0;
    size_t capacity = 0;
};

struct Array_with_trees {
    trees_type_t* data = NULL;
    size_t size = 0;
    size_t capacity = 0;
};

struct Differentiator {
    Tree tree;
    char* begin_buffer;
    char* end_buffer;
    size_t size_buffer;
    char* buffer_with_tree;
    Array_with_variables array_with_variables;
    Dump_information dump_info;
};


#endif // COMMON_H_