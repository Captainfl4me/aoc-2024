#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum parse_SM {
    ButtonA,
    ButtonB,
    Prize,
    Empty,
} parse_SM;

uint64_t part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    parse_SM sm = ButtonA;
    pos btn_a, btn_b, goal;
    size_t push_a, push_b;
    char *splitter, *start_pos;

    uint64_t total_cost = 0;
    for (size_t i = 0; i < nb_line; i++) {
        switch (sm) {
        case ButtonA: {
            start_pos = string_vector[i].text + 12;
            splitter = strchr(start_pos, ',');
            *splitter = '\0';
            btn_a.line = atoi(start_pos);
            btn_a.column = atoi(splitter + 4);
            sm = ButtonB;
            break;
        }
        case ButtonB: {
            start_pos = string_vector[i].text + 12;
            splitter = strchr(start_pos, ',');
            *splitter = '\0';
            btn_b.line = atoi(start_pos);
            btn_b.column = atoi(splitter + 4);
            sm = Prize;
            break;
        }
        case Prize: {
            start_pos = string_vector[i].text + 9;
            splitter = strchr(start_pos, ',');
            *splitter = '\0';
            goal.line = atoi(start_pos);
            goal.column = atoi(splitter + 4);

            int matrix_det = btn_a.line * btn_b.column - btn_a.column * btn_b.line;
            push_a = (btn_b.column * goal.line - btn_b.line * goal.column) / matrix_det;
            push_b = (-btn_a.column * goal.line + btn_a.line * goal.column) / matrix_det;

            if ((btn_a.line * push_a + btn_b.line * push_b) == (size_t)goal.line && (btn_a.column * push_a + btn_b.column * push_b) == (size_t)goal.column) {
                total_cost += push_a * 3 + push_b;
            }

            sm = Empty;
            break;
        }
        case Empty: {
            sm = ButtonA;
            break;
        }
        }
    }
    free_string_table(string_vector, nb_line);

    return total_cost;
}

typedef struct pos_long {
    int64_t line;
    int64_t column;
} pos_long;

uint64_t part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    parse_SM sm = ButtonA;
    pos_long btn_a, btn_b, goal;
    int64_t push_a, push_b;
    char *splitter, *start_pos;

    uint64_t total_cost = 0;
    for (size_t i = 0; i < nb_line; i++) {
        switch (sm) {
        case ButtonA: {
            start_pos = string_vector[i].text + 12;
            splitter = strchr(start_pos, ',');
            *splitter = '\0';
            btn_a.line = atoi(start_pos);
            btn_a.column = atoi(splitter + 4);
            sm = ButtonB;
            break;
        }
        case ButtonB: {
            start_pos = string_vector[i].text + 12;
            splitter = strchr(start_pos, ',');
            *splitter = '\0';
            btn_b.line = atoi(start_pos);
            btn_b.column = atoi(splitter + 4);
            sm = Prize;
            break;
        }
        case Prize: {
            start_pos = string_vector[i].text + 9;
            splitter = strchr(start_pos, ',');
            *splitter = '\0';
            goal.line = atoi(start_pos) + 10000000000000;
            goal.column = atoi(splitter + 4) + 10000000000000;

            int64_t matrix_det = btn_a.line * btn_b.column - btn_a.column * btn_b.line;
            push_a = (btn_b.column * goal.line - btn_b.line * goal.column) / matrix_det;
            push_b = (-btn_a.column * goal.line + btn_a.line * goal.column) / matrix_det;

            if ((btn_a.line * push_a + btn_b.line * push_b) == goal.line && (btn_a.column * push_a + btn_b.column * push_b) == goal.column) {
                total_cost += push_a * 3 + push_b;
            }

            sm = Empty;
            break;
        }
        case Empty: {
            sm = ButtonA;
            break;
        }
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
    char test_input_1[] = "Button A: X+94, Y+34\nButton B: X+22, Y+67\nPrize: X=8400, Y=5400\n\nButton A: X+26, Y+66\nButton B: X+67, Y+21\nPrize: X=12748, Y=12176\n\nButton A: X+17, Y+86\nButton B: X+84, Y+37\nPrize: X=7870, Y=6450\n\nButton A: X+69, Y+23\nButton B: X+27, Y+71\nPrize: X=18641, Y=10279\n";
    cr_assert(eq(int, part_1(test_input_1, sizeof(test_input_1)), 480));
}

Test(aoc, part_2)
{
    char test_input_1[] = "Button A: X+94, Y+34\nButton B: X+22, Y+67\nPrize: X=8400, Y=5400\n\nButton A: X+26, Y+66\nButton B: X+67, Y+21\nPrize: X=12748, Y=12176\n\nButton A: X+17, Y+86\nButton B: X+84, Y+37\nPrize: X=7870, Y=6450\n\nButton A: X+69, Y+23\nButton B: X+27, Y+71\nPrize: X=18641, Y=10279\n";
    cr_assert(eq(long, part_2(test_input_1, sizeof(test_input_1)), 875318608908));
}
#endif
