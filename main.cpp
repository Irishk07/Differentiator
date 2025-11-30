#include "array.h"
#include "common.h"
#include "differentiator.h"
#include "differentiation_functions.h"
#include "tree.h"
#include "spusk.h"

int main(int, char** argv) {
    // Tree_status status = SUCCESS;

    Differentiator differentiator = {};

    const char* html_dump_filename = argv[1];
    const char* tex_dump_filename  = argv[2];
    const char* directory          = argv[3]; // FIXME check
    // const char* file = argv[4];

    // // FIXME check error
    DifferentiatorCtor(&differentiator, html_dump_filename, tex_dump_filename, directory);

    // Tree tree = {};
    // tree.root = GetComandir(&status, file);
    // ArrayPushtrees(&differentiator.array_with_trees, &tree);

    StartDifferntiator(&differentiator);

    DifferentiatorDtor(&differentiator);

    return 0;
}