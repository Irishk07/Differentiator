#include "common.h"
#include "differentiator.h"
#include "tree.h"

int main(int, char** argv) {
    Differentiator differentiator = {};

    const char* dump_filename = argv[1];
    const char* directory     = argv[2];

    DifferentiatorCtor(&differentiator, dump_filename, directory);

    UploadTree(&differentiator);

    CalculateValueOfExample(&differentiator);

    DifferentiatorDtor(&differentiator);

    return 0;
}