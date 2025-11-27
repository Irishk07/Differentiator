#ifndef SPUSK_H_
#define SPUSK_H_

#define CHECK_AND_RETURN_ERRORS(error, ...)                          \
        {                                                                 \
            status now_error = (error);                              \
            if (now_error != ALL_RIGHT) {                                   \
                fprintf(stderr, "Error is: %d, %d\n", (error), __LINE__); \
                __VA_ARGS__;                                              \
                return now_error;                                         \
            }                                                             \
        }


enum status {
    ALL_RIGHT = 0,
    NOT_END_SYMBOL = 1,
    NOT_NUMBER = 2,
    NOT_END_SKOBKA = 3,
    OPEN_ERRORS = 4,
    STAT_ERRORS = 5,
    MEMORY_ERRORS = 6,
    READ_ERRORS = 7
};


status GetComandir(int* val, const char* file_name);

status GetExpression(char** str, int* val);

status GetTerm(char** str, int* val);

status GetP(char** str, int* val);

status GetNumber(char** str, int* val);

int SpuskSizeOfText(const char *text_name);

status SpuskReadOnegin(char** str, const char* name_file);


#endif // SPUSK_H_