#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

void apply_direction_to_coord(size_t* pos_line, size_t* pos_column, direction direction)
{
    switch (direction) {
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

char* part_1(char* input, size_t strlen)
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
    direction current_direction = up;
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
            apply_direction_to_coord(&next_pos_line, &next_pos_column, current_direction);
            if (next_pos_line < nb_line && next_pos_column < string_vector[0].length) {
                is_next_valid = string_vector[next_pos_line].text[next_pos_column] != '#';
                if (!is_next_valid) {
                    current_direction = (current_direction + 1) % 4;
                }
            }
        } while (!is_next_valid);
        current_pos_line = next_pos_line;
        current_pos_column = next_pos_column;
    }

    free_string_table(string_vector, nb_line);

    return uint64_t_to_str(total_visited);
}

BUILT_IN_CMP(uint8_t);
LIST_DECL(uint8_t, 5, cmp_uint8_t);

char* part_2(char* input, size_t strlen)
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

    list_uint8_t already_visit_grid[nb_line][string_vector[0].length];
    for (size_t i = 0; i < nb_line; i++) {
        for (size_t j = 0; j < string_vector[0].length; j++) {
            already_visit_grid[i][j].length = 0;
            already_visit_grid[i][j].current_capacity = 0;
            already_visit_grid[i][j].vector = NULL;
        }
    }

    uint32_t total_positions = 0;
    size_t current_pos_line = initial_pos_line, current_pos_column = initial_pos_column;
    direction current_direction = up;
    direction saved_direction = up;
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
            apply_direction_to_coord(&next_pos_line, &next_pos_column, current_direction);
            if (next_pos_line < nb_line && next_pos_column < string_vector[0].length) {
                next_is_valid = string_vector[next_pos_line].text[next_pos_column] != '#';
                if (!next_is_valid) {
                    current_direction = (current_direction + 1) % 4;
                }
            } else {
                next_is_valid = 1;
            }
        } while (!next_is_valid);

        if (next_pos_line < nb_line && next_pos_column < string_vector[0].length) {
            // Try putting obstacle ahead
            saved_direction = current_direction;
            size_t temp_wall_line = next_pos_line, temp_wall_column = next_pos_column;

            if (obstacle_found[temp_wall_line][temp_wall_column] == 0 && path_until_here[temp_wall_line][temp_wall_column] == 0) {
                string_vector[temp_wall_line].text[temp_wall_column] = '#';
                do {
                    if (already_visit_grid[current_pos_line][current_pos_column].vector == NULL) {
                        init_list_uint8_t(&already_visit_grid[current_pos_line][current_pos_column]);
                    }
                    add_unique_to_list_uint8_t(&already_visit_grid[current_pos_line][current_pos_column], current_direction);

                    next_pos_line = current_pos_line;
                    next_pos_column = current_pos_column;
                    apply_direction_to_coord(&next_pos_line, &next_pos_column, current_direction);

                    while (next_pos_line < nb_line && next_pos_column < string_vector[0].length && string_vector[next_pos_line].text[next_pos_column] == '#') {
                        next_pos_line = current_pos_line;
                        next_pos_column = current_pos_column;
                        current_direction = (current_direction + 1) % 4;
                        apply_direction_to_coord(&next_pos_line, &next_pos_column, current_direction);
                    }

                    current_pos_line = next_pos_line;
                    current_pos_column = next_pos_column;

                    if (current_pos_line < nb_line && current_pos_column < string_vector[0].length && is_value_in_list_uint8_t(&already_visit_grid[current_pos_line][current_pos_column], current_direction)) {
                        obstacle_found[temp_wall_line][temp_wall_column] = 1;
                        total_positions++;
                        break;
                    }
                } while (next_pos_line < nb_line && next_pos_column < string_vector[0].length);

                string_vector[temp_wall_line].text[temp_wall_column] = '0';
            }
            current_pos_line = temp_wall_line;
            current_pos_column = temp_wall_column;
            current_direction = saved_direction;
        } else {
            break;
        }
    }

    free_string_table(string_vector, nb_line);

    return uint64_t_to_str(total_positions);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n........#.\n#.........\n......#...\n";
    cr_assert(eq(str, part_1(test_input, sizeof(test_input)), "41"));
}

Test(aoc, part_2)
{
    char test_input[] = "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n........#.\n#.........\n......#...\n";
    cr_assert(eq(str, part_2(test_input, sizeof(test_input)), "6"));
}

Test(aoc, part_1_opt)
{
    char test_input[] = ".#................\n..#...............\n..................\n.^................\n..................\n";
    cr_assert(eq(str, part_1(test_input, sizeof(test_input)), "4"));
}
#endif
