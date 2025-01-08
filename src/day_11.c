#include "include/day.h"
#include "include/utils.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define HASHMAP_SIZE 2024
typedef struct stones {
    uint64_t type_of_stone;
    uint64_t number_of_stones;
} stones;
size_t stone_hash(stones stones)
{
    return (size_t)stones.type_of_stone;
}
uint8_t stone_eq(stones a, stones b)
{
    return a.type_of_stone == b.type_of_stone;
}
HASHMAP_DECL(stones, HASHMAP_SIZE, stone_hash, stone_eq);

void append_new_stone_to_hashmap(hashmap_stones* map, stones stone)
{
    stones* search_ptr = NULL;
    if ((search_ptr = get_value_ptr_stones(map, (stones) { .type_of_stone = stone.type_of_stone }))) {
        search_ptr->number_of_stones += stone.number_of_stones;
    } else {
        add_value_unique_key_stones(map, (stones) { .type_of_stone = stone.type_of_stone, .number_of_stones = stone.number_of_stones });
    }
}

uint64_t int_pow(uint64_t num, size_t pow)
{
    uint64_t res = 1;
    for (size_t i = 0; i < pow; i++) {
        res *= num;
    }
    return res;
}

char* part_1(char* input, size_t strlen)
{
    char* cp_input = (char*)malloc(sizeof(char) * (strlen + 1));
    strcpy(cp_input, input);
    hashmap_stones stones_map;
    init_hashmap_stones(&stones_map);

    char *start_pointer = cp_input, *next_pointer;
    uint8_t end_match = 0;
    while (!end_match) {
        next_pointer = strchr(start_pointer, ' ');
        if (next_pointer != NULL) {
            *next_pointer = '\0';
        } else {
            end_match = 1;
        }
        uint64_t type_of_stone = atoi(start_pointer);
        stones* stone_ptr = NULL;
        if ((stone_ptr = get_value_ptr_stones(&stones_map, (stones) { .type_of_stone = type_of_stone }))) {
            stone_ptr->number_of_stones++;
        } else {
            add_value_unique_key_stones(&stones_map, (stones) { .type_of_stone = type_of_stone, .number_of_stones = 1 });
        }
        start_pointer = next_pointer + 1;
    }
    free(cp_input);

    for (size_t i = 0; i < 25; i++) {
        hashmap_stones new_stones_map;
        init_hashmap_stones(&new_stones_map);

        for (size_t j = 0; j < HASHMAP_SIZE; j++) {
            queue_stones_wrapper* current_val_ptr = stones_map.table[j].first;
            if (stones_map.table[j].queue_length > 0) {
            }
            while (current_val_ptr != NULL) {
                stones* stone = &current_val_ptr->value;
                uint64_t stone_size = log10(stone->type_of_stone) + 1.0;
                if (stone->type_of_stone == 0) {
                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = 1, .number_of_stones = stone->number_of_stones });
                } else if ((stone_size & 1) == 0) {
                    uint64_t left_stone = stone->type_of_stone / int_pow(10, stone_size / 2);
                    uint64_t right_stone = stone->type_of_stone - (left_stone * int_pow(10, stone_size / 2));

                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = left_stone, .number_of_stones = stone->number_of_stones });
                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = right_stone, .number_of_stones = stone->number_of_stones });
                } else {
                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = (stone->type_of_stone * 2024), .number_of_stones = stone->number_of_stones });
                }

                current_val_ptr = current_val_ptr->next_in_list;
            }
        }

        free_hashmap_stones(&stones_map);
        stones_map = new_stones_map;
    }

    uint64_t total_stones = 0;
    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        queue_stones_wrapper* current_val_ptr = stones_map.table[i].first;
        while (current_val_ptr != NULL) {
            total_stones += current_val_ptr->value.number_of_stones;
            current_val_ptr = current_val_ptr->next_in_list;
        }
    }

    return uint64_t_to_str(total_stones);
}

char* part_2(char* input, size_t strlen)
{
    char* cp_input = (char*)malloc(sizeof(char) * (strlen + 1));
    strcpy(cp_input, input);
    hashmap_stones stones_map;
    init_hashmap_stones(&stones_map);

    char *start_pointer = cp_input, *next_pointer;
    uint8_t end_match = 0;
    while (!end_match) {
        next_pointer = strchr(start_pointer, ' ');
        if (next_pointer != NULL) {
            *next_pointer = '\0';
        } else {
            end_match = 1;
        }
        uint64_t type_of_stone = atoi(start_pointer);
        stones* stone_ptr = NULL;
        if ((stone_ptr = get_value_ptr_stones(&stones_map, (stones) { .type_of_stone = type_of_stone }))) {
            stone_ptr->number_of_stones++;
        } else {
            add_value_unique_key_stones(&stones_map, (stones) { .type_of_stone = type_of_stone, .number_of_stones = 1 });
        }
        start_pointer = next_pointer + 1;
    }
    free(cp_input);

    for (size_t i = 0; i < 75; i++) {
        hashmap_stones new_stones_map;
        init_hashmap_stones(&new_stones_map);

        for (size_t j = 0; j < HASHMAP_SIZE; j++) {
            queue_stones_wrapper* current_val_ptr = stones_map.table[j].first;
            if (stones_map.table[j].queue_length > 0) {
            }
            while (current_val_ptr != NULL) {
                stones* stone = &current_val_ptr->value;
                uint64_t stone_size = log10(stone->type_of_stone) + 1;
                if (stone->type_of_stone == 0) {
                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = 1, .number_of_stones = stone->number_of_stones });
                } else if ((stone_size & 1) == 0) {
                    uint64_t left_stone = stone->type_of_stone / (pow(10, (double)stone_size / 2));
                    uint64_t right_stone = stone->type_of_stone - (uint64_t)(left_stone * pow(10, (double)stone_size / 2));

                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = left_stone, .number_of_stones = stone->number_of_stones });
                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = right_stone, .number_of_stones = stone->number_of_stones });
                } else {
                    append_new_stone_to_hashmap(&new_stones_map, (stones) { .type_of_stone = stone->type_of_stone * 2024, .number_of_stones = stone->number_of_stones });
                }

                current_val_ptr = current_val_ptr->next_in_list;
            }
        }

        free_hashmap_stones(&stones_map);
        stones_map = new_stones_map;
    }

    uint64_t total_stones = 0;

    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        queue_stones_wrapper* current_val_ptr = stones_map.table[i].first;
        while (current_val_ptr != NULL) {
            total_stones += current_val_ptr->value.number_of_stones;
            current_val_ptr = current_val_ptr->next_in_list;
        }
    }

    return uint64_t_to_str(total_stones);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input_1[] = "125 17\n";
    cr_assert(eq(str, part_1(test_input_1, sizeof(test_input_1)), "55312"));
}

Test(aoc, part_2)
{
    char test_input_1[] = "125 17\n";
    cr_assert(eq(str, part_2(test_input_1, sizeof(test_input_1)), "65601038650482"));

    char test_input_2[] = "0\n";
    cr_assert(eq(str, part_2(test_input_2, sizeof(test_input_2)), "22938365706844"));
}
#endif
