#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

#include "colors.h"
#include "common.h"
#include "debug.h"
#include "onegin.h"


Tree_status TreeCtor(Tree* tree, const char* html_dump_filename, const char* directory) {
    assert(tree);
    assert(html_dump_filename);
    assert(directory);

    tree->size = 0;

    return SUCCESS;
}

Tree_status NodeCtor(Tree* tree, Tree_node** new_node) {
    assert(tree);
    assert(new_node);

    *new_node = (Tree_node*)calloc(1, sizeof(Tree_node));
    if (*new_node == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR);

    tree->size++;

    return SUCCESS;
}


Tree_status TreeVerify(Differentiator* differentiator, Tree* tree) {
    if (tree == NULL)
        TREE_DUMP_AND_RETURN_ERRORS(NULL_POINTER_ON_TREE);

    if (tree->size != TreeSize(tree->root))
        TREE_DUMP_AND_RETURN_ERRORS(WRONG_SIZE);

    TREE_DUMP_AND_RETURN_ERRORS(AllNodesVerify(tree, tree->root));

    return SUCCESS;
}

Tree_status AllNodesVerify(Tree* tree, Tree_node* tree_node) {
    if (tree_node == NULL)
        return SUCCESS;

    TREE_CHECK_AND_RETURN_ERRORS(OneNodeVerify(tree, tree_node));

    TREE_CHECK_AND_RETURN_ERRORS(AllNodesVerify(tree, tree_node->left_node));

    TREE_CHECK_AND_RETURN_ERRORS(AllNodesVerify(tree, tree_node->right_node));

    return SUCCESS;
}

Tree_status OneNodeVerify(Tree* tree, Tree_node* tree_node) {
    if (tree_node == NULL)            TREE_CHECK_AND_RETURN_ERRORS(NULL_POINTER_ON_NODE);

    if (tree_node == tree->root && 
        tree_node->parent != NULL)    TREE_CHECK_AND_RETURN_ERRORS(WRONG_ROOT);

    if (tree_node != tree->root &&
        tree_node->parent == NULL)    TREE_CHECK_AND_RETURN_ERRORS(WRONG_NODE);

    if (tree_node != tree->root) {
        if (tree_node->parent->left_node != tree_node &&
            tree_node->parent->right_node != tree_node) TREE_CHECK_AND_RETURN_ERRORS(PARENT_AND_CHILD_UNEQUAL);
    }
    return SUCCESS;
}

size_t TreeSize(Tree_node* tree_node) {
    if (tree_node == NULL) {
        return 0;
    }
    
    return 1 + TreeSize(tree_node->left_node) + TreeSize(tree_node->right_node);
}

char* ReadAnswer() {
    char* answer = NULL;
    size_t size_buf = 0;

    if (getline(&answer, &size_buf, stdin) == -1)
        return NULL;

    answer[strlen(answer) - 1] = '\0';

    return answer;
}

size_t IndexOfVariable(Tree_node* tree_node) {
    return (size_t)tree_node->value.index_variable;
}

double ValueOfVariable(Differentiator* differentiator, Tree_node* tree_node) {
    return (differentiator->array_with_variables.data)[IndexOfVariable(tree_node)]->value;
}

char* NameOfVariable(Differentiator* differentiator, Tree_node* tree_node) {
    return (differentiator->array_with_variables.data)[IndexOfVariable(tree_node)]->name;
}

const char* IndetifySign(Type_node type_node) {
    switch(type_node) {
        case OPERATOR_ADD: return "+";
        case OPERATOR_SUB: return "-";
        case OPERATOR_MUL: return "*";
        case OPERATOR_DIV: return "\\";
        case OPERATOR_POW: return "^";
        case NUMBER:
        case VARIABLE:
        case WRONG_TYPE:
        default: break;
    }

    return NULL;
}


Tree_status TreeHTMLDump(Differentiator* differentiator, Tree* tree, Tree_node* tree_node, int line, const char* file, Type_dump type_dump, Tree_status tree_status) {
    FILE* html_dump_file = NULL;
    if (differentiator->dump_info.num_dump == 0)
        html_dump_file = fopen(differentiator->dump_info.html_dump_filename, "w");
    else
        html_dump_file = fopen(differentiator->dump_info.html_dump_filename, "a");
    if (html_dump_file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    fprintf(html_dump_file, "<pre> <font size = \"8\">\n");

    if (type_dump == USUAL_DUMP)
        fprintf(html_dump_file, "<h3> DUMP <font color = green> Tree </font> </h3>\n");
    
    if (type_dump == ERROR_DUMP) {
        fprintf(html_dump_file, "<h3> ERROR_ANSWER DUMP <font color = red> Tree </font> </h3>\n");

        PrintErrors(tree_status, html_dump_file);

        if (tree_status == NULL_POINTER_ON_TREE ||
            tree_status == NULL_POINTER_ON_NODE)
            return tree_status;
    }

    fprintf(html_dump_file, "Tree {%s: %d}\n", file, line);

    fprintf(html_dump_file, "Size: %zu\n", tree->size);

    fprintf(html_dump_file, "Root: %p\n", tree->root);

    TREE_CHECK_AND_RETURN_ERRORS(GenerateGraph(differentiator, tree, tree_node));

    char command[MAX_LEN_NAME] = {};
    snprintf(command, MAX_LEN_NAME, "dot %s/graphes/graph%d.txt -T png -o %s/images/image%d.png", 
                                     differentiator->dump_info.directory, differentiator->dump_info.num_dump, 
                                     differentiator->dump_info.directory, differentiator->dump_info.num_dump);
    
    if (system((const char*)command) != 0)
        TREE_CHECK_AND_RETURN_ERRORS(EXECUTION_FAILED,      fprintf(html_dump_file, "Error with create image:(\n"));

    fprintf(html_dump_file, "\n");
    fprintf(html_dump_file, "<img src = %s/images/image%d.png width = 700px>", differentiator->dump_info.directory, differentiator->dump_info.num_dump);

    fprintf(html_dump_file, "\n\n");

    differentiator->dump_info.num_dump++;

    if (fclose(html_dump_file) == EOF)
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    return SUCCESS;
}

Tree_status GenerateGraph(Differentiator* differentiator, Tree* tree, Tree_node* tree_node) {
    char filename_graph[MAX_LEN_NAME] = {};
    snprintf(filename_graph, MAX_LEN_NAME, "%s/graphes/graph%d.txt", differentiator->dump_info.directory, differentiator->dump_info.num_dump);

    FILE* graph = fopen(filename_graph, "w");
    if (graph == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    fprintf(graph, "digraph {\n");
    fprintf(graph, "    node [shape = Mrecord, style = filled, fillcolor = \"#99FF99\"];\n");

    if (tree_node != NULL) {
        PrintNodeToDot(differentiator, tree, graph, tree_node);
    } 
    else {
        fprintf(graph, "There will be no tree\n");
    }

    fprintf(graph, "}\n");

    if (fclose(graph) == EOF)
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,    perror("Error is:"));

    return SUCCESS;
}

void PrintNodeToDot(Differentiator* differentiator, Tree* tree, FILE *file, Tree_node* tree_node) {
    assert(file);
    assert(tree_node);

    if (tree_node->type == NUMBER) {
        fprintf(file, "    node_%p [label=\"{%p | type = Number | value = %lg | parent = %p | left = %p | right = %p}\"];\n", 
                      (void *)tree_node, tree_node, tree_node->value.number, tree_node->parent, tree_node->left_node, tree_node->right_node);
    }    

    else if (tree_node->type == VARIABLE) {
        fprintf(file, "    node_%p [label=\"{%p | type = Variable | value = %s (%lg) | parent = %p | left = %p | right = %p}\"];\n", 
                      (void *)tree_node, tree_node, NameOfVariable(differentiator, tree_node), ValueOfVariable(differentiator, tree_node), 
                      tree_node->parent, tree_node->left_node, tree_node->right_node);
    }

    else if (tree_node->type != WRONG_TYPE) {
        fprintf(file, "    node_%p [label=\"{%p | type = Operator | value = %s | parent = %p | left = %p | right = %p}\"];\n", 
                      (void *)tree_node, tree_node, IndetifySign(tree_node->type), tree_node->parent, tree_node->left_node, tree_node->right_node);
    }  


    if (tree_node->left_node) {
        fprintf(file, "    node_%p -> node_%p [label = \"Yes\", color = \"#FF6600\"];\n ", (void *)tree_node, (void *)tree_node->left_node);
        PrintNodeToDot(differentiator, tree, file, tree_node->left_node);
    }

    if (tree_node->right_node) {
        fprintf(file, "    node_%p -> node_%p [label = \"No\", color = \"#0000CC\"];\n", (void *)tree_node, (void *)tree_node->right_node);
        PrintNodeToDot(differentiator, tree, file, tree_node->right_node);
    }
}

void PrintErrors(int error, FILE* stream) {
    if (error == SUCCESS                  ) fprintf(stream, "ALL_RIGHT\n");
    if (error == NULL_POINTER_ON_TREE     ) fprintf(stream, "Null pointer on tree\n");
    if (error == NULL_POINTER_ON_NODE     ) fprintf(stream, "Null pointer on node\n");
    if (error == MEMORY_ERROR             ) fprintf(stream, "Not enough memory\n");
    if (error == OPEN_ERROR               ) fprintf(stream, "Open error\n");
    if (error == CLOSE_ERROR              ) fprintf(stream, "Close error\n");
    if (error == EXECUTION_FAILED         ) fprintf(stream, "Execution failed\n");
    if (error == WRONG_SITUATION          ) fprintf(stream, "Left node is NULL, but right node not OR right node is NULL, but left node not\n");
    if (error == READ_ERROR               ) fprintf(stream, "Scanf can't read user's answers\n");
    if (error == WRONG_SIZE               ) fprintf(stream, "Size of tree is wrong\n");
    if (error == PARENT_AND_CHILD_UNEQUAL ) fprintf(stream, "Parent's chils is unequal with current node\n");
    if (error == WRONG_ROOT               ) fprintf(stream, "Node is root, but its parent isn't NULL\n");
    if (error == WRONG_NODE               ) fprintf(stream, "Node isn't root, but its parent is NULL\n");
}