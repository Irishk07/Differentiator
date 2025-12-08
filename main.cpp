#include <stdlib.h>

#include "array.h"
#include "common.h"
#include "differentiator.h"
#include "differentiation_functions.h"
#include "tree.h"
#include "read_expression.h"


int main(int argc, char** argv) {
    if (argc == 4) {
        Differentiator differentiator = {};

        const char* html_dump_filename = argv[1];
        const char* tex_dump_filename  = argv[2];
        const char* directory          = argv[3];

        // // FIXME check error
        DifferentiatorCtor(&differentiator, html_dump_filename, tex_dump_filename, directory);

        StartDifferntiator(&differentiator);

        DifferentiatorDtor(&differentiator);
    }

    return 0;
}