#include "./include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef AOC_DAY
#define AOC_DAY 0
#endif

#define SEC_TO_NS(sec) ((sec) * 1000000000)
uint64_t nanos()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    uint64_t ns = SEC_TO_NS((uint64_t)ts.tv_sec) + (uint64_t)ts.tv_nsec;
    return ns;
}

int main(int argc, char ** argv)
{
	UNUSED(argc); UNUSED(argv);
    printf("!! AOC 2024 day %d !!\r\n", AOC_DAY);

    FILE* fptr;
    // Create file path
    size_t path_size = snprintf(NULL, 0, "./aoc-2024-inputs/day-%02d/input.txt", AOC_DAY);
    char* input_path = (char*)malloc(sizeof(char) * (path_size + 1));
    snprintf(input_path, path_size + 1, "./aoc-2024-inputs/day-%02d/input.txt",
        AOC_DAY);
    printf("Fetching input from: %s\r\n", input_path);

    fptr = fopen(input_path, "r");
	free(input_path);
    if (fptr != NULL) {
        if (fseek(fptr, 0L, SEEK_END) == 0) {
            long input_size = ftell(fptr);
            if (input_size == -1) {
                fputs("ERR: Error reading file\r\n", stderr);
                return 1;
            }

            char* input_text = (char*)malloc(sizeof(char) * (input_size + 1));
            if (fseek(fptr, 0L, SEEK_SET) != 0) {
                return 1;
            }

            size_t read_length = fread(input_text, sizeof(char), input_size, fptr);
            if (ferror(fptr) != 0) {
                fputs("ERR: Error reading file\r\n", stderr);
                return 1;
            }
            input_text[read_length++] = '\0';

            uint64_t start_time = nanos();
            uint64_t result = part_1(input_text, read_length);
            uint64_t end_time = nanos();
			printf("[part_1] result = %lu\r\n", result);
			printf("[part_1] time = %lu us\r\n\n", (end_time - start_time)/1000);

            start_time = nanos();
            result = part_2(input_text, read_length);
            end_time = nanos();
			printf("[part_2] result = %lu\r\n", result);
			printf("[part_2] time = %lu us\r\n", (end_time - start_time)/1000);

			fclose(fptr);
        } else {
			fputs("ERR: FSEEK Error\r\n", stderr);
			return 1;
		}
	} else {
		fputs("ERR: Input file not found!\r\n", stderr);
		return 1;
	}

    return 0;
}
