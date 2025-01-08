#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

#ifdef TEST
#define GRID_WIDTH 11
#define GRID_HEIGHT 7
#else
#define GRID_WIDTH 101
#define GRID_HEIGHT 103
#endif

typedef struct pos_vel {
    pos pos;
    pos vel;
} pos_vel;

char* part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    pos_vel pos_vel;
    char *start_pointer, *next_pointer;
    uint64_t quartile_number[4] = { 0 }, safety_factor = 1;
    for (size_t i = 0; i < nb_line; i++) {
        start_pointer = string_vector[i].text + 2;
        next_pointer = strchr(start_pointer, ',');
        *next_pointer = '\0';
        pos_vel.pos.column = atoi(start_pointer);
        start_pointer = next_pointer + 1;

        next_pointer = strchr(start_pointer, ' ');
        *next_pointer = '\0';
        pos_vel.pos.line = atoi(start_pointer);
        start_pointer = next_pointer + 3;

        next_pointer = strchr(start_pointer, ',');
        *next_pointer = '\0';
        pos_vel.vel.column = atoi(start_pointer);
        start_pointer = next_pointer + 1;
        pos_vel.vel.line = atoi(start_pointer);

        pos final_pos = { .line = ((pos_vel.pos.line + 100 * pos_vel.vel.line) % GRID_HEIGHT), .column = ((pos_vel.pos.column + 100 * pos_vel.vel.column) % GRID_WIDTH) };
        if (final_pos.line < 0) {
            final_pos.line += GRID_HEIGHT;
        }
        if (final_pos.column < 0) {
            final_pos.column += GRID_WIDTH;
        }

        if (final_pos.line < (GRID_HEIGHT - 1) / 2 && final_pos.column < (GRID_WIDTH - 1) / 2) {
            quartile_number[0]++;
        } else if (final_pos.line < (GRID_HEIGHT - 1) / 2 && final_pos.column > (GRID_WIDTH - 1) / 2) {
            quartile_number[1]++;
        } else if (final_pos.line > (GRID_HEIGHT - 1) / 2 && final_pos.column < (GRID_WIDTH - 1) / 2) {
            quartile_number[2]++;
        } else if (final_pos.line > (GRID_HEIGHT - 1) / 2 && final_pos.column > (GRID_WIDTH - 1) / 2) {
            quartile_number[3]++;
        }
    }
    free_string_table(string_vector, nb_line);

    for (uint8_t i = 0; i < 4; i++) {
        safety_factor *= quartile_number[i];
    }

    return uint64_t_to_str(safety_factor);
}

char* part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    pos_vel pos_vel[nb_line];
    char *start_pointer, *next_pointer;
    for (size_t i = 0; i < nb_line; i++) {
        start_pointer = string_vector[i].text + 2;
        next_pointer = strchr(start_pointer, ',');
        *next_pointer = '\0';
        pos_vel[i].pos.column = atoi(start_pointer);
        start_pointer = next_pointer + 1;

        next_pointer = strchr(start_pointer, ' ');
        *next_pointer = '\0';
        pos_vel[i].pos.line = atoi(start_pointer);
        start_pointer = next_pointer + 3;

        next_pointer = strchr(start_pointer, ',');
        *next_pointer = '\0';
        pos_vel[i].vel.column = atoi(start_pointer);
        start_pointer = next_pointer + 1;
        pos_vel[i].vel.line = atoi(start_pointer);
    }
    free_string_table(string_vector, nb_line);

    uint64_t min_entropy = -1, step_k = 0;
    for (size_t k = 0; k < 10000; k++) {
        uint64_t quartile_number[4] = { 0 }, entropy = 1;
        for (size_t i = 0; i < nb_line; i++) {
            pos_vel[i].pos.line = ((pos_vel[i].pos.line + pos_vel[i].vel.line) % GRID_HEIGHT);
            pos_vel[i].pos.column = ((pos_vel[i].pos.column + pos_vel[i].vel.column) % GRID_WIDTH);
            if (pos_vel[i].pos.line < 0) {
                pos_vel[i].pos.line += GRID_HEIGHT;
            }
            if (pos_vel[i].pos.column < 0) {
                pos_vel[i].pos.column += GRID_WIDTH;
            }

            if (pos_vel[i].pos.line < (GRID_HEIGHT - 1) / 2 && pos_vel[i].pos.column < (GRID_WIDTH - 1) / 2) {
                quartile_number[0]++;
            } else if (pos_vel[i].pos.line < (GRID_HEIGHT - 1) / 2 && pos_vel[i].pos.column > (GRID_WIDTH - 1) / 2) {
                quartile_number[1]++;
            } else if (pos_vel[i].pos.line > (GRID_HEIGHT - 1) / 2 && pos_vel[i].pos.column < (GRID_WIDTH - 1) / 2) {
                quartile_number[2]++;
            } else if (pos_vel[i].pos.line > (GRID_HEIGHT - 1) / 2 && pos_vel[i].pos.column > (GRID_WIDTH - 1) / 2) {
                quartile_number[3]++;
            }
        }

        for (uint8_t i = 0; i < 4; i++) {
            entropy *= quartile_number[i];
        }

        if (min_entropy > entropy) {
            min_entropy = entropy;
            step_k = k;
        }
    }

    return uint64_t_to_str(step_k + 1);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "p=0,4 v=3,-3\np=6,3 v=-1,-3\np=10,3 v=-1,2\np=2,0 v=2,-1\np=0,0 v=1,3\np=3,0 v=-2,-2\np=7,6 v=-1,-3\np=3,0 v=-1,-2\np=9,3 v=2,3\np=7,3 v=-1,2\np=2,4 v=2,-3\np=9,5 v=-3,-3\n";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), "12"));
}
#endif
