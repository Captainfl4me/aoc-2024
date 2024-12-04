#include "include/day.h"
#include "include/utils.h"
#include <string.h>

int part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    char xmas_text[] = "XMAS";
    uint8_t valid_world[9] = { 0 };
    uint16_t find_word = 0;
    for (int line = 0; line < (int)nb_line; line++) {
        for (int column = 0; column < (int)string_vector[line].length; column++) {
            if (string_vector[line].text[column] == xmas_text[0]) {
                memset(valid_world, 1, 9);
                valid_world[4] = 0;
                for (int size = 1; size <= 3; size++) {
                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++) {
                            if (i == 0 && j == 0) {
                                continue;
                            }

                            if (valid_world[(i + 1) * 3 + j + 1]) {
                                int current_line = line + i * size;
                                int current_column = column + j * size;
                                if (current_line >= 0 && current_line < (int)nb_line && current_column >= 0 && current_column < (int)string_vector[line].length) {
                                    valid_world[(i + 1) * 3 + j + 1] = string_vector[current_line].text[current_column] == xmas_text[size];
                                } else {
                                    valid_world[(i + 1) * 3 + j + 1] = 0;
                                }
                            }
                        }
                    }
                }

                for (int i = 0; i < 9; i++) {
                    find_word += valid_world[i];
                }
            }
        }
    }

    return find_word;
}

int part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);
    uint16_t find_word = 0;
    for (int line = 1; line < (int)nb_line-1; line++) {
        for (int column = 1; column < (int)string_vector[line].length-1; column++) {
            if (string_vector[line].text[column] == 'A') {
                int corners[4] = { string_vector[line - 1].text[column - 1],
                    string_vector[line - 1].text[column + 1],
                    string_vector[line + 1].text[column + 1],
                    string_vector[line + 1].text[column - 1] };

				for (int i = 0; i < 4; i++) {
					if (corners[i] == 'M' && corners[(i+1)%4] == 'M' && corners[(i+2)%4] == 'S' && corners[(i+3)%4] == 'S') {
						find_word++;
					}
				}
			}
        }
    }

    return find_word;
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX\n";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 18));
}

Test(aoc, part_2)
{
    char test_input[] = "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX\n";
    cr_assert(eq(int, part_2(test_input, sizeof(test_input)), 9));
}
#endif
