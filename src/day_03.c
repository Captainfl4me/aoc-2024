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
    return 0;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 161));
}

Test(aoc, passing)
{
    cr_assert(1);
}
#endif
