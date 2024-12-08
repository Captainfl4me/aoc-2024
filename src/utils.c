#include "include/utils.h"
#include <stdint.h>
#include <string.h>

size_t get_number_of_line(char* input, size_t strlen)
{
    size_t counter = 0;
    for (size_t i = 0; i < strlen; i++) {
        if (input[i] == '\n') {
            counter++;
        }
    }

    return counter;
}

string* split_by_lines(char* input, size_t strlen, size_t* vector_length)
{
    size_t number_of_line = get_number_of_line(input, strlen);
    size_t string_vector_id = 0;
    string* string_vector = (string*)malloc(number_of_line * sizeof(string));

    char *new_pointer, *last_pointer = input;
    while ((new_pointer = strchr(last_pointer, '\n'))) {
        size_t substring_length = new_pointer - last_pointer + 1;
        string_vector[string_vector_id].length = substring_length;

        string_vector[string_vector_id].text = (char*)malloc(sizeof(char) * substring_length);
        memcpy(string_vector[string_vector_id].text, last_pointer, substring_length - 1);
        string_vector[string_vector_id].text[substring_length - 1] = '\0';

        string_vector_id++;
        if ((size_t)(new_pointer - input + 1) < strlen) {
            last_pointer = new_pointer + 1;
        } else {
            break;
        }
    }

    *vector_length = string_vector_id;
    return string_vector;
}

int compare_uint32_t(const void* a, const void* b)
{
    return (*(uint32_t*)a - *(uint32_t*)b);
}
