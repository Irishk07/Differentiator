#ifndef DIFFRENTIATION_FUNCTIONS_H_
#define DIFFRENTIATION_FUNCTIONS_H_

#include "common.h"


void StartDifferntiator(Differentiator* differentiator);

Tree_status Differentiation(Differentiator* differentiator, char* variable);

Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node);

void PrintfConnectingPhrases(Differentiator* differentiator);

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

int ReadIntNumber(const char* string);

double ReadDoubleNumber(const char* string);

void OptimizationTree(Differentiator* differentiator, Tree_node** old_node, const char* variable);

void OptimizationNode(Differentiator* differentiator, Tree_node** old_node, const char* variable);

void OptimizationZeroNode(Differentiator* differentiator, Tree* tree, Tree_node** old_node);

void OptimizationOneNode(Differentiator* differentiator, Tree_node** old_node);

bool IsConstantNode(Differentiator* differentiator, Tree_node* node, const char* variable);

bool IsZeroNode(Tree_node* node);

bool IsOneNode(Tree_node* node);

void CalculateValueAllFunctions(Differentiator* differentiator, char* variable, double value);

Tree_status CalculateValueOfFunction(Differentiator* differentiator, Tree_node* tree_node, char* variable, double value, size_t number);

Tree_status FormulaTaylora(Differentiator* differentiator, char* variable, double value);

size_t Factorial(size_t n);


#endif // DIFFRENTIATION_FUNCTIONS_H_