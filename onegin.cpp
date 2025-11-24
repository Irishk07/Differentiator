#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "onegin.h"

#include "common.h"


int SizeOfText(const char *text_name) {
    assert(text_name);

    struct stat text_info = {};

    if (stat(text_name, &text_info) == -1) {
        perror("Error is");

        return -1;
    }

    return (int)text_info.st_size;
}

Tree_status ReadOnegin(Differentiator* differentiator, const char* name_file) {
    assert(differentiator != NULL);    

    FILE *text = fopen(name_file, "r");
    if (text == NULL) {
        perror("Error is");
        return OPEN_ERROR;
    }

    int size = SizeOfText(name_file);
    if (size == -1) {
        return STAT_ERROR;
    }
    differentiator->size_buffer = (size_t)size;

    differentiator->buffer_with_tree = (char*)calloc((differentiator->size_buffer + 1), sizeof(char));
    if (differentiator->buffer_with_tree == NULL)
        TREE_CHECK_AND_RETURN_ERRORS(MEMORY_ERROR);

    fread((char *)differentiator->buffer_with_tree, sizeof(char), differentiator->size_buffer, text);
    if (ferror(text) != 0) {
        TREE_CHECK_AND_RETURN_ERRORS(READ_ERROR,    free(differentiator->buffer_with_tree));
    }

    return SUCCESS;
}