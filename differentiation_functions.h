#ifndef DIFFRENTIATION_FUNCTIONS_H_
#define DIFFRENTIATION_FUNCTIONS_H_

#include "common.h"


Tree_status Differentiation(Differentiator* differentiator);

Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node);

void PrintfConnectingPhrases(Differentiator* differentiator);

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

int ReadNumberDifferentiation();

void OptimizationTree(Differentiator* differentiator, Tree_node** old_node, const char* variable);

void OptimizationNode(Differentiator* differentiator, Tree_node** old_node, const char* variable);

void OptimizationZeroNode(Differentiator* differentiator, Tree* tree, Tree_node** old_node);

void OptimizationOneNode(Differentiator* differentiator, Tree* tree, Tree_node** old_node);

bool IsConstantNode(Differentiator* differentiator, Tree_node* node, const char* variable);

bool IsZeroNode(Tree_node* node);

bool IsOneNode(Tree_node* node);


#endif // DIFFRENTIATION_FUNCTIONS_H_