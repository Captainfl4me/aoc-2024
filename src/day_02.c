#include "include/day.h"
#include "include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    int safe_counter = 0;
    size_t table_counter = 0;
    uint32_t series_table[64] = { 0 };
    char *current_pointer, *next_pointer;
    for (size_t i = 0; i < nb_line; i++) {
		table_counter = 0;
        current_pointer = string_vector[i].text;
        while ((next_pointer = strchr(current_pointer, ' '))) {
            *next_pointer = '\0';
            series_table[table_counter++] = atoi(current_pointer);
            current_pointer = next_pointer + 1;
        }
		*(string_vector[i].text + string_vector[i].length - 1) = '\0';
		series_table[table_counter++] = atoi(current_pointer);

        uint8_t is_decreasing = 0;
        uint8_t is_safe = 1;
        for (size_t j = 1; j < table_counter; j++) {
            int32_t diff = series_table[j] - series_table[j - 1];
            if (j == 1) {
                if (diff < 0) {
                    is_decreasing = 1;
                } else {
                    is_decreasing = 0;
                }
				if (abs(diff) < 1 || abs(diff) > 3) {
                    is_safe = 0;
                    break;
				}
            } else {
                if ((is_decreasing == 1 && diff > 0) || (is_decreasing == 0 && diff < 0) || (abs(diff) < 1 || abs(diff) > 3)) {
                    is_safe = 0;
                    break;
                }
            }
        }

        if (is_safe == 1) {
            safe_counter++;
        }
    }
    free(string_vector);
    return safe_counter;
}

int part_2(char* input, size_t strlen)
{
    return 0;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[60] = "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9\n";
    cr_assert(eq(int, part_1(test_input, 60), 2));
}

Test(aoc, part_2)
{
    char test_input[60] = "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9\n";
    cr_assert(eq(int, part_1(test_input, 60), 2));
}
#endif
