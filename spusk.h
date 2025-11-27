#ifndef SPUSK_H_
#define SPUSK_H_


Tree_node* GetComandir(Tree_status* status, const char* file_name);

Tree_node* GetExpression(char** str, Tree_status* status);

Tree_node* GetTerm(char** str, Tree_status* status);

Tree_node* GetP(char** str, Tree_status* status);

Tree_node* GetNumber(char** str, Tree_status* status);

Tree_node* SpuskNodeCtor(Type_node type, type_t value,
                    Tree_node* left_node, Tree_node* right_node);

int SpuskSizeOfText(const char *text_name);

Tree_status SpuskReadOnegin(char** str, const char* name_file);


#endif // SPUSK_H_