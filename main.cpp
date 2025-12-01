#include <stdlib.h>

#include "array.h"
#include "common.h"
#include "differentiator.h"
#include "differentiation_functions.h"
#include "tree.h"
#include "read_expression.h"

int main(int, char** argv) {
    // Tree_status status = SUCCESS;

    Differentiator differentiator = {};

    const char* html_dump_filename = argv[1];
    const char* tex_dump_filename  = argv[2];
    const char* directory          = argv[3]; // FIXME check
    // const char* file = argv[4];

    // // FIXME check error
    DifferentiatorCtor(&differentiator, html_dump_filename, tex_dump_filename, directory);

    // About_tree* about_tree = (About_tree*)calloc(1, sizeof(About_tree));
    // about_tree->tree = (Tree*)calloc(1, sizeof(Tree));
    // about_tree->tree->root = GetComandir(&differentiator, &status, file);

    // ArrayPushtrees(&differentiator.array_with_trees, about_tree);
    // TreeHTMLDump(&differentiator, about_tree->tree->root, DUMP_INFO, NOT_ERROR_DUMP);

    StartDifferntiator(&differentiator);

    DifferentiatorDtor(&differentiator);

    return 0;
}