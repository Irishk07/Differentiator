#ifndef READ_EXPRESSION_H_
#define READ_EXPRESSION_H_

#include "common.h"


Tree_node* GetComandir(Differentiator* differentiator, Tree_status* status, char** str);

Tree_node* GetExpression(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetTerm(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetPrimaryExpression(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetNumber(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetVariable(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetFunction(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* SpuskNodeCtor(Type_node type, type_t value,
                    Tree_node* left_node, Tree_node* right_node);


#endif // READ_EXPRESSION_H_