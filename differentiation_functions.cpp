#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "differentiation_functions.h"

#include "array.h"
#include "colors.h"
#include "common.h"
#include "differentiator.h"
#include "tree.h"


Tree_status Differentiation(Differentiator* differentiator) {
    assert(differentiator);

    color_printf(COLOR_PURPLE, " - By which variable do you want to differentiate?\n");
    char* variable = ReadAnswer();

    int number = ReadNumberDifferentiation();
    if (number == -1)
        return INPUT_ERROR;

    CreateTexFileForDump(differentiator);

    while (number-- > 0){

        Tree_node* old_tree_root = PointerOnTree(differentiator)->root;

        Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
        if (new_tree == NULL)
            TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR,      free(variable););

        TREE_CHECK_AND_RETURN_ERRORS(ArrayPushtrees(&differentiator->array_with_trees, new_tree),   free(variable););

        fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
        
        new_tree->root = DifferentiationFunctions(differentiator, old_tree_root, variable);

        fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");
        (differentiator->dump_info.tex_dump_file, "\\vspace{10pt}\n\n");

        TreeHTMLDump(differentiator, PointerOnTree(differentiator)->root, DUMP_INFO, NOT_ERROR_DUMP);

        // TreeTexDump(differentiator, old_tree_root, new_tree->root, variable);

        OptimizationTree(differentiator, &new_tree->root, variable);

        TreeTexDump(differentiator, old_tree_root, new_tree->root, variable);
    }

    ClostTexFileForDump(differentiator);

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
#define SH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_SH}, left, NULL)
#define CH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CH}, left, NULL)
#define TH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_TH}, left, NULL)
#define CTH_(left)  NodeCtor(tree, OPERATOR, (type_t){.operators = OPERATOR_CTH}, left, NULL)

#define TEX_ADD                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);                                        \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") + \\frac{d}{d%s} ( ", variable);                                    \
PrintExpressionToTex(differentiator, tree_node->right_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);  \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_SUB                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);                                        \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") - \\frac{d}{d%s} ( ", variable);                                    \
PrintExpressionToTex(differentiator, tree_node->right_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);  \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_MUL                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s}(", variable);                                          \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot (");                                                         \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") + (");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);  \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_DIV                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{ \\frac{d}{d%s}(", variable);                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot (");                                                         \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") - (");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") }{ (");                                                             \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ")^2 }\\\\\n");                                                        

#define TEX_POW_X_N                                                                                                                                 \
fprintf(differentiator->dump_info.tex_dump_file, "%g \\cdot (", Calculating(differentiator, tree_node->right_node));                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);                               \
fprintf(differentiator->dump_info.tex_dump_file, ")^{%g} \\cdot \\frac{d}{d%s}(", Calculating(differentiator, tree_node->right_node) - 1, variable);\
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);                               \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_POW_X_X                                                                                                     \
fprintf(differentiator->dump_info.tex_dump_file, "(");                                                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^{");                                                                \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, "} \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\ln(");                                                     \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") + \\frac{");                                                        \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, " \\cdot \\frac{d}{d%s}(", variable);                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")}{");                                                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, "}\\\\\n");  

#define TEX_LN                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{");                                                         \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, "} \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_LOG                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{");                                                         \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, " \\cdot \\ln(");                                                      \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") } \\cdot \\frac{d}{d%s}(", variable);                               \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_SIN                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\cos(");                                                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_COS                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "-\\sin(");                                                            \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_TG                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{\\cos^2(");                                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_CTG                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{\\sin^2(");                                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_ARCSIN                                                                                                      \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{\\sqrt{1 - (");                                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2}} \\cdot \\frac{d}{d%s}(", variable);                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_ARCCOS                                                                                                      \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{\\sqrt{1 - (");                                            \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2}} \\cdot \\frac{d}{d%s}(", variable);                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\n");

#define TEX_ARCTG                                                                                                       \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{1 + (");                                                    \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2} \\cdot \\frac{d}{d%s}(", variable);                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_ARCCTG                                                                                                      \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{1 + (");                                                   \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2} \\cdot \\frac{d}{d%s}(", variable);                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_SH                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\ch(");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_CH                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\sh(");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_TH                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{\\ch^2(");                                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_CTH                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{\\sh^2(");                                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");


Tree_node* DifferentiationFunctions(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    assert(differentiator);
    assert(tree_node);
    assert(variable);

    Tree* tree = PointerOnTree(differentiator);
    Tree_node* new_node = NULL;

    // fprintf(differentiator->dump_info.tex_dump_file, "\\subsection{}\n");
    
    fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);
    PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ") = ");

    if (tree_node->type == VARIABLE) {
        int result_compare = strcmp(NameOfVariable(differentiator, tree_node), variable);

        if (result_compare != 0)
            new_node = NodeCtor(tree, NUMBER, (type_t){.number = 0}, tree_node->left_node, tree_node->right_node);
        
        else 
            new_node = NodeCtor(tree, NUMBER, (type_t){.number = 1}, tree_node->left_node, tree_node->right_node);
    }        

    if (tree_node->type == NUMBER)
        new_node = NodeCtor(tree, NUMBER, (type_t){.number = 0}, tree_node->left_node, tree_node->right_node);

    if (tree_node->type == OPERATOR) {
        switch(tree_node->value.operators) {
            case OPERATOR_ADD:
                TEX_ADD; new_node = ADD_(dL, dR); break;

            case OPERATOR_SUB:
                TEX_SUB; new_node = SUB_(dL, dR); break;

            case OPERATOR_MUL:
                TEX_MUL; new_node = ADD_(MUL_(dL, cR), MUL_(cL, dR)); break;

            case OPERATOR_DIV:
                TEX_DIV; new_node = DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), POW_(cR, NUMBER_NODE_CTOR(2))); break;

            case OPERATOR_POW:
                {
                Status_of_finding find_in_left_node = ContainsVariable(differentiator, tree_node->left_node, variable);
                Status_of_finding find_in_right_node = ContainsVariable(differentiator, tree_node->right_node, variable);
                if (find_in_left_node && !find_in_right_node) {
                    TEX_POW_X_N; new_node = MUL_(cR, MUL_(POW_(cL, SUB_(cR, NUMBER_NODE_CTOR(1))), dL));
                }
                else {
                    TEX_POW_X_X; new_node = MUL_(POW_(cL, cR), ADD_(MUL_(dR, LN_(cL)), DIV_(MUL_(cR, dL), cL)));
                }
                break;
                }

            case OPERATOR_LN:
                TEX_LN; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), cL), dL); break;
                
            case OPERATOR_LOG:
                TEX_LOG; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), MUL_(cR, LN_(cL))), dR); break;
                
            case OPERATOR_SIN:
                TEX_SIN; new_node = MUL_(COS_(cL), dL); break;
                
            case OPERATOR_COS:
                TEX_COS; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), SIN_(cL)), dL); break;
                
            case OPERATOR_TG:
                TEX_TG; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(COS_(cL), NUMBER_NODE_CTOR(2))), dL); break;
                
            case OPERATOR_CTG:
                TEX_CTG; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SIN_(cL), NUMBER_NODE_CTOR(2)))), dL); break;
                
            case OPERATOR_ARCSIN:
                TEX_ARCSIN; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(SUB_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))), NUMBER_NODE_CTOR(0.5))), dL); break;
                
            case OPERATOR_ARCCOS:
                TEX_ARCCOS; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SUB_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))), NUMBER_NODE_CTOR(0.5)))), dL); break;
                
            case OPERATOR_ARCTG:
                TEX_ARCTG; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), ADD_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2)))), dL); break;
                
            case OPERATOR_ARCCTG:
                TEX_ARCCTG; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), ADD_(NUMBER_NODE_CTOR(1), POW_(cL, NUMBER_NODE_CTOR(2))))), dL); break;

            case OPERATOR_SH:
                TEX_SH; new_node = MUL_(CH_(cL), dL); break;

            case OPERATOR_CH:
                TEX_CH; new_node = MUL_(SH_(cL), dL); break;

            case OPERATOR_TH:
                TEX_TH; new_node = MUL_(DIV_(NUMBER_NODE_CTOR(1), POW_(CH_(cL), NUMBER_NODE_CTOR(2))), dL); break;

            case OPERATOR_CTH:
                TEX_CTH; new_node = MUL_(MUL_(NUMBER_NODE_CTOR(-1), DIV_(NUMBER_NODE_CTOR(1), POW_(SH_(cL), NUMBER_NODE_CTOR(2)))), dL); break;

            case WRONG_OPERATOR:
            default: return NULL;
        }
    }

    fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);
    PrintExpressionToTex(differentiator, tree_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ") = ");
    PrintExpressionToTex(differentiator, new_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\n");

    return new_node;
}

Tree_node* CopyPartOfTree(Differentiator* differentiator, Tree_node* old_node) {
    assert(differentiator);

    if (old_node == NULL) {
        return NULL;
    }

    Tree_node* new_node = NodeCtor(PointerOnTree(differentiator), old_node->type, old_node->value, NULL, NULL);
    if (new_node == NULL) {
        return NULL;
    }

    new_node->type = old_node->type;
    new_node->value = old_node->value;

    new_node->left_node = CopyPartOfTree(differentiator, old_node->left_node);
    new_node->right_node = CopyPartOfTree(differentiator, old_node->right_node);

    return new_node;
}

Status_of_finding ContainsVariable(Differentiator* differentiator, Tree_node* tree_node, const char* variable) {
    assert(differentiator);
    assert(variable);

    if (tree_node == NULL) return FIND_NO;
    
    switch (tree_node->type) {
        case VARIABLE:
            if (strcmp(NameOfVariable(differentiator, tree_node), variable) == 0)
                return FIND_YES;
            return FIND_NO;
        case NUMBER:
        case OPERATOR:
            if (ContainsVariable(differentiator, tree_node->left_node, variable) == FIND_YES ||
                ContainsVariable(differentiator, tree_node->right_node, variable) == FIND_YES)
                    return FIND_YES;
            else
                return FIND_NO;
        case WRONG_TYPE:
        default:
            return FIND_NO;
    }

    return FIND_NO;
}

int ReadNumberDifferentiation() {
    int number = -1;

    int cnt_attempts = CNT_ATTEMPTS;
    while (cnt_attempts-- > 0) {
        color_printf(COLOR_PURPLE, " - Enter, please, number of differention\n");

        if (scanf("%d%*c", &number) != 1)
            scanf("%*[^\n]%*c");

        if (number >= 0)
            return number;
    }

    return number;
}

void OptimizationTree(Differentiator* differentiator, Tree_node** old_node, const char* variable) {
    if (old_node == NULL) 
        return;

    if ((*old_node)->left_node)
        OptimizationTree(differentiator, &(*old_node)->left_node, variable);
    if ((*old_node)->right_node)
        OptimizationTree(differentiator, &(*old_node)->right_node, variable);

    if ((*old_node)->type == OPERATOR)
        OptimizationNode(differentiator, old_node, variable);
}

void OptimizationNode(Differentiator* differentiator, Tree_node** old_node, const char* variable) {
    Tree* tree = PointerOnTree(differentiator);

    if (IsConstantNode(differentiator, *old_node, variable)) {
        double result = Calculating(differentiator, (*old_node));
        DifferentiatorNodeDtor(differentiator, *old_node);
        *old_node = NUMBER_NODE_CTOR(result);
        return;
    }
    
    if ((*old_node)->value.operators == OPERATOR_MUL && 
        (IsZeroNode((*old_node)->left_node) || IsZeroNode((*old_node)->right_node))) {
        DifferentiatorNodeDtor(differentiator, (*old_node));
        (*old_node) = NUMBER_NODE_CTOR(0.0);
        return;
    }

    if (IsOneNode((*old_node)->left_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_MUL:
            case OPERATOR_DIV:
            case OPERATOR_POW:
                {
                    Tree_node* right_child = (*old_node)->right_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->left_node);
                    free((*old_node));
                    (*old_node) = right_child;
                    return;
                }            
            default:
                break;
        }
    }

    if (IsOneNode((*old_node)->right_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_MUL:
            case OPERATOR_DIV:
            case OPERATOR_POW:
                {
                    Tree_node* left_child = (*old_node)->left_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->right_node);
                    free((*old_node));
                    (*old_node) = left_child;
                    return;
                }
            default:
                break;
            }
    }

    if (IsZeroNode((*old_node)->left_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_ADD:
            case OPERATOR_SUB:
                {
                    Tree_node* right_child = (*old_node)->right_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->left_node);
                    free((*old_node));
                    (*old_node) = right_child;
                    return;
                }
            case OPERATOR_POW:
                {
                    DifferentiatorNodeDtor(differentiator, (*old_node)->left_node);
                    (*old_node) = NUMBER_NODE_CTOR(1.0);
                    return;
                }
            default:
                break;
            }
    }

    if (IsZeroNode((*old_node)->right_node)) {
        switch ((*old_node)->value.operators) {
            case OPERATOR_ADD:
            case OPERATOR_SUB:
                {
                    Tree_node* left_child = (*old_node)->left_node;
                    DifferentiatorNodeDtor(differentiator, (*old_node)->right_node);
                    free((*old_node));
                    (*old_node) = left_child;
                    return;
                }
            case OPERATOR_POW:
                {
                    DifferentiatorNodeDtor(differentiator, (*old_node)->right_node);
                    free((*old_node));
                    (*old_node) = NUMBER_NODE_CTOR(1.0);
                    return;
                }
            default:
                break;
            }
    }
}

bool IsConstantNode(Differentiator* differentiator, Tree_node* node, const char* variable) {
    if (node == NULL) return true;
    
    switch (node->type) {
        case NUMBER:
            return true;
            
        case VARIABLE:
            return strcmp(NameOfVariable(differentiator, node), variable) != 0;
            
        case OPERATOR:
            return IsConstantNode(differentiator, node->left_node, variable) && 
                   IsConstantNode(differentiator, node->right_node, variable);
            
        default:
            return false;
    }
}

bool IsZeroNode(Tree_node* node) {
    if (node == NULL) 
        return false;

    return (node->type == NUMBER && fabs(node->value.number) < 1e-10);
}

bool IsOneNode(Tree_node* node) {
    if (node == NULL) 
        return false;

    return (node->type == NUMBER && fabs(node->value.number - 1.0) < 1e-10);
}