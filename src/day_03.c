#include "include/day.h"
#include "include/utils.h"
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int part_1(char* input, size_t strlen)
{
    UNUSED(strlen);
    regex_t mul_regex;
    int function_result;

    function_result = regcomp(&mul_regex, "mul\\(([[:digit:]]{1,3}),([[:digit:]]{1,3})\\)", REG_EXTENDED);
    if (function_result) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    size_t nmatch = mul_regex.re_nsub;
    regmatch_t pmatch[nmatch + 1];
    size_t input_offset = 0;
    char read_number[4];
    uint8_t read_number_size = 0;
    uint16_t number_couple[2] = { 0 };
    uint32_t result = 0;
    do {
        function_result = regexec(&mul_regex, input + input_offset, nmatch + 1, pmatch, 0);
        if (!function_result) {
            for (int i = 0; i < 2; i++) {
                read_number_size = pmatch[i + 1].rm_eo - pmatch[i + 1].rm_so;
                strncpy(read_number, input + input_offset + pmatch[i + 1].rm_so, read_number_size);
                read_number[read_number_size] = '\0';
                number_couple[i] = atoi(read_number);
            }
            result += number_couple[0] * number_couple[1];
            input_offset += pmatch[0].rm_eo;
        }
    } while (function_result != REG_NOMATCH);

    regfree(&mul_regex);
    return result;
}

int part_2(char* input, size_t strlen)
{
    UNUSED(strlen);
    regex_t mul_regex, do_regex, dont_regex;
    int function_result;

    function_result = regcomp(&mul_regex, "mul\\(([[:digit:]]{1,3}),([[:digit:]]{1,3})\\)", REG_EXTENDED);
    if (function_result) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    function_result = regcomp(&do_regex, "do\\(\\)", REG_EXTENDED);
    if (function_result) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    function_result = regcomp(&dont_regex, "don\'t\\(\\)", REG_EXTENDED);
    if (function_result) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

	size_t mul_pmatch_size = mul_regex.re_nsub + 1;
    regmatch_t mul_pmatch[mul_pmatch_size];
	size_t do_pmatch_size = do_regex.re_nsub + 1;
    regmatch_t do_pmatch[do_pmatch_size];
    int16_t next_dont = -1;
    size_t input_offset = 0;
    char read_number[4];
    uint8_t read_number_size = 0;
    uint16_t number_couple[2] = { 0 };
    uint32_t result = 0;
    do {
        function_result = regexec(&dont_regex, input + input_offset, do_pmatch_size, do_pmatch, 0);
        if (!function_result) {
            next_dont = do_pmatch[0].rm_eo;
        } else {
            next_dont = -1;
        }

        function_result = regexec(&mul_regex, input + input_offset, mul_pmatch_size, mul_pmatch, 0);
        if (function_result != REG_NOMATCH && (next_dont < 0 || mul_pmatch[0].rm_so < next_dont)) {
            for (int i = 0; i < 2; i++) {
                read_number_size = mul_pmatch[i + 1].rm_eo - mul_pmatch[i + 1].rm_so;
                strncpy(read_number, input + input_offset + mul_pmatch[i + 1].rm_so, read_number_size);
                read_number[read_number_size] = '\0';
                number_couple[i] = atoi(read_number);
            }
            result += number_couple[0] * number_couple[1];
            input_offset += mul_pmatch[0].rm_eo;
        } else if (next_dont > 0) {
            input_offset += next_dont;
            function_result = regexec(&do_regex, input + input_offset, do_pmatch_size, do_pmatch, 0);
            if (!function_result) {
                input_offset += do_pmatch[0].rm_eo;
            }
        }
    } while (function_result != REG_NOMATCH);

    regfree(&mul_regex);
    return result;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 161));
}

Test(aoc, part_2)
{
    char test_input[] = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
    cr_assert(eq(int, part_2(test_input, sizeof(test_input)), 48));
}
#endif
