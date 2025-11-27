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


const int CNT_ATTEMPTS  = 5;


enum Type_node {
    VARIABLE    = 0,
    NUMBER      = 1,
    OPERATOR    = 2,
    WRONG_TYPE  = 3
};

enum Type_operators {
    OPERATOR_ADD    = 0,
    OPERATOR_SUB    = 1,
    OPERATOR_MUL    = 2,
    OPERATOR_DIV    = 3,
    OPERATOR_POW    = 4,
    OPERATOR_LN     = 5,
    OPERATOR_LOG    = 6,
    OPERATOR_SIN    = 7,
    OPERATOR_COS    = 8,
    OPERATOR_TG     = 9,
    OPERATOR_CTG    = 10,
    OPERATOR_ARCSIN = 11,
    OPERATOR_ARCCOS = 12,
    OPERATOR_ARCTG  = 13,
    OPERATOR_ARCCTG = 14,
    OPERATOR_SH     = 15,  
    OPERATOR_CH     = 16,  
    OPERATOR_TH     = 17,  
    OPERATOR_CTH    = 18, 
    WRONG_OPERATOR
};

union type_t {
    double number;
    int    index_variable;
    Type_operators operators;
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
    INPUT_ERROR              = 17
};

enum Status_of_finding {
    FIND_NO  = 0,
    FIND_YES = 1
};

enum Prioritets {
    NO_PRIORITET      = 0,
    LOW_PRIOTITET     = 1,
    MIDDLE_PRIORITET  = 2,
    HIGH_PRIORITET    = 3,
    HIGHEST_PRIORITET = 4
};


struct Tree_node {
    Type_node type;
    type_t value;
    Tree_node* left_node;
    Tree_node* right_node;
};

struct Dump_information {
    const char* html_dump_filename;
    const char* tex_dump_filename;
    const char* directory;
    int num_html_dump = 0;
    FILE* tex_dump_file;
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
    char* begin_buffer;
    char* end_buffer;
    size_t size_buffer;
    char* buffer_with_tree;
    Array_with_variables array_with_variables;
    Array_with_trees array_with_trees;
    Dump_information dump_info;
};


#endif // COMMON_H_