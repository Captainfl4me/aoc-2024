#include "include/day.h"
#include "include/utils.h"
#include <stdio.h>

typedef struct mem_map {
    uint64_t reg_a;
    uint64_t reg_b;
    uint64_t reg_c;

    uint64_t inst_ptr;
} mem_map;

uint64_t read_combo(mem_map* mem, uint8_t value)
{
    switch (value) {
    case 6:
        return mem->reg_c;
    case 5:
        return mem->reg_b;
    case 4:
        return mem->reg_a;
    case 0 ... 3:
        return value;
    default:
        return 0;
    }
}

uint64_t pow_int(uint64_t a, uint64_t pow)
{
    uint64_t acc = 1;
    for (size_t i = 0; i < pow; i++) {
        acc *= a;
    }
    return acc;
}

uint8_t uint8_t_eq(uint8_t a, uint8_t b)
{
    return a == b;
}
LIST_DECL(uint8_t, 5, uint8_t_eq);

void run_program(mem_map* mem, list_uint8_t* out_list, uint8_t program[], size_t program_size) {
    mem->inst_ptr = 0;
    uint8_t no_incr = 0;
    while (1) {
        if (mem->inst_ptr >= program_size) {
            break;
        }

        no_incr = 0;
        uint64_t combo = read_combo(mem, program[mem->inst_ptr + 1]);
        uint64_t literal = program[mem->inst_ptr + 1];
        switch (program[mem->inst_ptr]) {
        case 0: // adv
            mem->reg_a = mem->reg_a / (1 << combo);
            break;
        case 1: // bxl
            mem->reg_b = mem->reg_b ^ literal;
            break;
        case 2: // bst
            mem->reg_b = combo % 8;
            break;
        case 3: // jnz
            if (mem->reg_a != 0) {
                mem->inst_ptr = literal;
                no_incr = 1;
            }
            break;
        case 4: // bxc
            mem->reg_b = mem->reg_b ^ mem->reg_c;
            break;
        case 5: // out
            add_to_list_uint8_t(out_list, combo % 8);
            break;
        case 6: // bdv
            mem->reg_b = mem->reg_a / (1 << combo);
            break;
        case 7: // cdv
            mem->reg_c = mem->reg_a / (1 << combo);
            break;
        }
        if (!no_incr) {
            mem->inst_ptr += 2;
        }
    }
}

char* part_1(char* input, size_t strlen)
{
    UNUSED(strlen);
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    mem_map mem;
    mem.reg_a = atoi(string_vector[0].text + 12);
    mem.reg_b = atoi(string_vector[1].text + 12);
    mem.reg_c = atoi(string_vector[2].text + 12);

    size_t program_size = (string_vector[4].length - 9) / 2;
    uint8_t program[program_size];
    for (size_t i = 9; i < string_vector[4].length; i += 2) {
        size_t idx = (i - 9) / 2;
        program[idx] = string_vector[4].text[i] - 48;
    }
    free_string_table(string_vector, nb_line);

	list_uint8_t out_list = {.vector=NULL};
	init_list_uint8_t(&out_list);
	run_program(&mem, &out_list, program, program_size);

    if (out_list.length > 0) {
        size_t output_str_size = out_list.length * 2 - 1;
        char* output_str = (char*)malloc(sizeof(char) * (output_str_size + 1));
        for (size_t i = 0; i < out_list.length - 1; i++) {
            snprintf(output_str + i * 2, output_str_size - i * 2, "%d,", out_list.vector[i] % 8);
        }
        snprintf(output_str + (out_list.length - 1) * 2, 2, "%d", out_list.vector[out_list.length - 1] % 8);

        return output_str;
    } else {
        return "";
    }
}

char* part_2(char* input, size_t strlen)
{
    return uint64_t_to_str(0);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "Register A: 729\nRegister B: 0\nRegister C: 9\n\nProgram: 2,6\n";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), ""));

    char test_input_2[] = "Register A: 10\nRegister B: 0\nRegister C: 0\n\nProgram: 5,0,5,1,5,4\n";
    cr_assert(eq(str, part_1(test_input_2, sizeof(test_input_2)), "0,1,2"));

    char test_input_3[] = "Register A: 2024\nRegister B: 0\nRegister C: 0\n\nProgram: 0,1,5,4,3,0\n";
    cr_assert(eq(str, part_1(test_input_3, sizeof(test_input_3)), "4,2,5,6,7,7,7,7,3,1,0"));

    char test_input_4[] = "Register A: 2024\nRegister B: 0\nRegister C: 29\n\nProgram: 1,7\n";
    cr_assert(eq(str, part_1(test_input_4, sizeof(test_input_4)), ""));

    char test_input_5[] = "Register A: 2024\nRegister B: 29\nRegister C: 2024\n\nProgram: 4,0\n";
    cr_assert(eq(str, part_1(test_input_5, sizeof(test_input_5)), ""));

    char test_input_6[] = "Register A: 729\nRegister B: 2024\nRegister C: 0\n\nProgram: 0,1,5,4,3,0\n";
    cr_assert(eq(str, part_1(test_input_6, sizeof(test_input_6)), "4,6,3,5,6,3,5,2,1,0"));
}

Test(aoc, part_1_memory)
{
	list_uint8_t out_list = {.vector=NULL};
	init_list_uint8_t(&out_list);
    mem_map mem;
    mem.reg_a = 729;
    mem.reg_b = 0;
    mem.reg_c = 9;
	uint8_t program_1[] = {2, 6};
	run_program(&mem, &out_list, program_1, 2);
    cr_assert(eq(int, mem.reg_b, 1));

    mem.reg_b = 29;
	uint8_t program_2[] = {1, 7};
	run_program(&mem, &out_list, program_2, 2);
    cr_assert(eq(int, mem.reg_b, 26));

    mem.reg_b = 2024;
    mem.reg_c = 43690;
	uint8_t program_3[] = {4, 0};
	run_program(&mem, &out_list, program_3, 2);
    cr_assert(eq(int, mem.reg_b, 44354));
}
#endif
