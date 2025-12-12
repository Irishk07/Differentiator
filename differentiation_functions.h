#ifndef DIFFRENTIATION_FUNCTIONS_H_
#define DIFFRENTIATION_FUNCTIONS_H_

#include "common.h"


const double EPS = 1e-10;

const double X_START = -0.5;
const double X_END   = 1.5;
const double Y_START = -0.5;
const double Y_END   =  1.5;
const double STEP  = 1e-5;


void StartDifferntiator(Differentiator* differentiator);

Tree_status Differentiation(Differentiator* differentiator, char* variable, int number);

Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

Tree_node* DoOperatorPow(Differentiator* differentiator, Tree* tree, Tree_node* tree_node, const char* variable);

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node);

void PrintfConnectingPhrases(Differentiator* differentiator);

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable);

int ReadIntNumber(const char* string);

double ReadDoubleNumber(const char* string);

void OptimizationTree(Differentiator* differentiator, Tree_node** old_node, const char* variable);

void OptimizationNode(Differentiator* differentiator, Tree_node** old_node, const char* variable);

void OptimizationZeroNode(Differentiator* differentiator, Tree* tree, Tree_node** tree_node, Tree_node* first_child, Tree_node* second_child);

void OptimizationOneNode(Differentiator* differentiator, Tree_node** tree_node, Tree_node* first_child, Tree_node* second_child);

bool IsConstantNode(Differentiator* differentiator, Tree_node* node, const char* variable);

bool IsZeroNode(Tree_node* node);

bool IsOneNode(Tree_node* node);

void CalculateValueAllFunctions(Differentiator* differentiator, char* variable, double value);

double CalculateValueOfFunction(Differentiator* differentiator, Tree_node* tree_node, char* variable, double value, size_t number);

Tree_status CreateTaylorsTree(Differentiator* differentiator, char* varible, double value);

Tree_node* CreateTaylorNode(Tree* tree, double exp, double dif_value, double factorial, Tree_node* x_minus_x0);

Tree_node* CopyNode(Tree_node* tree_node);

Tree_status FormulaTaylora(Differentiator* differentiator, char* variable, double value);

size_t Factorial(size_t n);

Tree_status GNUPlot(Differentiator* differentiator);

Tree_status AnalyzeInterval(char** buffer, Intervals* intervals);

void GetStartEndNum(char** buffer, double* start, double* end);

Tree_status CreateFileWithData(Differentiator* differentiator, Tree_node* tree_node, const char* data_file_name, Intervals* intervals);

Tree_status CreateFileWithTangency(Differentiator* differentiator, const char* data_file_tangency_name, double point_tangency, Intervals* intervals);

Tree_status CreateGNUPlotFile(const char* GNUPlot_file, const char* data_file, const char* image);

Tree_status CreateGNUPlotFileThreeFunction(const char* GNUPlot_file, 
                                           const char* data_file, const char* data_2_file, const char* data_file_tangency_name, 
                                           const char* image,
                                           Intervals* intervals);

#endif // DIFFRENTIATION_FUNCTIONS_H_