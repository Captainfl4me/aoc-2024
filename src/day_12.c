#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>

QUEUE_DECL(pos);

uint64_t part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    size_t nb_column = string_vector[0].length - 1;

    uint8_t already_mapped[nb_line][nb_column];
    memset(already_mapped, 0, nb_line * nb_column);
    char current_region = '\0';
    uint64_t total_cost = 0;
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < nb_column; j++) {
            if (already_mapped[i][j] == 1) {
                continue;
            }

            queue_pos queue;
            init_queue_pos(&queue);
            current_region = string_vector[i].text[j];
            uint32_t area = 0;
            uint32_t perimeter = 0;
            push_to_queue_pos(&queue, (pos) { .line = i, .column = j });
            while (!is_queue_empty_pos(&queue)) {
                pos current_pos = queue.last->value;
                pop_queue_pos(&queue);

                if (already_mapped[current_pos.line][current_pos.column] == 1) {
                    continue;
                }
                area++;
                already_mapped[current_pos.line][current_pos.column] = 1;

                for (int i = 0; i < 4; i++) {
                    pos next_pos = current_pos;
                    direction dir = i % 4;
                    apply_direction(&next_pos, dir);

                    if (next_pos.line < 0 || next_pos.column < 0 || (size_t)next_pos.line >= nb_line || (size_t)next_pos.column >= nb_column) {
                        perimeter++;
                    } else if (string_vector[next_pos.line].text[next_pos.column] == current_region) {
                        if (already_mapped[next_pos.line][next_pos.column] == 0) {
                            push_to_queue_pos(&queue, next_pos);
                        }
                    } else {
                        perimeter++;
                    }
                }
            }
            total_cost += area * perimeter;
        }
    }

    free_string_table(string_vector, nb_line);
    return total_cost;
}

LIST_DECL(pos, 5, pos_eq);

uint8_t size_t_eq(size_t a, size_t b)
{
    return a == b;
}
LIST_DECL(size_t, 5, size_t_eq);

uint64_t part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    size_t nb_column = string_vector[0].length - 1;

    uint8_t already_mapped[nb_line][nb_column];
    memset(already_mapped, 0, nb_line * nb_column);
    char current_region = '\0';
    uint64_t total_cost = 0;
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < nb_column; j++) {
            if (already_mapped[i][j] == 1) {
                continue;
            }

            queue_pos queue;
            init_queue_pos(&queue);
            push_to_queue_pos(&queue, (pos) { .line = i, .column = j });

            list_pos edges[4] = { { .vector = NULL }, { .vector = NULL }, { .vector = NULL }, { .vector = NULL } };
            for (size_t k = 0; k < 4; k++) {
                init_list_pos(&edges[k]);
            }

            current_region = string_vector[i].text[j];
            uint32_t area = 0;
            while (!is_queue_empty_pos(&queue)) {
                pos current_pos = queue.last->value;
                pop_queue_pos(&queue);

                if (already_mapped[current_pos.line][current_pos.column] == 1) {
                    continue;
                }
                area++;
                already_mapped[current_pos.line][current_pos.column] = 1;

                for (int k = 0; k < 4; k++) {
                    pos next_pos = current_pos;
                    direction dir = k % 4;
                    apply_direction(&next_pos, dir);

                    if (next_pos.line < 0 || next_pos.column < 0 || (size_t)next_pos.line >= nb_line || (size_t)next_pos.column >= nb_column) {
                        add_unique_to_list_pos(&edges[dir], next_pos);
                    } else if (string_vector[next_pos.line].text[next_pos.column] == current_region) {
                        if (already_mapped[next_pos.line][next_pos.column] == 0) {
                            push_to_queue_pos(&queue, next_pos);
                        }
                    } else {
                        add_unique_to_list_pos(&edges[dir], next_pos);
                    }
                }
            }

            uint32_t edge_number = 0;
            for (size_t k = 0; k < 4; k++) {
                while (edges[k].length > 0) {
                    edge_number++;
                    list_pos visited_edge = { .vector = NULL };
                    init_list_pos(&visited_edge);

                    pos starting_edge = edges[k].vector[0];
                    pos current_pos = starting_edge;
                    direction dir = k % 2 == 0 ? right : up;
                    uint8_t front_is_empty = 0;
                    do {
                        add_unique_to_list_pos(&visited_edge, current_pos);
                        apply_direction(&current_pos, dir);
                        front_is_empty = !is_value_in_list_pos(&edges[k], current_pos);
                    } while (!front_is_empty);

                    dir = k % 2 == 0 ? left : down;
                    current_pos = starting_edge;
                    do {
                        add_unique_to_list_pos(&visited_edge, current_pos);
                        apply_direction(&current_pos, dir);
                        front_is_empty = !is_value_in_list_pos(&edges[k], current_pos);
                    } while (!front_is_empty);

                    for (size_t z = 0; z < visited_edge.length; z++) {
                        int idx = index_of_pos(&edges[k], visited_edge.vector[z]);
                        if (idx >= 0) {
                            delete_value_at_pos(&edges[k], idx);
                        }
                    }
                    free(visited_edge.vector);
                }
                free(edges[k].vector);
            }

            total_cost += area * edge_number;
        }
    }

    free_string_table(string_vector, nb_line);
    return total_cost;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "AAAA\nBBCD\nBBCC\nEEEC\n";
    cr_assert(eq(int, part_1(test_input_1, sizeof(test_input_1)), 140));

    char test_input_2[] = "OOOOO\nOXOXO\nOOOOO\nOXOXO\nOOOOO\n";
    cr_assert(eq(int, part_1(test_input_2, sizeof(test_input_2)), 772));

    char test_input_3[] = "RRRRIICCFF\nRRRRIICCCF\nVVRRRCCFFF\nVVRCCCJFFF\nVVVVCJJCFE\nVVIVCCJJEE\nVVIIICJJEE\nMIIIIIJJEE\nMIIISIJEEE\nMMMISSJEEE\n";
    cr_assert(eq(int, part_1(test_input_3, sizeof(test_input_3)), 1930));
}

Test(aoc, part_2)
{
    char test_input_1[] = "AAAA\nBBCD\nBBCC\nEEEC\n";
    cr_assert(eq(int, part_2(test_input_1, sizeof(test_input_1)), 80));

    char test_input_2[] = "OOOOO\nOXOXO\nOOOOO\nOXOXO\nOOOOO\n";
    cr_assert(eq(int, part_2(test_input_2, sizeof(test_input_2)), 436));

    char test_input_3[] = "AAAAAA\nAAABBA\nAAABBA\nABBAAA\nABBAAA\nAAAAAA\n";
    cr_assert(eq(int, part_2(test_input_3, sizeof(test_input_3)), 368));

    char test_input_4[] = "EEEEE\nEXXXX\nEEEEE\nEXXXX\nEEEEE\n";
    cr_assert(eq(int, part_2(test_input_4, sizeof(test_input_4)), 236));

    char test_input_5[] = "RRRRIICCFF\nRRRRIICCCF\nVVRRRCCFFF\nVVRCCCJFFF\nVVVVCJJCFE\nVVIVCCJJEE\nVVIIICJJEE\nMIIIIIJJEE\nMIIISIJEEE\nMMMISSJEEE\n";
    cr_assert(eq(int, part_2(test_input_5, sizeof(test_input_5)), 1206));

    char test_input_6[] = "WWWW\nWLLW\nWWWW\n";
    cr_assert(eq(int, part_2(test_input_6, sizeof(test_input_6)), 88));

    char test_input_7[] = "WWW\nWLW\nWLW\nWWW\n";
    cr_assert(eq(int, part_2(test_input_7, sizeof(test_input_7)), 88));
}
#endif
