#include "include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct towel_pattern {
    size_t size;
    char* shape;
} towel_pattern;

uint8_t towel_pattern_eq(towel_pattern a, towel_pattern b)
{
    if (a.size != b.size) {
        return 0;
    }

    for (size_t i = 0; i < a.size - 1; i++) {
        if (a.shape[i] != b.shape[i]) {
            return 0;
        }
    }

    return 1;
}

LIST_DECL(towel_pattern, 5, towel_pattern_eq);
uint8_t priority_fct(uint16_t a, uint16_t b)
{
    return a > b;
}
PRIORITY_QUEUE_DECL(uint16_t, priority_fct);

void add_valid_towel_pattern_to_queue(priority_queue_uint16_t* queue, list_towel_pattern* towels, char* list, size_t start_index, size_t list_size, uint8_t* already_visited)
{
    for (size_t towel_index = 0; towel_index < towels->length; towel_index++) {
        if (list_size - start_index < towels->vector[towel_index].size) {
            continue;
        }

        uint8_t valid_towel = 1;
        for (size_t shape_index = 0; shape_index < towels->vector[towel_index].size; shape_index++) {
            if (list[start_index + shape_index] != towels->vector[towel_index].shape[shape_index]) {
                valid_towel = 0;
                break;
            }
        }

        size_t new_index = start_index + towels->vector[towel_index].size;
        if (valid_towel && !already_visited[new_index]) {
            push_to_priority_queue_uint16_t(queue, new_index);
            already_visited[new_index] = 1;
        }
    }
}

char* part_1(char* input, size_t strlen)
{
    UNUSED(strlen);
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    char *start_ptr = string_vector[0].text, *next_ptr;
    list_towel_pattern pattern_list = { .vector = NULL };
    init_list_towel_pattern(&pattern_list);
    while ((next_ptr = strchr(start_ptr, ','))) {
        size_t size = next_ptr - start_ptr;
        *next_ptr = '\0';
        add_to_list_towel_pattern(&pattern_list, (towel_pattern) { .size = size, .shape = start_ptr });
        start_ptr = next_ptr + 2;
    }
    add_to_list_towel_pattern(&pattern_list, (towel_pattern) { .size = string_vector[0].length - (start_ptr - string_vector[0].text) - 1, .shape = start_ptr });

    uint64_t possible_design = 0;
    for (size_t line = 2; line < nb_line; line++) {
        uint8_t visited_index[string_vector[line].length];
        for (size_t i = 0; i < string_vector[line].length; i++) {
            visited_index[i] = 0;
        }
        priority_queue_uint16_t index_queue;
        init_priority_queue_uint16_t(&index_queue);

        push_to_priority_queue_uint16_t(&index_queue, 0);
        visited_index[0] = 1;
        while (!is_priority_queue_empty_uint16_t(&index_queue)) {
            uint16_t index = index_queue.last->value;
            pop_priority_queue_uint16_t(&index_queue);

            if (index == string_vector[line].length - 1) {
                possible_design++;
                empty_priority_queue_uint16_t(&index_queue);
                break;
            } else if (index > string_vector[line].length - 1) {
                continue;
            }

            add_valid_towel_pattern_to_queue(&index_queue, &pattern_list, string_vector[line].text, index, string_vector[line].length, visited_index);
        }
    }
    return uint64_t_to_str(possible_design);
}

void add_valid_towel_pattern_to_visited_array(uint64_t* already_visited, list_towel_pattern* towels, char* list, size_t start_index, size_t list_size)
{
    for (size_t towel_index = 0; towel_index < towels->length; towel_index++) {
        if (list_size - start_index < towels->vector[towel_index].size) {
            continue;
        }

        uint8_t valid_towel = 1;
        for (size_t shape_index = 0; shape_index < towels->vector[towel_index].size; shape_index++) {
            if (list[start_index + shape_index] != towels->vector[towel_index].shape[shape_index]) {
                valid_towel = 0;
                break;
            }
        }

        size_t new_index = start_index + towels->vector[towel_index].size;
        if (valid_towel) {
            already_visited[new_index] += already_visited[start_index];
        }
    }
}

char* part_2(char* input, size_t strlen)
{
    UNUSED(strlen);
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    char *start_ptr = string_vector[0].text, *next_ptr;
    list_towel_pattern pattern_list = { .vector = NULL };
    init_list_towel_pattern(&pattern_list);
    while ((next_ptr = strchr(start_ptr, ','))) {
        size_t size = next_ptr - start_ptr;
        *next_ptr = '\0';
        add_to_list_towel_pattern(&pattern_list, (towel_pattern) { .size = size, .shape = start_ptr });
        start_ptr = next_ptr + 2;
    }
    add_to_list_towel_pattern(&pattern_list, (towel_pattern) { .size = string_vector[0].length - (start_ptr - string_vector[0].text) - 1, .shape = start_ptr });

    uint64_t possible_design = 0;
    for (size_t line = 2; line < nb_line; line++) {
        uint64_t visited_index[string_vector[line].length];
        for (size_t i = 0; i < string_vector[line].length; i++) {
            visited_index[i] = 0;
        }

        visited_index[0] = 1;
        for (size_t i = 0; i < string_vector[line].length; i++) {
			if (visited_index[i] == 0) {
				continue;
			}
            add_valid_towel_pattern_to_visited_array(visited_index, &pattern_list, string_vector[line].text, i, string_vector[line].length);
        }

		possible_design += visited_index[string_vector[line].length-1];
    }
    return uint64_t_to_str(possible_design);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "r, wr, b, g, bwu, rb, gb, br\n\nbrwrr\nbggr\ngbbr\nrrbgbr\nubwu\nbwurrg\nbrgr\nbbrgwb\n";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), "6"));
}

Test(aoc, part_2)
{
    char test_input_1[] = "r, wr, b, g, bwu, rb, gb, br\n\nbrwrr\nbggr\ngbbr\nrrbgbr\nubwu\nbwurrg\nbrgr\nbbrgwb\n";
    cr_assert(eq(str, part_2(test_input_1, sizeof(test_input_1)), "16"));
}
#endif
