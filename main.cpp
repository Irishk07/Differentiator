#include "common.h"
#include "differentiator.h"
#include "differentiation_functions.h"
#include "tree.h"
#include "spusk.h"

int main(int, char** argv) {
    // int val = 0;
    // GetComandir(&val, argv[1]);
    // printf("result = %d\n", val);

    Differentiator differentiator = {};

    const char* html_dump_filename = argv[1];
    const char* tex_dump_filename  = argv[2];
    const char* directory          = argv[3]; // FIXME check

    // FIXME check error
    DifferentiatorCtor(&differentiator, html_dump_filename, tex_dump_filename, directory);

    UploadTree(&differentiator);

    // CalculateValueOfExample(&differentiator);

    Differentiation(&differentiator);

    DifferentiatorDtor(&differentiator);

    return 0;
}