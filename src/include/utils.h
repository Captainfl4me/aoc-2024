#ifndef H_UTILS
#define H_UTILS
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct string {
    char* text;
    size_t length;
} string;

size_t get_number_of_line(char* input, size_t strlen);
string* split_by_lines(char* input, size_t strlen, size_t* vector_length);
int compare_uint32_t(const void* a, const void* b);

#endif
