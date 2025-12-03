#ifndef READ_EXPRESSION_H_
#define READ_EXPRESSION_H_

#include "common.h"


Tree_node* GetComandir(Differentiator* differentiator, Tree_status* status, char** str);

Tree_node* GetExpression(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetTerm(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetPow(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetPrimaryExpression(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetNumber(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetVariable(Differentiator* differentiator, char** str, Tree_status* status);

Tree_node* GetFunction(Differentiator* differentiator, char** str, Tree_status* status);


#endif // READ_EXPRESSION_H_