#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

#define CAPACITY_STEP 5

typedef struct list {
    uint8_t* vector;
    uint8_t length;
    uint8_t current_capacity;
} list;

void init_list(list* list)
{
    if (list->vector != NULL) {
        free(list->vector);
    }
    list->current_capacity = CAPACITY_STEP;
    list->vector = (uint8_t*)malloc(sizeof(uint8_t) * list->current_capacity);
    list->length = 0;
}

void add_unique_to_list(list* list, uint8_t value)
{
    for (int i = 0; i < list->length; i++) {
        if (list->vector[i] == value) {
            return;
        }
    }

    if (list->length >= list->current_capacity) {
        list->current_capacity += CAPACITY_STEP;
        list->vector = (uint8_t*)realloc(list->vector, sizeof(uint8_t) * list->current_capacity);
    }

    list->vector[list->length++] = value;
}
uint8_t is_value_in_list(list* list, uint8_t value)
{
    for (int k = 0; k < list->length; k++) {
        if (list->vector[k] == value) {
            return 1;
        }
    }
    return 0;
}

typedef enum orientation {
    up,
    right,
    down,
    left
} orientation;

void apply_orientation_to_coord(size_t* pos_line, size_t* pos_column, orientation orientation)
{
    switch (orientation) {
    case up:
        (*pos_line)--;
        break;
    case right:
        (*pos_column)++;
        break;
    case down:
        (*pos_line)++;
        break;
    case left:
        (*pos_column)--;
        break;
    }
}

int part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    char* start_char_pointer;
    size_t initial_pos_line = 0, initial_pos_column = 0;
    for (size_t line = 0; line < nb_line; line++) {
        if ((start_char_pointer = strchr(string_vector[line].text, '^'))) {
            initial_pos_line = line;
            initial_pos_column = start_char_pointer - string_vector[line].text;
            break;
        }
    }

    uint8_t already_visit_grid[nb_line][string_vector[0].length];
    memset(already_visit_grid, 0, nb_line * string_vector[0].length);

    uint32_t total_visited = 0;
    size_t current_pos_line = initial_pos_line, current_pos_column = initial_pos_column;
    orientation current_orientation = up;
    size_t next_pos_line, next_pos_column;
    while (current_pos_line < nb_line && current_pos_column < string_vector[0].length) {
        if (already_visit_grid[current_pos_line][current_pos_column] == 0) {
            total_visited++;
            already_visit_grid[current_pos_line][current_pos_column] = 1;
        }
        uint8_t is_next_valid = 1;
        do {
            next_pos_line = current_pos_line;
            next_pos_column = current_pos_column;
            apply_orientation_to_coord(&next_pos_line, &next_pos_column, current_orientation);
            if (next_pos_line < nb_line && next_pos_column < string_vector[0].length) {
                is_next_valid = string_vector[next_pos_line].text[next_pos_column] != '#';
                if (!is_next_valid) {
                    current_orientation = (current_orientation + 1) % 4;
                }
            }
        } while (!is_next_valid);
        current_pos_line = next_pos_line;
        current_pos_column = next_pos_column;
    }

    free(string_vector);

    return total_visited;
}

int part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    char* start_char_pointer;
    size_t initial_pos_line = 0, initial_pos_column = 0;
    for (size_t line = 0; line < nb_line; line++) {
        if ((start_char_pointer = strchr(string_vector[line].text, '^'))) {
            initial_pos_line = line;
            initial_pos_column = start_char_pointer - string_vector[line].text;
            break;
        }
    }

    uint8_t obstacle_found[nb_line][string_vector[0].length];
    memset(obstacle_found, 0, nb_line * string_vector[0].length);
    uint8_t path_until_here[nb_line][string_vector[0].length];
    memset(path_until_here, 0, nb_line * string_vector[0].length);

    list already_visit_grid[nb_line][string_vector[0].length];
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < string_vector[0].length; j++) {
            already_visit_grid[i][j].length = 0;
            already_visit_grid[i][j].current_capacity = 0;
            already_visit_grid[i][j].vector = NULL;
        }
    }

    uint32_t total_positions = 0;
    size_t current_pos_line = initial_pos_line, current_pos_column = initial_pos_column;
    orientation current_orientation = up;
    orientation saved_orientation = up;
    size_t next_pos_line, next_pos_column;
    while (current_pos_line < nb_line && current_pos_column < string_vector[0].length) {
        // reset look-up table
        for (size_t i = 0; i < nb_line; i++) {
            for (size_t j = 0; j < string_vector[0].length; j++) {
                if (already_visit_grid[i][j].vector != NULL) {
                    free(already_visit_grid[i][j].vector);
                    already_visit_grid[i][j].vector = NULL;
                    already_visit_grid[i][j].length = 0;
                    already_visit_grid[i][j].current_capacity = 0;
                }
            }
        }
		path_until_here[current_pos_line][current_pos_column] = 1;

        // check if next is blocked and turn until free way ahead
        uint8_t next_is_valid = 1;
        do {
            next_pos_line = current_pos_line;
            next_pos_column = current_pos_column;
            apply_orientation_to_coord(&next_pos_line, &next_pos_column, current_orientation);
            if (next_pos_line < nb_line && next_pos_column < string_vector[0].length) {
                next_is_valid = string_vector[next_pos_line].text[next_pos_column] != '#';
                if (!next_is_valid) {
                    current_orientation = (current_orientation + 1) % 4;
                }
            } else {
                next_is_valid = 1;
            }
        } while (!next_is_valid);

        if (next_pos_line < nb_line && next_pos_column < string_vector[0].length) {
            // Try putting obstacle ahead
            saved_orientation = current_orientation;
            size_t temp_wall_line = next_pos_line, temp_wall_column = next_pos_column;

            if (obstacle_found[temp_wall_line][temp_wall_column] == 0 && path_until_here[temp_wall_line][temp_wall_column] == 0) {
                string_vector[temp_wall_line].text[temp_wall_column] = '#';
                do {
                    if (already_visit_grid[current_pos_line][current_pos_column].vector == NULL) {
                        init_list(&already_visit_grid[current_pos_line][current_pos_column]);
                    }
                    add_unique_to_list(&already_visit_grid[current_pos_line][current_pos_column], current_orientation);

                    next_pos_line = current_pos_line;
                    next_pos_column = current_pos_column;
                    apply_orientation_to_coord(&next_pos_line, &next_pos_column, current_orientation);

                    while (next_pos_line < nb_line && next_pos_column < string_vector[0].length && string_vector[next_pos_line].text[next_pos_column] == '#') {
                        next_pos_line = current_pos_line;
                        next_pos_column = current_pos_column;
                        current_orientation = (current_orientation + 1) % 4;
                        apply_orientation_to_coord(&next_pos_line, &next_pos_column, current_orientation);
                    }

                    current_pos_line = next_pos_line;
                    current_pos_column = next_pos_column;

                    if (current_pos_line < nb_line && current_pos_column < string_vector[0].length && is_value_in_list(&already_visit_grid[current_pos_line][current_pos_column], current_orientation)) {
                        obstacle_found[temp_wall_line][temp_wall_column] = 1;
                        total_positions++;
                        break;
                    }
                } while (next_pos_line < nb_line && next_pos_column < string_vector[0].length);

                string_vector[temp_wall_line].text[temp_wall_column] = '0';
            }
            current_pos_line = temp_wall_line;
            current_pos_column = temp_wall_column;
            current_orientation = saved_orientation;
        } else {
            break;
        }
    }

    free(string_vector);

    return total_positions;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// Test(aoc, part_1)
// {
//     char test_input[] = "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n........#.\n#.........\n......#...\n";
//     cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 41));
// }

Test(aoc, part_2)
{
    char test_input[] = "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n........#.\n#.........\n......#...\n";
    cr_assert(eq(int, part_2(test_input, sizeof(test_input)), 6));
}

Test(aoc, part_1_opt)
{
    char test_input[] = ".#................\n..#...............\n..................\n.^................\n..................\n";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 4));
}
#endif
