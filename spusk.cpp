#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "spusk.h"

// TODO написать в текстовый файл грамматику

status GetComandir(int* val, const char* file_name) {
    assert(val);
    assert(file_name);

    char* str = NULL;
    SpuskReadOnegin(&str, file_name);
    char* temp = str;

    CHECK_AND_RETURN_ERRORS(GetExpression(&str, val));

    if (*str != '$')
        CHECK_AND_RETURN_ERRORS(NOT_END_SYMBOL);

    free(temp);

    return ALL_RIGHT;
}

status GetExpression(char** str, int* val) {
    assert(str);
    assert(*str);

    CHECK_AND_RETURN_ERRORS(GetTerm(str, val));

    while (**str == '+' || **str == '-') {
        int cur_operator = (**str);
        (*str)++;

        int val2 = 0;
        CHECK_AND_RETURN_ERRORS(GetTerm(str, &val2));

        if (cur_operator == '+')
            *val += val2;
        else 
            *val -= val2;
    }

    return ALL_RIGHT;
}

status GetTerm(char** str, int* val) {
    assert(str);
    assert(*str);

    CHECK_AND_RETURN_ERRORS(GetP(str, val));

    while (**str == '*' || **str == '/') {
        int cur_operator = (**str);
        (*str)++;

        int val2 = 0;
        CHECK_AND_RETURN_ERRORS(GetP(str, &val2));
        if (cur_operator == '*')
            *val *= val2;
        else 
            *val /= val2;
    }

    return ALL_RIGHT;
}

status GetP(char** str, int* val) {
    assert(str);
    assert(*str);

    if (**str == '(') {
        (*str)++;
        CHECK_AND_RETURN_ERRORS(GetExpression(str, val));

        if (**str != ')')
            CHECK_AND_RETURN_ERRORS(NOT_END_SKOBKA);

        (*str)++;
    }

    else 
        CHECK_AND_RETURN_ERRORS(GetNumber(str, val));

    return ALL_RIGHT;
}

status GetNumber(char** str, int* val) {
    assert(str);
    assert(*str);

    char* old_str = *str;

    while ('0' <= **str && **str <= '9') {
        *val = *val * 10 + (**str - '0');
        (*str)++;
    }

    if (old_str == *str)
        CHECK_AND_RETURN_ERRORS(NOT_NUMBER);

    return ALL_RIGHT;
}



int SpuskSizeOfText(const char *text_name) {
    assert(text_name);

    struct stat text_info = {};

    if (stat(text_name, &text_info) == -1) {
        perror("Error is");

        return -1;
    }

    return (int)text_info.st_size;
}

status SpuskReadOnegin(char** str, const char* name_file) {
    assert(name_file != NULL);
    assert(str);    

    FILE *text = fopen(name_file, "r");
    if (text == NULL) {
        perror("Error is");
        return OPEN_ERRORS;
    }

    int size = SpuskSizeOfText(name_file);
    if (size == -1) {
        return STAT_ERRORS;
    }

    *str = (char*)calloc((size_t)size, sizeof(char));
    if (str == NULL)
        CHECK_AND_RETURN_ERRORS(MEMORY_ERRORS);

    fread((char *)*str, sizeof(char), (size_t)size, text);
    if (ferror(text) != 0) {
        CHECK_AND_RETURN_ERRORS(READ_ERRORS,    free(*str));
    }

    return ALL_RIGHT;
}