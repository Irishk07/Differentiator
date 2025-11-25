#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "differentiation_functions.h"

#include "array.h"
#include "colors.h"
#include "common.h"
#include "tree.h"


Tree_status Differentiation(Differentiator* differentiator) {
    assert(differentiator);

    color_printf(COLOR_PURPLE, " - By which variable do you want to differentiate?\n");
    char* variable = ReadAnswer();

    Tree_node* old_tree_root = PointerOnTree(differentiator)->root;

    Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
    ArrayPushtrees(&differentiator->array_with_trees, new_tree);
     
    new_tree->root = DifferentiationFunctions(differentiator, old_tree_root, variable);

    free(variable);

    return SUCCESS;
}

#define dL DifferentiationFunctions(differentiator, tree_node->left_node, variable)

#define dR DifferentiationFunctions(differentiator, tree_node->right_node, variable)

#define cL CopyPartOfTree(differentiator, tree_node->left_node)

#define cR CopyPartOfTree(differentiator, tree_node->right_node)

#define NUMBER_NODE_CTOR(num) NodeCtor(tree, NUMBER, (type_t){.number = num}, NULL, NULL)

#define ADD_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ADD}, left, right)

#define SUB_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SUB}, left, right)

#define MUL_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_MUL}, left, right)

#define DIV_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_DIV}, left, right)

#define POW_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_POW}, left, right)

#define LN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_LN}, left, NULL)

#define LOG_BASE_(left, right) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_LOG}, left, right)

#define SIN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SIN}, left, NULL)

#define COS_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_COS}, left, NULL)

#define TAN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_TG}, left, NULL)

#define COT_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CTG}, left, NULL)

#define ASIN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCSIN}, left, NULL)

#define ACOS_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCCOS}, left, NULL)

#define ATAN_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCTG}, left, NULL)

#define ACOT_(left) NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_ARCCTG}, left, NULL)


Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    assert(differentiator);
    assert(variable);

    Tree* tree = PointerOnTree(differentiator);
    Tree_node* new_node = NULL;

    if (tree_node->type == NUMBER)
        new_node = NodeCtor(tree, NUMBER, (type_t){.number = 0}, tree_node->left_node, tree_node->right_node);
    
    if (tree_node->type == VARIABLE && strcmp(NameOfVariable(differentiator, tree_node), variable) != 0)
        new_node = NodeCtor(tree, NUMBER, (type_t){.number = 0}, tree_node->left_node, tree_node->right_node);

    if (tree_node->type == VARIABLE && strcmp(NameOfVariable(differentiator, tree_node), variable) == 0)
         new_node = NodeCtor(tree, NUMBER, (type_t){.number = 1}, tree_node->left_node, tree_node->right_node);

    if (tree_node->type == OPERATOR) {
        switch(tree_node->value.operators) {
            case OPERATOR_ADD:
                new_node = ADD_(dL, dR); break;

            case OPERATOR_SUB:
                new_node = SUB_(dL, dR); break;

            case OPERATOR_MUL:
                new_node = ADD_(MUL_(dL, cR), MUL_(cL, dR)); break;

            case OPERATOR_DIV:
                new_node = DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), POW_(cR, NUMBER_NODE_CTOR(2))); break;

            case OPERATOR_POW:
                {
                Tree_node* base_power = POW_(cL, cR);     
                Tree_node* ln_base = LN_(cL);                
                Tree_node* term1 = MUL_(dR, ln_base);       
                Tree_node* term2 = DIV_(MUL_(cR, dL), cL);  
                Tree_node* sum_terms = ADD_(term1, term2);  
                new_node = MUL_(base_power, sum_terms);
                break;
                }

            case OPERATOR_LN:
                new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), cL), dL); break;
                
            case OPERATOR_LOG:
                new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), MUL_(cR, LN_(cL))), dR); break;
                
            case OPERATOR_SIN:
                new_node = MUL_(COS_(cL), dL); break;
                
            case OPERATOR_COS:
                new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), SIN_(cL)), dL); break;
                
            case OPERATOR_TG:
                new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(COS_(cL), NUMBER_NODE_CTOR(2))), dL); break;
                
            case OPERATOR_CTG:
                new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SIN_(cL), NUMBER_NODE_CTOR(2)))), dL); break;
                
            case OPERATOR_ARCSIN:
                new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(SUB_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))), NUMBER_NODE_CTOR(0.5))), dL); break;
                
            case OPERATOR_ARCCOS:
                new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SUB_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))), NUMBER_NODE_CTOR(0.5)))), dL); break;
                
            case OPERATOR_ARCTG:
                new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), ADD_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2)))), dL); break;
                
            case OPERATOR_ARCCTG:
                new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), ADD_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))))), dL); break;

            case WRONG_OPERATOR:
            default: break;
        }
    }

    return new_node;
}

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node) {
    assert(differentiator);
    assert(old_node);

    Tree_node* new_node = NodeCtor(PointerOnTree(differentiator), old_node->type, old_node->value, old_node->left_node, old_node->right_node);

    new_node->need_dtor_childs = 0;

    PointerOnTree(differentiator)->size += TreeSize(old_node) - 1;

    return new_node;
}

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    if (tree_node == NULL) return FIND_NO;
    
    switch (tree_node->type) {
        case VARIABLE:
            if (strcmp(NameOfVariable(differentiator, tree_node), variable) == 0)
                return FIND_YES;
            return FIND_NO;
        case NUMBER:
        case OPERATOR:
        default:
            return FIND_NO;
    }

    return FIND_NO;
}