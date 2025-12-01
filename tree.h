#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>

#include "common.h"


#define TREE_DUMP_AND_RETURN_ERRORS(error, ...)                                                  \
        {                                                                                        \
            Tree_status now_error = error;                                                       \
            if (now_error != SUCCESS) {                                                          \
                TreeHTMLDump(differentiator, tree->root, DUMP_INFO, ERROR_DUMP, now_error);      \
                return now_error;                                                                \
            }                                                                                    \
        }

#define DUMP_INFO __LINE__, __FILE__
#define NOT_ERROR_DUMP USUAL_DUMP, SUCCESS


const int MAX_LEN_NAME = 256;
const int LEN_NIL      = 3;


enum Type_dump {
    USUAL_DUMP = 0,
    ERROR_DUMP = 1
};


Tree_status TreeCtor(Tree* tree, const char* html_dump_filename, const char* directory);

Tree_node* NodeCtor(Tree* tree, Type_node type, type_t value,
                    Tree_node* left_node, Tree_node* right_node);

Tree_status TreeVerify(Differentiator* differentiator);

Tree_status AllNodesVerify(Tree* tree, Tree_node* tree_node);

size_t TreeSize(Tree_node* tree_node);

char* ReadAnswer();

double ValueOfVariable(Differentiator* differentiator, Tree_node* tree_node);

char* NameOfVariable(Differentiator* differentiator, Tree_node* tree_node);

size_t IndexOfVariable(Tree_node* tree_node);

Tree* PointerOnTree(Differentiator* differentiator);

Tree_node* PointerOnTreeRoot(Differentiator* differentiator);

Tree_node* PointerOnTreeRootFromIndex(Differentiator* differentiator, size_t index);

double ValueOfTree(Differentiator* differentiator);

double ValueOfTreeFromIndex(Differentiator* differentiator, size_t index);

const char* IndetifySign(Tree_node* tree_node);

Tree_status TreeHTMLDump(Differentiator* differentiator, Tree_node* tree_node, int line, const char* file, Type_dump type_dump, Tree_status tree_status);

Tree_status GenerateGraph(Differentiator* differentiator, Tree_node* tree_node);

void PrintNodeToDot(Differentiator* differentiator, FILE *file, Tree_node* tree_node);

void PrintErrors(int error, FILE* stream);

Tree_status CreateTexFileForDump(Differentiator* differentiator);

Tree_status TreeTexDump(Differentiator* differentiator, Tree_node* old_tree_root, Tree_node* new_tree_root, char* variable);

void PrintExpressionToTex(Differentiator* differentiator, Tree_node* tree_node, FILE* tex_dump_file, Prioritets parent_prioritet, int depth);

Prioritets GetPrioritet(Tree_node* tree_node);

Tree_status ClostTexFileForDump(Differentiator* differentiator);


#endif // TREE_H_