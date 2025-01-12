#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

#ifdef TEST
#define GRID_SIZE 7
#define STEP_NUM 12
#else
#define GRID_SIZE 71
#define STEP_NUM 1024
#endif

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

char* part_1(char* input, size_t strlen)
{
    UNUSED(strlen);
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    uint8_t map[GRID_SIZE][GRID_SIZE] = { { 0 } };
    for (size_t i = 0; i < STEP_NUM; i++) {
        char* sep = strchr(string_vector[i].text, ',');
        *sep = '\0';
        uint8_t line = atoi(sep + 1);
        uint8_t column = atoi(string_vector[i].text);
        map[line][column] = 1;
    }
    free_string_table(string_vector, nb_line);

    uint64_t visited_map[GRID_SIZE * GRID_SIZE * 4];
    for (size_t k = 0; k < GRID_SIZE * GRID_SIZE * 4; k++) {
        visited_map[k] = -1;
    }

    priority_queue_state heap;
    init_priority_queue_state(&heap);
    push_to_priority_queue_state(&heap, (state) { .pos = { .line = 0, .column = 0 }, .dir = right, .weight = 0 });
    push_to_priority_queue_state(&heap, (state) { .pos = { .line = 0, .column = 0 }, .dir = down, .weight = 0 });
    while (!is_priority_queue_empty_state(&heap)) {
        state current_state = heap.last->value;
        pop_priority_queue_state(&heap);

        size_t visited_index = current_state.pos.line * GRID_SIZE * 4 + current_state.pos.column * 4 + current_state.dir;
        if (current_state.weight >= visited_map[visited_index]) {
            continue;
        }
        visited_map[visited_index] = current_state.weight;

        for (int8_t dir_offset = -1; dir_offset <= 1; dir_offset++) {
            pos next_pos = current_state.pos;
            direction new_dir = (current_state.dir + dir_offset) % 4;
            apply_direction(&next_pos, new_dir);
            if (next_pos.line >= 0 && next_pos.line < GRID_SIZE && next_pos.column >= 0 && next_pos.column < GRID_SIZE) {
                if (map[next_pos.line][next_pos.column] == 0) {
                    size_t next_visited_index = next_pos.line * GRID_SIZE * 4 + next_pos.column * 4 + new_dir;
                    uint64_t new_weight = current_state.weight + 1;
                    if (new_weight < visited_map[next_visited_index]) {
                        push_to_priority_queue_state(&heap, (state) { .pos = next_pos, .dir = new_dir, .weight = new_weight });
                    }
                }
            }
        }
    }

    uint64_t min_path_cost = -1;
    for (uint8_t k = 0; k < 4; k++) {
        size_t visited_index = (GRID_SIZE - 1) * GRID_SIZE * 4 + (GRID_SIZE - 1) * 4 + k;
        if (visited_map[visited_index] < min_path_cost) {
            min_path_cost = visited_map[visited_index];
        }
    }

    return uint64_t_to_str(min_path_cost);
}

char* part_2(char* input, size_t strlen)
{
    UNUSED(strlen);
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    uint8_t fallen_bytes[nb_line][2];
    for (size_t i = 0; i < nb_line; i++) {
        char* sep = strchr(string_vector[i].text, ',');
        *sep = '\0';
        fallen_bytes[i][0] = atoi(sep + 1);
        fallen_bytes[i][1] = atoi(string_vector[i].text);
		*sep = ',';
    }

    uint8_t map[GRID_SIZE][GRID_SIZE] = { { 0 } };
    uint64_t visited_map[GRID_SIZE * GRID_SIZE * 4];

    uint16_t step_to_search_min = STEP_NUM;
    uint16_t step_to_search_max = nb_line;
    uint16_t step_to_search = (step_to_search_max - step_to_search_min) / 2 + step_to_search_min;
    uint8_t end_reachable = 1;
    while (step_to_search_max - step_to_search_min > 2) {
        for (size_t line = 0; line < GRID_SIZE; line++) {
            for (size_t column = 0; column < GRID_SIZE; column++) {
                map[line][column] = 0;
            }
        }
        for (size_t i = 0; i < step_to_search; i++) {
            map[fallen_bytes[i][0]][fallen_bytes[i][1]] = 1;
        }

        for (size_t k = 0; k < GRID_SIZE * GRID_SIZE * 4; k++) {
            visited_map[k] = -1;
        }

        priority_queue_state heap;
        init_priority_queue_state(&heap);
        push_to_priority_queue_state(&heap, (state) { .pos = { .line = 0, .column = 0 }, .dir = right, .weight = 0 });
        push_to_priority_queue_state(&heap, (state) { .pos = { .line = 0, .column = 0 }, .dir = down, .weight = 0 });
        while (!is_priority_queue_empty_state(&heap)) {
            state current_state = heap.last->value;
            pop_priority_queue_state(&heap);

            size_t visited_index = current_state.pos.line * GRID_SIZE * 4 + current_state.pos.column * 4 + current_state.dir;
            if (current_state.weight >= visited_map[visited_index]) {
                continue;
            }
            visited_map[visited_index] = current_state.weight;

            for (int8_t dir_offset = -1; dir_offset <= 1; dir_offset++) {
                pos next_pos = current_state.pos;
                direction new_dir = (current_state.dir + dir_offset) % 4;
                apply_direction(&next_pos, new_dir);
                if (next_pos.line >= 0 && next_pos.line < GRID_SIZE && next_pos.column >= 0 && next_pos.column < GRID_SIZE) {
                    if (map[next_pos.line][next_pos.column] == 0) {
                        size_t next_visited_index = next_pos.line * GRID_SIZE * 4 + next_pos.column * 4 + new_dir;
                        uint64_t new_weight = current_state.weight + 1;
                        if (new_weight < visited_map[next_visited_index]) {
                            push_to_priority_queue_state(&heap, (state) { .pos = next_pos, .dir = new_dir, .weight = new_weight });
                        }
                    }
                }
            }
        }

        uint64_t min_path_cost = -1;
        uint64_t no_path_cost = -1;
        for (uint8_t k = 0; k < 4; k++) {
            size_t visited_index = (GRID_SIZE - 1) * GRID_SIZE * 4 + (GRID_SIZE - 1) * 4 + k;
            if (visited_map[visited_index] < min_path_cost) {
                min_path_cost = visited_map[visited_index];
            }
        }
        step_to_search++;
        end_reachable = min_path_cost != no_path_cost;
		if (end_reachable) {
			step_to_search_min = step_to_search;
		} else {
			step_to_search_max = step_to_search;
		}
    }

    return string_vector[step_to_search-2].text;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "5,4\n4,2\n4,5\n3,0\n2,1\n6,3\n2,4\n1,5\n0,6\n3,3\n2,6\n5,1\n1,2\n5,5\n2,5\n6,5\n1,4\n0,4\n6,4\n1,1\n6,1\n1,0\n0,5\n1,6\n2,0\n";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), "22"));
}

Test(aoc, part_2)
{
    char test_input_1[] = "5,4\n4,2\n4,5\n3,0\n2,1\n6,3\n2,4\n1,5\n0,6\n3,3\n2,6\n5,1\n1,2\n5,5\n2,5\n6,5\n1,4\n0,4\n6,4\n1,1\n6,1\n1,0\n0,5\n1,6\n2,0\n";
    cr_assert(eq(str, part_2(test_input_1, sizeof(test_input_1)), "6,1"));
}
#endif
