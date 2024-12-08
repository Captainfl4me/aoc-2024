#include "include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>

uint8_t is_pos_eq(pos a, pos b)
{
    return a.line == b.line && a.column == b.column;
}
LIST_DECL(pos, 5, is_pos_eq);

typedef struct list_antenna {
    list_pos list;
    char type;
} list_antenna;

uint8_t is_list_antenna_eq(list_antenna a, list_antenna b)
{
    return a.type == b.type;
}
LIST_DECL(list_antenna, 5, is_list_antenna_eq);

int find_type_index(list_list_antenna* list, char type)
{
    for (size_t i = 0; i < list->length; i++) {
        if (list->vector[i].type == type) {
            return i;
        }
    }

    return -1;
}

uint64_t part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    list_list_antenna antenna_list;
    antenna_list.vector = NULL;
    init_list_list_antenna(&antenna_list);
    for (size_t line = 0; line < nb_line; line++) {
        for (size_t column = 0; column < string_vector[line].length - 1; column++) {
            char current_type = string_vector[line].text[column];
            if (current_type != '.') {
                int current_type_index;
                if ((current_type_index = find_type_index(&antenna_list, current_type)) < 0) {
                    add_to_list_list_antenna(&antenna_list, (list_antenna) { .type = current_type, .list = { .vector = NULL } });
                    init_list_pos(&antenna_list.vector[antenna_list.length - 1].list);
                    current_type_index = antenna_list.length - 1;
                }

                add_to_list_pos(&antenna_list.vector[current_type_index].list, (pos) { .line = line, .column = column });
            }
        }
    }
    size_t nb_column = string_vector[0].length-1;
    free(string_vector);

    uint64_t total_antinodes = 0;
    uint8_t antinode_map[nb_line][nb_column];
    memset(antinode_map, 0, sizeof(uint8_t) * nb_line * nb_column);
    for (size_t type_index = 0; type_index < antenna_list.length; type_index++) {
        for (size_t i = 0; i < antenna_list.vector[type_index].list.length - 1; i++) {
            pos current_point = antenna_list.vector[type_index].list.vector[i];
            for (size_t j = i + 1; j < antenna_list.vector[type_index].list.length; j++) {
                pos second_point = antenna_list.vector[type_index].list.vector[j];

                pos diff = { .line = (second_point.line - current_point.line), .column = (second_point.column - current_point.column) };
                pos antinodes[2] = { { .line = (second_point.line + diff.line), .column = (second_point.column + diff.column) }, { .line = (current_point.line - diff.line), .column = (current_point.column - diff.column) } };
                for (uint8_t antinode_idx = 0; antinode_idx < 2; antinode_idx++) {
                    if (antinodes[antinode_idx].line < (int)nb_line && antinodes[antinode_idx].column < (int)nb_column && antinodes[antinode_idx].line >= 0 && antinodes[antinode_idx].column >= 0) {
                        if (antinode_map[antinodes[antinode_idx].line][antinodes[antinode_idx].column] == 0) {
                            total_antinodes++;
                            antinode_map[antinodes[antinode_idx].line][antinodes[antinode_idx].column] = 1;
                        }
                    }
                }
            }
        }
    }

    return total_antinodes;
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
    char test_input[] = "............\n........0...\n.....0......\n.......0....\n....0.......\n......A.....\n............\n............\n........A...\n.........A..\n............\n............\n";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 14));
}
#endif
