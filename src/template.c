#include "include/day.h"
#include "include/utils.h"

char* part_1(char* input, size_t strlen) {
	return uint64_t_to_str(0);
}

char* part_2(char* input, size_t strlen) {
	return uint64_t_to_str(0);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), "0"));
}

Test(aoc, part_2)
{
    char test_input_1[] = "";
    cr_assert(eq(str, part_2(test_input_1, sizeof(test_input_1)), "0"));
}
#endif
