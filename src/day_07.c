#include "include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct equation {
    uint64_t test_value;
    list values;
} equation;

typedef struct temp_result {
    uint64_t current_value;
    size_t current_index;
    struct temp_result* next_in_list;
    struct temp_result* prev_in_list;
} temp_result;

typedef struct result_queue {
    size_t queue_length;
    temp_result* first;
    temp_result* last;
} result_queue;

void init_queue(result_queue* queue)
{
    queue->queue_length = 0;
    queue->first = NULL;
    queue->last = NULL;
}
uint8_t is_queue_empty(result_queue* queue)
{
    return queue->queue_length == 0;
}
void push_to_queue(result_queue* queue, temp_result value)
{
    if (queue->last == NULL) {
        queue->first = (temp_result*)malloc(sizeof(temp_result));
        queue->first->current_value = value.current_value;
        queue->first->current_index = value.current_index;
        queue->first->next_in_list = NULL;
        queue->first->prev_in_list = NULL;

        queue->last = queue->first;
    } else {
        queue->last->next_in_list = (temp_result*)malloc(sizeof(temp_result));
        queue->last->next_in_list->current_value = value.current_value;
        queue->last->next_in_list->current_index = value.current_index;
        queue->last->next_in_list->next_in_list = NULL;
        queue->last->next_in_list->prev_in_list = queue->last;

        queue->last = queue->last->next_in_list;
    }
    queue->queue_length++;
}
void pop_queue(result_queue* queue)
{
    if (queue->queue_length == 0)
        return;

    if (queue->queue_length == 1) {
        free(queue->first);
        queue->first = NULL;
        queue->last = NULL;
    } else {
        temp_result* to_delete = queue->last;
        queue->last = queue->last->prev_in_list;
        queue->last->next_in_list = NULL;
        free(to_delete);
    }
    queue->queue_length--;
}
void empty_queue(result_queue* queue)
{
    while (!is_queue_empty(queue))
        pop_queue(queue);
}

uint64_t part_1(char* input, size_t str_len)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, str_len, &nb_line);

    // Parse text to equation list
    equation eq_list[nb_line];
    for (size_t i = 0; i < nb_line; i++) {
        eq_list[i].values.vector = NULL;
        eq_list[i].values.length = 0;
        eq_list[i].values.current_capacity = 0;
        init_list(&eq_list[i].values);

        // Start test value
        char* first_sep = strchr(string_vector[i].text, ':');
        if (first_sep == NULL) {
            continue;
        } else {
            *first_sep = '\0';
            eq_list[i].test_value = atoll(string_vector[i].text);
        }

        // Parse values list
        char* value_sep = first_sep + 2;
        parse_text_to_list(&eq_list[i].values, value_sep, ' ');
    }
    free(string_vector);

    uint64_t result_sum = 0;
    for (size_t i = 0; i < nb_line; i++) {
        result_queue queue;
        init_queue(&queue);

        push_to_queue(&queue, (temp_result) { .current_value = eq_list[i].values.vector[0], .current_index = 0 });
        while (!is_queue_empty(&queue)) {
            uint64_t current_value = queue.last->current_value;
            size_t next_index = queue.last->current_index + 1;
            pop_queue(&queue);

            if (next_index < eq_list[i].values.length) {
                if (current_value <= eq_list[i].test_value) {

                    push_to_queue(&queue, (temp_result) { .current_value = (current_value + (uint64_t)eq_list[i].values.vector[next_index]), .current_index = next_index });
                    push_to_queue(&queue, (temp_result) { .current_value = (current_value * (uint64_t)eq_list[i].values.vector[next_index]), .current_index = next_index });
                }
            } else if (current_value == eq_list[i].test_value) {
                result_sum += eq_list[i].test_value;
                break;
            }
        }
        empty_queue(&queue);
    }

    return result_sum;
}

uint64_t part_2(char* input, size_t strlen)
{
    return 0;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "190: 10 19\n3267: 81 40 27\n83: 17 5\n156: 15 6\n7290: 6 8 6 15\n161011: 16 10 13\n192: 17 8 14\n21037: 9 7 18 13\n292: 11 6 16 20\n";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 3749));
}
Test(aoc, part_1_testcase)
{
    char test_input_1[] = "190: 10 19 1\n";
    cr_assert(eq(int, part_1(test_input_1, sizeof(test_input_1)), 190));

    char test_input_2[] = "191: 10 19 1\n";
    cr_assert(eq(int, part_1(test_input_2, sizeof(test_input_2)), 191));

    char test_input_3[] = "29: 10 19 1\n";
    cr_assert(eq(int, part_1(test_input_3, sizeof(test_input_3)), 29));

    char test_input_4[] = "30: 10 19 1\n";
    cr_assert(eq(int, part_1(test_input_4, sizeof(test_input_4)), 30));

    char test_input_5[] = "19000: 10 19 100\n";
    cr_assert(eq(int, part_1(test_input_5, sizeof(test_input_5)), 19000));
}
Test(aoc, part_1_big_number)
{
    char test_input_1[] = "418300198: 83 297 755 54 27\n1361693900: 1 297 8 1 4 86 2 5 665\n59105280: 8 39 32 148 40\n2387431: 6 51 924 3 647 1\n1248447503: 6 4 610 5 6 8 592 83 15\n13146127111: 8 83 2 582 3 9 8 8 9 4 1 2\n15317144898469: 524 37 788 3 79 8 469\n805511: 73 3 7 4 968 149 3 8 9\n1702569: 3 741 9 454 67\n7054236: 2 84 3 42 651\n147968: 2 6 5 15 7 939 21 4 32\n";
    cr_assert(eq(ulong, part_1(test_input_1, sizeof(test_input_1)), 2678836318));

    char test_input_2[] = "2950073910: 331 35 7 774 47\n";
    cr_assert(eq(ulong, part_1(test_input_2, sizeof(test_input_2)), 2950073910));

    char test_input_3[] = "401473748420777: 802 94 74 96 841 5 6 5\n";
    cr_assert(eq(ulong, part_1(test_input_3, sizeof(test_input_3)), 0));

    char test_input_4[] = "246016: 56 274 32 16 3\n";
    cr_assert(eq(ulong, part_1(test_input_4, sizeof(test_input_4)), 0));
}
#endif
