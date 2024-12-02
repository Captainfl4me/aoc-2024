#include "include/day.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct string {
    char* text;
    size_t length;
} string;

size_t get_number_of_line(char* input, size_t strlen)
{
    size_t counter = 0;
    for (unsigned int i = 0; i < strlen; i++) {
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

int part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    uint32_t* left_table = (uint32_t*)malloc(sizeof(uint32_t) * nb_line);
    uint32_t* right_table = (uint32_t*)malloc(sizeof(uint32_t) * nb_line);

    for (int i = 0; i < nb_line; i++) {
        char* first_space = strchr(string_vector[i].text, ' ');
        *first_space = '\0';
        left_table[i] = atoi(string_vector[i].text);
        right_table[i] = atoi(first_space + 3);
    }
    free(string_vector);

    qsort(left_table, nb_line, sizeof(uint32_t), compare_uint32_t);
    qsort(right_table, nb_line, sizeof(uint32_t), compare_uint32_t);

    uint32_t sum = 0;
    for (int i = 0; i < nb_line; i++) {
        sum += abs((int32_t)left_table[i] - (int32_t)right_table[i]);
    }

    return sum;
}

int part_2(char* input, size_t strlen)
{
    return 0;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>

Test(aoc, number_of_line)
{
    char test_input[36] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
    cr_assert(eq(ullong, get_number_of_line(test_input, 36), 6));
}

Test(aoc, split_by_line)
{
    char test_input[37] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
    size_t nb_line = 0;
    string* string_vector = split_by_lines(test_input, 36, &nb_line);
    cr_assert(eq(ulong, nb_line, 6));
    cr_assert(eq(str, string_vector[0].text, "3   4"));
    cr_assert(eq(str, string_vector[1].text, "4   3"));
    cr_assert(eq(str, string_vector[2].text, "2   5"));
    cr_assert(eq(str, string_vector[3].text, "1   3"));
    cr_assert(eq(str, string_vector[4].text, "3   9"));
    cr_assert(eq(str, string_vector[5].text, "3   3"));
    free(string_vector);
}
Test(aoc, part_1)
{
    char test_input[37] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
    cr_assert(eq(int, part_1(test_input, 37), 11));
}
#endif
