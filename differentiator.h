#ifndef DIFFERENTIATOR_H_
#define DIFFERENTIATOR_H_

#include "common.h"

#include "array.h"

#define ADD +
#define SUB -
#define MUL *
#define POW ^
#define DIV /


const int DEFAULT_VALUE = 666;


Tree_status DifferentiatorCtor(Differentiator* differentiator, const char* html_dump_filename, const char* tex_dump_filename, const char* directory);

Tree_status UploadTree(Differentiator* differentiator);

Tree_status ReadTree(Differentiator* differentiator, const char* file_with_tree);

Tree_status ReadNode(Differentiator* differentiator, Tree_node** tree_node, char** current_pos);

Type_node FindOutType(Differentiator* differentiator, char* buffer, type_t* value);

Status_of_finding ItIsOperator(char* buffer, type_t* value);

Status_of_finding ItIsNumber(char* buffer, type_t* value);

Status_of_finding ItIsVariable(Differentiator* differentiator, char* buffer, type_t* value);

void SkipSpaces(char** buffer);

Tree_status CalculateValueOfExample(Differentiator* differentiator, Tree_node* tree_node);

Tree_status FillValueOfVariables(Differentiator* differentiator, size_t index_of_variable);

double Calculating(Differentiator* differentiator, Tree_node* tree_node);

Tree_status DifferentiatorDtor(Differentiator* differentiator);

void DifferentiatorNodeDtor(Differentiator* differentiator, Tree_node* tree_node);


#endif // DIFFERENTIATOR_H_