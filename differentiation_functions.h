#ifndef DIFFRENTIATION_FUNCTIONS_H_
#define DIFFRENTIATION_FUNCTIONS_H_

#include "common.h"


Tree_status Differentiation(Differentiator* differentiator);

Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node);

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable);


#endif // DIFFRENTIATION_FUNCTIONS_H_