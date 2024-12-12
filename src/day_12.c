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

    return total_cost;
}

typedef struct edge {
    char dir;
    pos pos;
} edge;
uint8_t edge_eq(edge a, edge b)
{
    return ((a.pos.line == b.pos.line) && (a.pos.column == b.pos.column)) && a.dir == b.dir;
}
LIST_DECL(edge, 5, edge_eq);

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
    printf("== == ==\n");
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < nb_column; j++) {
            if (already_mapped[i][j] == 1) {
                continue;
            }

            queue_pos queue;
            init_queue_pos(&queue);
            push_to_queue_pos(&queue, (pos) { .line = i, .column = j });

            list_edge edges = { .vector = NULL };
            init_list_edge(&edges);

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

                for (int i = 0; i < 4; i++) {
                    pos next_pos = current_pos;
                    direction dir = i % 4;
                    apply_direction(&next_pos, dir);

                    if (next_pos.line < 0 || next_pos.column < 0 || (size_t)next_pos.line >= nb_line || (size_t)next_pos.column >= nb_column) {
                        char edge_dir = dir % 2 ? '|' : '-';
                        add_unique_to_list_edge(&edges, (edge) { .pos = next_pos, .dir = edge_dir });
                    } else if (string_vector[next_pos.line].text[next_pos.column] == current_region) {
                        if (already_mapped[next_pos.line][next_pos.column] == 0) {
                            push_to_queue_pos(&queue, next_pos);
                        }
                    } else {
                        char edge_dir = dir % 2 ? '|' : '-';
                        add_unique_to_list_edge(&edges, (edge) { .pos = next_pos, .dir = edge_dir });
                    }
                }
            }

            uint32_t edge_number = 0;
            while (edges.length > 0) {
                list_edge visited_edge = { .vector = NULL };
                init_list_edge(&visited_edge);

                edge current_edge = edges.vector[0];
                direction start_dir = current_edge.dir == '-' ? right : up;
                char other_dir = current_edge.dir == '-' ? '|' : '-';
                int idx;
                if ((idx = index_of_edge(&edges, (edge) { .dir = other_dir, .pos = current_edge.pos })) >= 0) {
                    current_edge = edges.vector[idx];
                    char next_dir = current_edge.dir;
                    uint8_t forward_empty = 0;
                    uint8_t loop_counter = 0;
                    do {
                        loop_counter++;
                        pos next_pos = current_edge.pos;
                        apply_direction(&next_pos, start_dir);
                        forward_empty = !is_value_in_list_edge(&edges, (edge) { .dir = next_dir, .pos = next_pos });
                        if (forward_empty) {
                            start_dir = (start_dir - 1) % 4;
                            next_dir = (start_dir % 2) ? '|' : '-';
                        }
                    } while (forward_empty && loop_counter <= 4);
                }
                direction current_dir = start_dir;
                pos start_pos = current_edge.pos;
                uint8_t has_inner_turn = 1;
                uint8_t skip_step = 0;

                printf("[%c] start_with : %d %d %d\n", current_region, current_edge.pos.line, current_edge.pos.column, current_dir);
                do {
                    int idx = -1;
                    add_unique_to_list_edge(&visited_edge, current_edge);

                    pos next_pos = current_edge.pos;
                    apply_direction(&next_pos, current_dir);
                    char other_dir = current_edge.dir == '-' ? '|' : '-';
                    add_unique_to_list_edge(&visited_edge, (edge) { .dir = other_dir, .pos = current_edge.pos });

                    pos direct_right = current_edge.pos;
                    direction right_dir = (current_dir + 1) % 4;
                    apply_direction(&direct_right, right_dir);

                    if (is_value_in_list_edge(&edges, (edge) { .dir = other_dir, .pos = direct_right })) {
                        skip_step = 0;
                        edge_number++;
                        current_edge.pos = direct_right;
                        current_edge.dir = other_dir;
                        current_dir = right_dir;
                        printf("direct right: %d %d %d\n", direct_right.line, direct_right.column, right_dir);
                    } else if (has_inner_turn == 0 && is_value_in_list_edge(&edges, (edge) { .dir = other_dir, .pos = current_edge.pos })) {
                        skip_step = 0;
                        uint8_t first_check = 0;
                        edge_number++;
                        direction new_dir = (current_dir + 1) % 4;
                        pos next_pos = current_edge.pos;
                        apply_direction(&next_pos, new_dir);

                        if ((next_pos.line >= 0 && next_pos.column >= 0 && (size_t)next_pos.line < nb_line && (size_t)next_pos.column < nb_column) && string_vector[next_pos.line].text[next_pos.column] == current_region) {
                            new_dir = current_dir;
                            do {
                                if (first_check > 0) {
                                    edge_number++;
                                }
                                first_check++;
                                printf("wrongdir | ");
                                new_dir = (new_dir - 1) % 4;
                                next_pos = current_edge.pos;
                                apply_direction(&next_pos, new_dir);
                            } while ((next_pos.line >= 0 && next_pos.column >= 0 && (size_t)next_pos.line < nb_line && (size_t)next_pos.column < nb_column) && string_vector[next_pos.line].text[next_pos.column] == current_region && first_check <= 3);
                        }

                        if (first_check <= 1) {
                            current_edge.dir = other_dir;
                        }
                        printf("inner corner: %d %d %d\n", current_edge.pos.line, current_edge.pos.column, new_dir);
                        current_dir = new_dir;
                        has_inner_turn = 1;
                    } else {
                        has_inner_turn = 0;
                        direction new_dir = (current_dir + 1) % 4;
                        pos new_pos = next_pos;
                        apply_direction(&new_pos, new_dir);

                        if ((idx = index_of_edge(&edges, (edge) { .dir = other_dir, .pos = new_pos })) < 0) {
                            if ((idx = index_of_edge(&edges, (edge) { .dir = current_edge.dir, .pos = next_pos })) >= 0) {
                                has_inner_turn = 0;
                                current_edge.pos = next_pos;
                                printf("follow: %d %d %d\n", next_pos.line, next_pos.column, current_dir);
                                continue;
                            } else {
                                new_dir = (current_dir - 1) % 4;
                                new_pos = next_pos;
                                apply_direction(&new_pos, new_dir);
                                idx = index_of_edge(&edges, (edge) { .dir = other_dir, .pos = new_pos });
                                if (idx < 0) {
                                    skip_step++;
                                    printf("skip\n");
                                    continue;
                                }
                            }
                        } else {
                            skip_step = 0;
                        }
                        edge_number++;

                        printf("corner: %d %d %d\n", new_pos.line, new_pos.column, new_dir);

                        current_edge.pos = new_pos;
                        current_edge.dir = other_dir;
                        current_dir = new_dir;
                    }
                } while (!(pos_eq(current_edge.pos, start_pos) && current_dir == start_dir) || skip_step == 1);

                for (size_t k = 0; k < visited_edge.length; k++) {
                    int idx = index_of_edge(&edges, visited_edge.vector[k]);
                    if (idx >= 0) {
                        delete_value_at_edge(&edges, idx);
                    }
                }
            }
            free(edges.vector);

            printf("$$ %d %d\n", area, edge_number);
            total_cost += area * edge_number;
        }
    }

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
