#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

typedef struct state {
    pos pos;
    direction dir;
    uint64_t weight;
} state;
uint8_t order_state(state a, state b)
{
    return a.weight < b.weight;
}
PRIORITY_QUEUE_DECL(state, order_state);

uint64_t part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    size_t nb_column = string_vector[0].length - 1;

    pos start_pos = { 0 }, end_pos = { 0 };
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < nb_column; j++) {
            if (string_vector[i].text[j] == 'S') {
                start_pos.line = i;
                start_pos.column = j;
            } else if (string_vector[i].text[j] == 'E') {
                end_pos.line = i;
                end_pos.column = j;
            }
        }
    }

    uint64_t visited_map[nb_line * nb_column * 4];
    for (size_t k = 0; k < nb_line * nb_column * 4; k++) {
        visited_map[k] = -1;
    }

    priority_queue_state heap;
    init_priority_queue_state(&heap);
    push_to_priority_queue_state(&heap, (state) { .pos = start_pos, .dir = right, .weight = 0 });
    while (!is_priority_queue_empty_state(&heap)) {
        state current_state = heap.last->value;
        pop_priority_queue_state(&heap);

        size_t visited_index = current_state.pos.line * nb_column * 4 + current_state.pos.column * 4 + current_state.dir;
        if (current_state.weight >= visited_map[visited_index]) {
            continue;
        }
        visited_map[visited_index] = current_state.weight;

        for (int8_t dir_offset = -1; dir_offset <= 1; dir_offset++) {
            pos next_pos = current_state.pos;
            direction new_dir = (current_state.dir + dir_offset) % 4;
            apply_direction(&next_pos, new_dir);
            if (string_vector[next_pos.line].text[next_pos.column] != '#') {
                size_t next_visited_index = next_pos.line * nb_column * 4 + next_pos.column * 4 + new_dir;
                uint64_t new_weight = current_state.weight + 1 + (dir_offset == 0 ? 0 : 1000);
                if (new_weight < visited_map[next_visited_index]) {
                    push_to_priority_queue_state(&heap, (state) { .pos = next_pos, .dir = new_dir, .weight = new_weight });
                }
            }
        }
    }

    uint64_t min_path_cost = -1;
    for (uint8_t k = 0; k < 4; k++) {
        size_t visited_index = end_pos.line * nb_column * 4 + end_pos.column * 4 + k;
        if (visited_map[visited_index] < min_path_cost) {
            min_path_cost = visited_map[visited_index];
        }
    }

    return min_path_cost;
}

typedef struct state_with_history {
    pos pos;
    direction dir;
    uint64_t weight;
    pos* history;
    size_t history_size;
} state_with_history;
uint8_t order_state_with_history(state_with_history a, state_with_history b)
{
    return a.weight < b.weight;
}
PRIORITY_QUEUE_DECL(state_with_history, order_state_with_history);
uint8_t state_with_history_eq(state_with_history a, state_with_history b)
{
    return pos_eq(a.pos, b.pos) && a.weight == b.weight && a.dir == b.dir;
}
LIST_DECL(state_with_history, 5, state_with_history_eq);

uint64_t part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    size_t nb_column = string_vector[0].length - 1;

    pos start_pos = { 0 }, end_pos = { 0 };
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < nb_column; j++) {
            if (string_vector[i].text[j] == 'S') {
                start_pos.line = i;
                start_pos.column = j;
            } else if (string_vector[i].text[j] == 'E') {
                end_pos.line = i;
                end_pos.column = j;
            }
        }
    }

    uint64_t visited_map[nb_line * nb_column * 4];
    for (size_t k = 0; k < nb_line * nb_column * 4; k++) {
        visited_map[k] = -1;
    }

    list_state_with_history end_state = { .vector = NULL };
    init_list_state_with_history(&end_state);

    priority_queue_state_with_history heap;
    init_priority_queue_state_with_history(&heap);
    push_to_priority_queue_state_with_history(&heap, (state_with_history) { .pos = start_pos, .dir = right, .weight = 0, .history = NULL, .history_size = 0 });
    while (!is_priority_queue_empty_state_with_history(&heap)) {
        state_with_history current_state = heap.last->value;
        pop_priority_queue_state_with_history(&heap);

        size_t visited_index = current_state.pos.line * nb_column * 4 + current_state.pos.column * 4 + current_state.dir;
        if (current_state.weight > visited_map[visited_index]) {
            continue;
        }
        visited_map[visited_index] = current_state.weight;

        for (int8_t dir_offset = -1; dir_offset <= 1; dir_offset++) {
            pos next_pos = current_state.pos;
            direction new_dir = (current_state.dir + dir_offset) % 4;
            apply_direction(&next_pos, new_dir);
            if (string_vector[next_pos.line].text[next_pos.column] != '#') {
                size_t next_visited_index = next_pos.line * nb_column * 4 + next_pos.column * 4 + new_dir;
                uint64_t new_weight = current_state.weight + 1 + (dir_offset == 0 ? 0 : 1000);
                if (new_weight <= visited_map[next_visited_index]) {
                    pos* new_history = (pos*)malloc(sizeof(pos) * (current_state.history_size + 1));
                    if (current_state.history != NULL) {
                        memcpy(new_history, current_state.history, current_state.history_size * sizeof(pos));
                    }
                    new_history[current_state.history_size] = next_pos;
                    push_to_priority_queue_state_with_history(&heap, (state_with_history) { .pos = next_pos, .dir = new_dir, .weight = new_weight, .history = new_history, .history_size = (current_state.history_size + 1) });
                }
            }
        }

        if (pos_eq(current_state.pos, end_pos)) {
            add_to_list_state_with_history(&end_state, current_state);
        } else {
            free(current_state.history);
        }
    }

    uint64_t min_path_cost = -1;
    for (size_t k = 0; k < end_state.length; k++) {
        if (end_state.vector[k].weight < min_path_cost) {
            min_path_cost = end_state.vector[k].weight;
        }
    }

    uint8_t sits_map[nb_line * nb_column];
    for (size_t k = 0; k < nb_line * nb_column; k++) {
        sits_map[k] = 0;
    }

    for (size_t k = 0; k < end_state.length; k++) {
		if (end_state.vector[k].weight > min_path_cost) {
			continue;
		}
        for (size_t i = 0; i < end_state.vector[k].history_size; i++) {
            size_t flat_index = end_state.vector[k].history[i].line * nb_column + end_state.vector[k].history[i].column;
            sits_map[flat_index] = 1;
        }
    }

    uint64_t total_sits = 1;
    for (size_t k = 0; k < nb_line * nb_column; k++) {
        total_sits += sits_map[k];
    }

    return total_sits;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "###############\n#.......#....E#\n#.#.###.#.###.#\n#.....#.#...#.#\n#.###.#####.#.#\n#.#.#.......#.#\n#.#.#####.###.#\n#...........#.#\n###.#.#####.#.#\n#...#.....#.#.#\n#.#.#.###.#.#.#\n#.....#...#.#.#\n#.###.#.#.#.#.#\n#S..#.....#...#\n###############\n";
    cr_assert(eq(ulong, part_1(test_input_1, sizeof(test_input_1)), 7036));

    char test_input_2[] = "#################\n#...#...#...#..E#\n#.#.#.#.#.#.#.#.#\n#.#.#.#...#...#.#\n#.#.#.#.###.#.#.#\n#...#.#.#.....#.#\n#.#.#.#.#.#####.#\n#.#...#.#.#.....#\n#.#.#####.#.###.#\n#.#.#.......#...#\n#.#.###.#####.###\n#.#.#...#.....#.#\n#.#.#.#####.###.#\n#.#.#.........#.#\n#.#.#.#########.#\n#S#.............#\n#################\n";
    cr_assert(eq(ulong, part_1(test_input_2, sizeof(test_input_2)), 11048));
}

Test(aoc, part_2)
{
    char test_input_1[] = "###############\n#.......#....E#\n#.#.###.#.###.#\n#.....#.#...#.#\n#.###.#####.#.#\n#.#.#.......#.#\n#.#.#####.###.#\n#...........#.#\n###.#.#####.#.#\n#...#.....#.#.#\n#.#.#.###.#.#.#\n#.....#...#.#.#\n#.###.#.#.#.#.#\n#S..#.....#...#\n###############\n";
    cr_assert(eq(ulong, part_2(test_input_1, sizeof(test_input_1)), 45));

    char test_input_2[] = "#################\n#...#...#...#..E#\n#.#.#.#.#.#.#.#.#\n#.#.#.#...#...#.#\n#.#.#.#.###.#.#.#\n#...#.#.#.....#.#\n#.#.#.#.#.#####.#\n#.#...#.#.#.....#\n#.#.#####.#.###.#\n#.#.#.......#...#\n#.#.###.#####.###\n#.#.#...#.....#.#\n#.#.#.#####.###.#\n#.#.#.........#.#\n#.#.#.#########.#\n#S#.............#\n#################\n";
    cr_assert(eq(ulong, part_2(test_input_2, sizeof(test_input_2)), 64));
}
#endif
