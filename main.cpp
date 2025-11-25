#include "common.h"
#include "differentiator.h"
#include "tree.h"

int main(int, char** argv) {
    Differentiator differentiator = {};

    const char* html_dump_filename = argv[1];
    const char* tex_dump_filename  = argv[2];
    const char* directory          = argv[3];

    DifferentiatorCtor(&differentiator, html_dump_filename, tex_dump_filename, directory);

    UploadTree(&differentiator);

    CalculateValueOfExample(&differentiator);

    TreeTexDump(&differentiator, &differentiator.tree);

    DifferentiatorDtor(&differentiator);

    return 0;
}