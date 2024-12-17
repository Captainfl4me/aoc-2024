#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>

typedef enum case_type {
    Empty,
    Wall,
    Box,
} case_type;

uint64_t part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    size_t map_size_line = 0, map_size_column = string_vector[0].length - 1;
    for (size_t i = 0; i < nb_line; i++) {
        if (string_vector[i].length <= 1) {
            map_size_line = i;
            break;
        }
    }
    size_t nb_command_line = nb_line - map_size_line - 1, nb_command_column = string_vector[nb_line - 1].length - 1;

    case_type map[map_size_line][map_size_column];
    pos current_pos;
    for (size_t line = 0; line < map_size_line; line++) {
        for (size_t column = 0; column < map_size_column; column++) {
            if (string_vector[line].text[column] == '#') {
                map[line][column] = Wall;
            } else if (string_vector[line].text[column] == 'O') {
                map[line][column] = Box;
            } else if (string_vector[line].text[column] == '@') {
                map[line][column] = Empty;
                current_pos.line = line;
                current_pos.column = column;
            } else {
                map[line][column] = Empty;
            }
        }
    }

    size_t nb_command = nb_command_line * nb_command_column;
    direction command[nb_command_line * nb_command_column];
    for (size_t line = 0; line < nb_command_line; line++) {
        for (size_t column = 0; column < nb_command_column; column++) {
            size_t flat_index = line * nb_command_column + column;
            switch (string_vector[line + map_size_line + 1].text[column]) {
            case '^':
                command[flat_index] = up;
                break;
            case '>':
                command[flat_index] = right;
                break;
            case '<':
                command[flat_index] = left;
                break;
            case 'v':
                command[flat_index] = down;
                break;
            }
        }
    }
    free_string_table(string_vector, nb_line);

    for (size_t k = 0; k < nb_command; k++) {
        pos next_pos = current_pos;
        apply_direction(&next_pos, command[k]);

        if (map[next_pos.line][next_pos.column] == Empty) {
            current_pos = next_pos;
        } else if (map[next_pos.line][next_pos.column] == Box) {
            pos search_pos = next_pos;
            do {
                apply_direction(&search_pos, command[k]);
            } while (map[search_pos.line][search_pos.column] == Box);

            if (map[search_pos.line][search_pos.column] == Empty) {
                map[search_pos.line][search_pos.column] = Box;
                map[next_pos.line][next_pos.column] = Empty;
                current_pos = next_pos;
            }
        }
    }

    uint64_t box_gps_sum = 0;
    for (size_t line = 0; line < map_size_line; line++) {
        for (size_t column = 0; column < map_size_column; column++) {
            if (map[line][column] == Box) {
                box_gps_sum += 100 * line + column;
            }
        }
    }

    return box_gps_sum;
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
    char test_input_1[] = "########\n#..O.O.#\n##@.O..#\n#...O..#\n#.#.O..#\n#...O..#\n#......#\n########\n\n<^^>>>vv<v>>v<<\n";
    cr_assert(eq(int, part_1(test_input_1, sizeof(test_input_1)), 2028));

    char test_input_2[] = "########\n#..O.O.#\n##@.O..#\n#...O..#\n#.#.O..#\n#...O..#\n#......#\n########\n\n<^^>>>vv\n<v>>v<<<\n";
    cr_assert(eq(int, part_1(test_input_2, sizeof(test_input_2)), 2028));

    char test_input_3[] = "##########\n#..O..O.O#\n#......O.#\n#.OO..O.O#\n#..O@..O.#\n#O#..O...#\n#O..O..O.#\n#.OO.O.OO#\n#....O...#\n##########\n\n<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^\nvvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v\n><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<\n<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^\n^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><\n^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^\n>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^\n<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>\n^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>\nv^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^\n";
    cr_assert(eq(int, part_1(test_input_3, sizeof(test_input_3)), 10092));
}
#endif
