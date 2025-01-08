#include "include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>

typedef struct file_block {
    size_t id;
    size_t length;
    size_t free_space;
} file_block;

uint8_t is_file_eq(file_block a, file_block b)
{
    UNUSED(a);
    UNUSED(b);
    return 0;
}

LIST_DECL(file_block, 5, is_file_eq);

char* part_1(char* input, size_t strlen)
{
    list_file_block filesystem = { .vector = NULL };
    uint8_t first_set = 0;
    size_t first_with_free_space = 0;
    init_list_file_block(&filesystem);
    for (size_t file_idx = 0; file_idx < strlen / 2; file_idx++) {
        size_t length = input[file_idx * 2] - 48;
        size_t free_space = input[file_idx * 2 + 1] - 48;
        add_to_list_file_block(&filesystem, (file_block) { .id = file_idx, .length = length, .free_space = free_space });
        if (free_space > 0 && !first_set) {
            first_set = 1;
            first_with_free_space = filesystem.length - 1;
        }
    }

    while (first_with_free_space != filesystem.length - 1) {
        size_t blocks_to_move = filesystem.vector[first_with_free_space].free_space < filesystem.vector[filesystem.length - 1].length ? filesystem.vector[first_with_free_space].free_space : filesystem.vector[filesystem.length - 1].length;

        insert_value_at_file_block(&filesystem, first_with_free_space + 1, (file_block) { .id = filesystem.vector[filesystem.length - 1].id, .length = blocks_to_move, .free_space = (filesystem.vector[first_with_free_space].free_space - blocks_to_move) });

        filesystem.vector[first_with_free_space].free_space = 0;
        filesystem.vector[filesystem.length - 1].length -= blocks_to_move;
        if (filesystem.vector[filesystem.length - 1].length == 0) {
            delete_last_value_file_block(&filesystem);
        }
        while (filesystem.vector[first_with_free_space].free_space == 0 && first_with_free_space < filesystem.length) {
            first_with_free_space++;
        }
    }

    uint64_t checksum = 0;
    size_t index = 0;
    for (size_t i = 0; i < filesystem.length; i++) {
        for (size_t j = 0; j < filesystem.vector[i].length; j++) {
            // printf("%zu", filesystem.vector[i].id);
            checksum += filesystem.vector[i].id * index++;
        }
    }

    return uint64_t_to_str(checksum);
}

char* part_2(char* input, size_t strlen)
{
    list_file_block filesystem = { .vector = NULL };
    init_list_file_block(&filesystem);
    for (size_t file_idx = 0; file_idx < strlen / 2; file_idx++) {
        size_t length = input[file_idx * 2] - 48;
        size_t free_space = input[file_idx * 2 + 1] - 48;
        add_to_list_file_block(&filesystem, (file_block) { .id = file_idx, .length = length, .free_space = free_space });
    }

    size_t file_to_move_id = filesystem.vector[filesystem.length - 1].id;
	size_t file_to_move_index = filesystem.length-1;
    while (file_to_move_id > 0) {
		while (filesystem.vector[file_to_move_index].id != file_to_move_id) {
			file_to_move_index--;
		}
        size_t first_with_free_space = 0;
        uint8_t has_match = 0;
        for (int i = file_to_move_index - 1; i >= 0; i--) {
            if (filesystem.vector[i].free_space >= filesystem.vector[file_to_move_index].length) {
                has_match = 1;
                first_with_free_space = i;
            }
        }
        if (has_match == 1) {
            size_t blocks_to_move = filesystem.vector[file_to_move_index].length;
            size_t free_space_to_keep = filesystem.vector[file_to_move_index].free_space;

            insert_value_at_file_block(&filesystem, first_with_free_space + 1, (file_block) { .id = filesystem.vector[file_to_move_index].id, .length = blocks_to_move, .free_space = (filesystem.vector[first_with_free_space].free_space - blocks_to_move) });

            filesystem.vector[first_with_free_space].free_space = 0;
            filesystem.vector[file_to_move_index].free_space += blocks_to_move + free_space_to_keep;
            delete_value_at_file_block(&filesystem, file_to_move_index + 1);
        }
        file_to_move_id--;
    }

    filesystem.vector[filesystem.length - 1].free_space = 0;
    uint64_t checksum = 0;
    size_t index = 0;
    for (size_t i = 0; i < filesystem.length; i++) {
        for (size_t j = 0; j < filesystem.vector[i].length; j++) {
            checksum += filesystem.vector[i].id * index++;
        }
        index += filesystem.vector[i].free_space;
    }

    return uint64_t_to_str(checksum);
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "2333133121414131402";
    cr_assert(eq(str, part_1(test_input, sizeof(test_input)), "1928"));
}

Test(aoc, part_2)
{
    char test_input[] = "2333133121414131402";
    cr_assert(eq(str, part_2(test_input, sizeof(test_input)), "2858"));
}
#endif
