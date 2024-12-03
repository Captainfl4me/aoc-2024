#include "include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    uint32_t* left_table = (uint32_t*)malloc(sizeof(uint32_t) * nb_line);
    uint32_t* right_table = (uint32_t*)malloc(sizeof(uint32_t) * nb_line);

    for (size_t i = 0; i < nb_line; i++) {
        char* first_space = strchr(string_vector[i].text, ' ');
        *first_space = '\0';
        left_table[i] = atoi(string_vector[i].text);
        right_table[i] = atoi(first_space + 3);
    }
    free(string_vector);

    qsort(left_table, nb_line, sizeof(uint32_t), compare_uint32_t);
    qsort(right_table, nb_line, sizeof(uint32_t), compare_uint32_t);

    uint32_t sum = 0;
    for (size_t i = 0; i < nb_line; i++) {
        sum += abs((int32_t)left_table[i] - (int32_t)right_table[i]);
    }

    return sum;
}

int part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    uint32_t* left_table = (uint32_t*)malloc(sizeof(uint32_t) * nb_line);
    uint32_t* right_table = (uint32_t*)malloc(sizeof(uint32_t) * nb_line);

    for (size_t i = 0; i < nb_line; i++) {
        char* first_space = strchr(string_vector[i].text, ' ');
        *first_space = '\0';
        left_table[i] = atoi(string_vector[i].text);
        right_table[i] = atoi(first_space + 3);
    }
    free(string_vector);

    uint32_t sum = 0;
    for (size_t i = 0; i < nb_line; i++) {
        uint32_t counter = 0;
        for (size_t j = 0; j < nb_line; j++) {
            if (left_table[i] == right_table[j]) {
                counter++;
            }
        }
        sum += left_table[i] * counter;
    }

    return sum;
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

Test(aoc, part_2)
{
    char test_input[37] = "3   4\n4   3\n2   5\n1   3\n3   9\n3   3\n";
    cr_assert(eq(int, part_2(test_input, 37), 31));
}
#endif
