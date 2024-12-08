#include "include/day.h"
#include "include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

BUILT_IN_CMP(uint8_t);
LIST_DECL(uint8_t, 5, cmp_uint8_t);

uint8_t valid_update_middle_page(uint8_t* update_pages_list, uint8_t update_pages_list_length, list_uint8_t reverse_page_order[99]);

uint64_t part_1(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    list_uint8_t page_order[99] = { 0 };
    list_uint8_t reverse_page_order[99] = { 0 };
    uint8_t first_section = 1;
    uint8_t max_update_pages = 0;
    uint8_t number_of_updates = 0;
    uint8_t** pages_update = NULL;
    for (size_t i = 0; i < nb_line; i++) {
        if (first_section == 1) {
            if (string_vector[i].length == 1) {
                first_section = 0;
                for (size_t j = i + 1; j < nb_line; j++) {
                    uint8_t current_pages = 1;
                    for (size_t k = 0; k < string_vector[j].length; k++) {
                        if (string_vector[j].text[k] == ',') {
                            current_pages++;
                        }
                    }

                    if (current_pages > max_update_pages) {
                        max_update_pages = current_pages;
                    }
                }
                number_of_updates = nb_line - i - 1;
                pages_update = (uint8_t**)malloc(sizeof(uint8_t*) * number_of_updates);
                continue;
            }
            string_vector[i].text[2] = '\0';
            uint8_t num_1 = atoi(string_vector[i].text);
            uint8_t num_2 = atoi(string_vector[i].text + 3);

            if (page_order[num_1].vector == NULL) {
                init_list_uint8_t(&page_order[num_1]);
            }
            add_unique_to_list_uint8_t(&page_order[num_1], num_2);
            if (reverse_page_order[num_2].vector == NULL) {
                init_list_uint8_t(&reverse_page_order[num_2]);
            }
            add_unique_to_list_uint8_t(&reverse_page_order[num_2], num_1);
        } else {
            uint8_t pages_relative_idx = i - (nb_line - number_of_updates);
            pages_update[pages_relative_idx] = (uint8_t*)malloc(sizeof(uint8_t) * max_update_pages);
            memset(pages_update[pages_relative_idx], 0, max_update_pages);

            uint8_t update_idx = 0;
            char* start_char = string_vector[i].text;
            char* next_pointer;
            while ((next_pointer = strchr(start_char, ','))) {
                *next_pointer = '\0';
                pages_update[pages_relative_idx][update_idx++] = atoi(start_char);
                start_char = next_pointer + 1;
            }
            pages_update[pages_relative_idx][update_idx++] = atoi(start_char);
        }
    }
    free(string_vector);

    uint32_t total_middle_page = 0;
    for (uint8_t update_idx = 0; update_idx < number_of_updates; update_idx++) {
        total_middle_page += valid_update_middle_page(pages_update[update_idx], max_update_pages, reverse_page_order);
        free(pages_update[update_idx]);
    }
    free(pages_update);
    return total_middle_page;
}

uint64_t part_2(char* input, size_t strlen)
{
    size_t nb_line = 0;
    string* string_vector = split_by_lines(input, strlen, &nb_line);

    list_uint8_t page_order[99] = { 0 };
    list_uint8_t reverse_page_order[99] = { 0 };
    uint8_t first_section = 1;
    uint8_t max_update_pages = 0;
    uint8_t number_of_updates = 0;
    uint8_t** pages_update = NULL;
    for (size_t i = 0; i < nb_line; i++) {
        if (first_section == 1) {
            if (string_vector[i].length == 1) {
                first_section = 0;
                for (size_t j = i + 1; j < nb_line; j++) {
                    uint8_t current_pages = 1;
                    for (size_t k = 0; k < string_vector[j].length; k++) {
                        if (string_vector[j].text[k] == ',') {
                            current_pages++;
                        }
                    }

                    if (current_pages > max_update_pages) {
                        max_update_pages = current_pages;
                    }
                }
                number_of_updates = nb_line - i - 1;
                pages_update = (uint8_t**)malloc(sizeof(uint8_t*) * number_of_updates);
                continue;
            }
            string_vector[i].text[2] = '\0';
            uint8_t num_1 = atoi(string_vector[i].text);
            uint8_t num_2 = atoi(string_vector[i].text + 3);

            if (page_order[num_1].vector == NULL) {
                init_list_uint8_t(&page_order[num_1]);
            }
            add_unique_to_list_uint8_t(&page_order[num_1], num_2);
            if (reverse_page_order[num_2].vector == NULL) {
                init_list_uint8_t(&reverse_page_order[num_2]);
            }
            add_unique_to_list_uint8_t(&reverse_page_order[num_2], num_1);
        } else {
            uint8_t pages_relative_idx = i - (nb_line - number_of_updates);
            pages_update[pages_relative_idx] = (uint8_t*)malloc(sizeof(uint8_t) * max_update_pages);
            memset(pages_update[pages_relative_idx], 0, max_update_pages);

            uint8_t update_idx = 0;
            char* start_char = string_vector[i].text;
            char* next_pointer;
            while ((next_pointer = strchr(start_char, ','))) {
                *next_pointer = '\0';
                pages_update[pages_relative_idx][update_idx++] = atoi(start_char);
                start_char = next_pointer + 1;
            }
            pages_update[pages_relative_idx][update_idx++] = atoi(start_char);
        }
    }
    free(string_vector);

    uint32_t total_middle_page = 0;
    for (uint8_t update_idx = 0; update_idx < number_of_updates; update_idx++) {
        if (valid_update_middle_page(pages_update[update_idx], max_update_pages, reverse_page_order) == 0) {
            list_uint8_t new_order_list = { .vector = NULL, .length = 0, .current_capacity = 0 };
            init_list_uint8_t(&new_order_list);

            uint8_t next_page_idx = 0;
            while (pages_update[update_idx][next_page_idx] != 0 && next_page_idx < max_update_pages) {
                uint8_t index_to_insert = 0;
                for (index_to_insert = 0; index_to_insert < new_order_list.length; index_to_insert++) {
                    if (is_value_in_list_uint8_t(&page_order[pages_update[update_idx][next_page_idx]], new_order_list.vector[index_to_insert])) {
                        break;
                    }
                }
                insert_value_at_uint8_t(&new_order_list, index_to_insert, pages_update[update_idx][next_page_idx]);
                next_page_idx++;
            }
            total_middle_page += new_order_list.vector[new_order_list.length / 2];
        }
        free(pages_update[update_idx]);
    }
    free(pages_update);
    return total_middle_page;
}

uint8_t valid_update_middle_page(uint8_t* update_pages_list, uint8_t update_pages_list_length, list_uint8_t reverse_page_order[99])
{
    uint8_t page_idx = 0;
    uint8_t is_update_valid = 1;
    while (update_pages_list[page_idx] != 0 && page_idx < update_pages_list_length) {
        uint8_t next_page_idx = page_idx + 1;
        uint8_t is_number_valid = 1;
        while (update_pages_list[next_page_idx] != 0 && next_page_idx < update_pages_list_length) {
            if (is_value_in_list_uint8_t(&reverse_page_order[update_pages_list[page_idx]], update_pages_list[next_page_idx])) {
                is_number_valid = 0;
            }
            next_page_idx++;
        }
        if (!is_number_valid) {
            is_update_valid = 0;
        }
        page_idx++;
    }

    if (is_update_valid) {
        return update_pages_list[page_idx / 2];
    } else {
        return 0;
    }
}

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(aoc, part_1)
{
    char test_input[] = "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n\n75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47\n";
    cr_assert(eq(int, part_1(test_input, sizeof(test_input)), 143));
}

Test(aoc, part_2)
{
    char test_input[] = "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n\n75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47\n";
    cr_assert(eq(int, part_2(test_input, sizeof(test_input)), 123));
}
#endif
