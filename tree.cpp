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

Tree_node* NodeCtor(Tree* tree, Type_node type, type_t value,
                    Tree_node* left_node, Tree_node* right_node) {
    assert(tree);

    Tree_node* new_node = (Tree_node*)calloc(1, sizeof(Tree_node));
    if (new_node == NULL)
        return NULL;

    new_node->left_node  = left_node;
    new_node->right_node = right_node;
    new_node->type       = type;
    new_node->value      = value;
    tree->size++;

    return new_node;
}


Tree_status TreeVerify(Differentiator* differentiator) {
    Tree* tree = PointerOnTree(differentiator);

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

    if (tree_node == NULL) TREE_CHECK_AND_RETURN_ERRORS(NULL_POINTER_ON_NODE);

    TREE_CHECK_AND_RETURN_ERRORS(AllNodesVerify(tree, tree_node->left_node));

    TREE_CHECK_AND_RETURN_ERRORS(AllNodesVerify(tree, tree_node->right_node));

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

    answer[strlen(answer) - 1] = '\0'; // FIXME no strlen 

    return answer;
}

size_t IndexOfVariable(Tree_node* tree_node) {
    assert(tree_node);

    return (size_t)tree_node->value.index_variable;
}

double ValueOfVariable(Differentiator* differentiator, Tree_node* tree_node) {
    assert(differentiator);
    assert(tree_node);

    return (differentiator->array_with_variables.data)[IndexOfVariable(tree_node)]->value;
}

char* NameOfVariable(Differentiator* differentiator, Tree_node* tree_node) {
    assert(differentiator);
    assert(tree_node);

    return (differentiator->array_with_variables.data)[IndexOfVariable(tree_node)]->name;
}

Tree* PointerOnTree(Differentiator* differentiator) {
    assert(differentiator);

    return differentiator->array_with_trees.data[differentiator->array_with_trees.size - 1];
}

const char* IndetifySign(Tree_node* tree_node) {
    assert(tree_node);

    switch(tree_node->type) {
        case OPERATOR:
            switch (tree_node->value.operators) {
                case OPERATOR_ADD:    return "+";
                case OPERATOR_SUB:    return "-";
                case OPERATOR_MUL:    return "*";
                case OPERATOR_DIV:    return "/";
                case OPERATOR_POW:    return "^";
                case OPERATOR_LN:     return "ln";
                case OPERATOR_LOG:    return "log";
                case OPERATOR_SIN:    return "sin";
                case OPERATOR_COS:    return "cos";
                case OPERATOR_TG:     return "tg";
                case OPERATOR_CTG:    return "ctg";
                case OPERATOR_ARCSIN: return "arcsin";
                case OPERATOR_ARCCOS: return "arccos";
                case OPERATOR_ARCTG:  return "arctg";
                case OPERATOR_ARCCTG: return "arcctg";
                case OPERATOR_SH:     return "sh";
                case OPERATOR_CH:     return "ch";
                case OPERATOR_TH:     return "th";
                case OPERATOR_CTH:    return "cth";
                case WRONG_OPERATOR:
                default: break;
            }
        case NUMBER:
        case VARIABLE:
        case WRONG_TYPE:
        default: break;
    }

    return NULL;
}


Tree_status TreeHTMLDump(Differentiator* differentiator, Tree_node* tree_node, int line, const char* file, Type_dump type_dump, Tree_status tree_status) {
    assert(differentiator);
    assert(file);

    FILE* html_dump_file = NULL;
    if (differentiator->dump_info.num_html_dump == 0)
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

    fprintf(html_dump_file, "Size: %zu\n", PointerOnTree(differentiator)->size);

    fprintf(html_dump_file, "Root: %p\n", PointerOnTree(differentiator)->root);

    TREE_CHECK_AND_RETURN_ERRORS(GenerateGraph(differentiator, tree_node));

    char command[MAX_LEN_NAME] = {};
    snprintf(command, MAX_LEN_NAME, "dot %s/graphes/graph%d.txt -T png -o %s/images/image%d.png", 
                                     differentiator->dump_info.directory, differentiator->dump_info.num_html_dump, 
                                     differentiator->dump_info.directory, differentiator->dump_info.num_html_dump);
    
    if (system((const char*)command) != 0)
        TREE_CHECK_AND_RETURN_ERRORS(EXECUTION_FAILED,      fprintf(html_dump_file, "Error with create image:(\n"));

    fprintf(html_dump_file, "\n");
    fprintf(html_dump_file, "<img src = %s/images/image%d.png width = 700px>", differentiator->dump_info.directory, differentiator->dump_info.num_html_dump);

    fprintf(html_dump_file, "\n\n");

    differentiator->dump_info.num_html_dump++;

    if (fclose(html_dump_file) == EOF)
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    return SUCCESS;
}

Tree_status GenerateGraph(Differentiator* differentiator, Tree_node* tree_node) {
    assert(differentiator);

    char filename_graph[MAX_LEN_NAME] = {};
    snprintf(filename_graph, MAX_LEN_NAME, "%s/graphes/graph%d.txt", differentiator->dump_info.directory, differentiator->dump_info.num_html_dump);

    FILE* graph = fopen(filename_graph, "w");
    if (graph == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    fprintf(graph, "digraph {\n");
    fprintf(graph, "    node [shape = Mrecord, style = filled, fillcolor = \"#99FF99\"];\n");

    if (tree_node != NULL) {
        PrintNodeToDot(differentiator, graph, tree_node);
    } 
    else {
        fprintf(graph, "There will be no tree\n");
    }

    fprintf(graph, "}\n");

    if (fclose(graph) == EOF)
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,    perror("Error is:"));

    return SUCCESS;
}

void PrintNodeToDot(Differentiator* differentiator, FILE *file, Tree_node* tree_node) {
    assert(differentiator);
    assert(file);

    if (tree_node->type == NUMBER) {
        fprintf(file, "    node_%p [label=\"{%p | type = Number | value = %lg | left = %p | right = %p}\"];\n", 
                      (void *)tree_node, tree_node, tree_node->value.number, tree_node->left_node, tree_node->right_node);
    }    

    else if (tree_node->type == VARIABLE) {
        fprintf(file, "    node_%p [label=\"{%p | type = Variable | value = %s (%lg) | left = %p | right = %p}\"];\n", 
                      (void *)tree_node, tree_node, NameOfVariable(differentiator, tree_node), ValueOfVariable(differentiator, tree_node), 
                      tree_node->left_node, tree_node->right_node);
    }

    else if (tree_node->type != WRONG_TYPE) {
        fprintf(file, "    node_%p [label=\"{%p | type = Operator | value = %s | left = %p | right = %p}\"];\n", 
                      (void *)tree_node, tree_node, IndetifySign(tree_node), tree_node->left_node, tree_node->right_node);
    }  


    if (tree_node->left_node) {
        fprintf(file, "    node_%p -> node_%p [label = \"Yes\", color = \"#FF6600\"];\n ", (void *)tree_node, (void *)tree_node->left_node);
        PrintNodeToDot(differentiator, file, tree_node->left_node);
    }

    if (tree_node->right_node) {
        fprintf(file, "    node_%p -> node_%p [label = \"No\", color = \"#0000CC\"];\n", (void *)tree_node, (void *)tree_node->right_node);
        PrintNodeToDot(differentiator, file, tree_node->right_node);
    }
}

void PrintErrors(int error, FILE* stream) {
    assert(stream);

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

Tree_status CreateTexFileForDump(Differentiator* differentiator) {
    assert(differentiator);

    FILE* tex_dump_file = fopen(differentiator->dump_info.tex_dump_filename, "w");

    if (tex_dump_file == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(OPEN_ERROR);

    fprintf(tex_dump_file, "\\documentclass[14pt]{extarticle}\n");
    fprintf(tex_dump_file, "\\usepackage{graphicx}\n");
    fprintf(tex_dump_file, "\\usepackage{amsmath, amssymb}\n");
    fprintf(tex_dump_file, "\\usepackage[english, russian]{babel}\n");
    fprintf(tex_dump_file, "\\usepackage{geometry}\n");
    fprintf(tex_dump_file, "\\geometry{left=2.5cm, right=2.5cm, top=2cm, bottom=2cm}\n");
    fprintf(tex_dump_file, "\\usepackage{bm}\n");
    fprintf(tex_dump_file, "\\usepackage{indentfirst}\n");
    fprintf(tex_dump_file, "\\linespread{1.3}\n");
    fprintf(tex_dump_file, "\\parindent=1.5cm\n \\usepackage{tocbibind}\n");
    fprintf(tex_dump_file, "\\begin{document}\n");

    fprintf(tex_dump_file, "\\vspace*{3cm} \\begin{center}\n");
    fprintf(tex_dump_file, "{\\Huge \\textbf{Муррифицирование}}\n\n");
    fprintf(tex_dump_file, "\\end{center}\n");
    fprintf(tex_dump_file, "\\begin{figure}[h]\n");
    fprintf(tex_dump_file, "\\centering\n");
    fprintf(tex_dump_file, "\\includegraphics[width=0.7\\textwidth]{cat.png}\n");
    fprintf(tex_dump_file, "\\end{figure}\n\\newpage\n");

    fprintf(tex_dump_file, "\\tableofcontents\n\\newpage\n");
    fprintf(tex_dump_file, "\\section*{Процесс Муррифицирования}\n");
    fprintf(tex_dump_file, "\\noindent \\textit{Исследование проведено при непосредственном участии кота Барсика} \\\\ \n"
                            "\\newline\n"
                            "Дорогие читатели! Перед вами фундаментальный труд, раскрывающий глубинные связи"
                            "между дифференциальным исчислением и кошачьей психологией.\\\\ \n" 
                            "Как известно, каждый уважающий себя кот является прирожденным математиком - стоит только понаблюдать," 
                            "с какой грацией он вычисляет траекторию прыжка на подоконник или производную скорости убегания от ветеринара.\\\\ \n"
                            "\\newline\n"
                            "В данной работе мы докажем, что:"
                            "\\begin{itemize}\n"
                            "\\item Дифференциал кошачьего настроения всегда положителен в окрестности холодильника \n"
                            "\\item Ряд Тейлора для мурчания сходится абсолютно и равномерно \n"
                            "\\end{itemize} \n"
                            "Приступаем к вычислениям, и да поможет нам великий кот Ньютон! \\\\ \n"
                            "\\newline\n");

    differentiator->dump_info.tex_dump_file = tex_dump_file;

    return SUCCESS;
}

Tree_status TreeTexDump(Differentiator* differentiator, Tree_node* old_tree_root, Tree_node* new_tree_root, char* variable) {
    assert(differentiator);
    assert(differentiator->dump_info.tex_dump_file);

    fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}\n");
    fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);
    PrintExpressionToTex(differentiator, old_tree_root, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, ") = ");
    PrintExpressionToTex(differentiator, new_tree_root, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);
    fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}\n");    

    fprintf(differentiator->dump_info.tex_dump_file, "\\\\\n");

    return SUCCESS;
}

#define OPERATORS_TO_TEX
#include "tex.h"
void PrintExpressionToTex(Differentiator* differentiator, Tree_node* tree_node, FILE* tex_dump_file, Prioritets parent_prioritet, int depth) {
    assert(differentiator);
    assert(tree_node);
    assert(tex_dump_file);

    Prioritets current_prioritet = GetPrioritet(tree_node);

    if (current_prioritet < parent_prioritet && current_prioritet != NO_PRIORITET)
        fprintf(tex_dump_file, "(");

    switch (tree_node->type) {
        case NUMBER:
            if (tree_node->value.number < 0)
                fprintf(tex_dump_file, "(%lg)", tree_node->value.number);
            else
                fprintf(tex_dump_file, "%lg", tree_node->value.number);
            break; 
        case VARIABLE:
            fprintf(tex_dump_file, "%s", NameOfVariable(differentiator, tree_node));
            break;
        case OPERATOR:
            switch(tree_node->value.operators) {
                case OPERATOR_ADD:
                    PRINT_OPERATOR_TO_TEX(" + ");
                case OPERATOR_SUB:
                    PRINT_OPERATOR_TO_TEX(" - ");
                case OPERATOR_MUL:
                    PRINT_OPERATOR_TO_TEX(" \\cdot ");
                case OPERATOR_DIV:
                    PRINT_DIV_OPERATOR_TO_TEX;
                case OPERATOR_POW:
                    PRINT_POW_OPERATOR_TO_TEX;
                case OPERATOR_LN:
                    PRINT_LN_OPERATOR_TO_TEX;
                case OPERATOR_LOG:
                    PRINTF_LOG_OPERATOR_TO_TEX;
                case OPERATOR_SIN:
                    PRINTF_SIN_OPERATOR_TO_TEX;
                case OPERATOR_COS:
                    PRINTF_COS_OPERATOR_TO_TEX;
                case OPERATOR_TG:
                    PRINTF_TG_OPERATOR_TO_TEX;
                case OPERATOR_CTG:
                    PRINTF_CTG_OPERATOR_TO_TEX;
                case OPERATOR_ARCSIN:
                    PRINTF_ARCSIN_OPERATOR_TO_TEX;
                case OPERATOR_ARCCOS:
                    PRINTF_ARCCOS_OPERATOR_TO_TEX;
                case OPERATOR_ARCTG:
                    PRINTF_ARCTG_OPERATOR_TO_TEX;
                case OPERATOR_ARCCTG:
                    PRINTF_ARCCTG_OPERATOR_TO_TEX;
                case OPERATOR_SH:
                    PRINTF_SH_OPERATOR_TO_TEX;
                case OPERATOR_CH:
                    PRINTF_CH_OPERATOR_TO_TEX;
                case OPERATOR_TH:
                    PRINTF_TH_OPERATOR_TO_TEX;
                case OPERATOR_CTH:
                    PRINTF_CTH_OPERATOR_TO_TEX;
                case WRONG_OPERATOR:
                default: break;
            }
        case WRONG_TYPE:
        default:
            break;
    }

    if (current_prioritet < parent_prioritet && current_prioritet != NO_PRIORITET)
        fprintf(tex_dump_file, ")");
}

#undef OPERATORS_TO_TEX

Prioritets GetPrioritet(Tree_node* tree_node) {
    assert(tree_node);

    if (tree_node->type == OPERATOR) {
        switch (tree_node->value.operators) {
            case OPERATOR_ADD:
            case OPERATOR_SUB:
                return LOW_PRIOTITET;
            case OPERATOR_MUL:
            case OPERATOR_DIV:
                return MIDDLE_PRIORITET;
            case OPERATOR_POW:
                return HIGH_PRIORITET;
            case OPERATOR_LN: 
            case OPERATOR_LOG:
            case OPERATOR_SIN:
            case OPERATOR_COS:
            case OPERATOR_TG: 
            case OPERATOR_CTG:
            case OPERATOR_ARCSIN: 
            case OPERATOR_ARCCOS: 
            case OPERATOR_ARCTG: 
            case OPERATOR_ARCCTG: 
            case OPERATOR_SH:
            case OPERATOR_CH:
            case OPERATOR_TH:
            case OPERATOR_CTH:
                return HIGHEST_PRIORITET; 
            case WRONG_OPERATOR:
            default: return NO_PRIORITET;
        }
    }

    else {
        return NO_PRIORITET;
    }
}

Tree_status ClostTexFileForDump(Differentiator* differentiator) {
    assert(differentiator);
    assert(differentiator->dump_info.tex_dump_file);

    fprintf(differentiator->dump_info.tex_dump_file, "\\end{document}\n");

    if (fclose(differentiator->dump_info.tex_dump_file) == EOF)                                             
        TREE_CHECK_AND_RETURN_ERRORS(CLOSE_ERROR,      perror("Error is: "));

    char command[MAX_LEN_NAME] = {};
    snprintf(command, MAX_LEN_NAME, "pdflatex %s", differentiator->dump_info.tex_dump_filename);
    if (system(command) != 0)
        TREE_CHECK_AND_RETURN_ERRORS(EXECUTION_FAILED);

    return SUCCESS;
}