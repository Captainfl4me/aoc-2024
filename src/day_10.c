#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>

QUEUE_DECL(pos);

uint8_t is_pos_eq(pos a, pos b)
{
    return a.line == b.line && a.column == b.column;
}
LIST_DECL(pos, 5, is_pos_eq);

char* part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    size_t nb_column = string_vector[0].length - 1;

    list_pos starting_points = { .vector = NULL };
    init_list_pos(&starting_points);
    for (size_t line = 0; line < nb_line; line++) {
        for (size_t column = 0; column < nb_column; column++) {
            if (string_vector[line].text[column] == '0') {
                add_to_list_pos(&starting_points, (pos) { .line = line, .column = column });
            }
        }
    }

    uint64_t total_scores = 0;
    queue_pos position_search;
    init_queue_pos(&position_search);
    for (size_t start_id = 0; start_id < starting_points.length; start_id++) {
        empty_queue_pos(&position_search);
        push_to_queue_pos(&position_search, starting_points.vector[start_id]);

        list_pos final_points = { .vector = NULL };
        init_list_pos(&final_points);
        while (!is_queue_empty_pos(&position_search)) {
            pos current_pos = position_search.last->value;
            pop_queue_pos(&position_search);

            uint8_t current_height = string_vector[current_pos.line].text[current_pos.column] - 48;
            for (uint8_t i = 0; i < 4; i++) {
                pos next_pos = { .line = current_pos.line, .column = current_pos.column };
                apply_direction(&next_pos, i % 4);

                if (next_pos.line >= 0 && next_pos.column >= 0 && next_pos.line < (int)nb_line && next_pos.column < (int)nb_column) {
                    uint8_t next_height = string_vector[next_pos.line].text[next_pos.column] - 48;
                    if (next_height == current_height + 1) {
                        if (next_height == 9) {
                            add_unique_to_list_pos(&final_points, next_pos);
                        } else {
                            push_to_queue_pos(&position_search, next_pos);
                        }
                    }
                }
            }
        }
        total_scores += final_points.length;
        free(final_points.vector);
    }

    free_string_table(string_vector, nb_line);
    return uint64_t_to_str(total_scores);
}

char* part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    size_t nb_column = string_vector[0].length - 1;

    list_pos starting_points = { .vector = NULL };
    init_list_pos(&starting_points);
    for (size_t line = 0; line < nb_line; line++) {
        for (size_t column = 0; column < nb_column; column++) {
            if (string_vector[line].text[column] == '0') {
                add_to_list_pos(&starting_points, (pos) { .line = line, .column = column });
            }
        }
    }

    uint64_t total_scores = 0;
    queue_pos position_search;
    init_queue_pos(&position_search);
    for (size_t start_id = 0; start_id < starting_points.length; start_id++) {
        empty_queue_pos(&position_search);
        push_to_queue_pos(&position_search, starting_points.vector[start_id]);

        list_pos final_points = { .vector = NULL };
        init_list_pos(&final_points);
        while (!is_queue_empty_pos(&position_search)) {
            pos current_pos = position_search.last->value;
            pop_queue_pos(&position_search);

            uint8_t current_height = string_vector[current_pos.line].text[current_pos.column] - 48;
            for (uint8_t i = 0; i < 4; i++) {
                pos next_pos = { .line = current_pos.line, .column = current_pos.column };
                apply_direction(&next_pos, i % 4);

                if (next_pos.line >= 0 && next_pos.column >= 0 && next_pos.line < (int)nb_line && next_pos.column < (int)nb_column) {
                    uint8_t next_height = string_vector[next_pos.line].text[next_pos.column] - 48;
                    if (next_height == current_height + 1) {
                        if (next_height == 9) {
							total_scores++;
                        } else {
                            push_to_queue_pos(&position_search, next_pos);
                        }
                    }
                }
            }
        }
        total_scores += final_points.length;
        free(final_points.vector);
    }

    free_string_table(string_vector, nb_line);
    return uint64_t_to_str(total_scores);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "0123\n1234\n8765\n9876\n";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), "1"));

    char test_input_2[] = "...0...\n...1...\n...2...\n6543456\n7.....7\n8.....8\n9.....9\n";
    cr_assert(eq(str, part_1(test_input_2, sizeof(test_input_2)), "2"));

    char test_input_3[] = "..90..9\n...1.98\n...2..7\n6543456\n765.987\n876....\n987....\n";
    cr_assert(eq(str, part_1(test_input_3, sizeof(test_input_3)), "4"));

    char test_input_4[] = "89010123\n78121874\n87430965\n96549874\n45678903\n32019012\n01329801\n10456732\n";
    cr_assert(eq(str, part_1(test_input_4, sizeof(test_input_4)), "36"));
}

Test(aoc, part_2)
{
    char test_input_1[] = ".....0.\n..4321.\n..5..2.\n..6543.\n..7..4.\n..8765.\n..9....\n";
    cr_assert(eq(str, part_2(test_input_1, sizeof(test_input_1)), "3"));

    char test_input_2[] = "..90..9\n...1.98\n...2..7\n6543456\n765.987\n876....\n987....\n";
    cr_assert(eq(str, part_2(test_input_2, sizeof(test_input_2)), "13"));

    char test_input_3[] = "012345\n123456\n234567\n345678\n4.6789\n56789.\n";
    cr_assert(eq(str, part_2(test_input_3, sizeof(test_input_3)), "227"));

    char test_input_4[] = "89010123\n78121874\n87430965\n96549874\n45678903\n32019012\n01329801\n10456732\n";
    cr_assert(eq(str, part_2(test_input_4, sizeof(test_input_4)), "81"));
}
#endif
