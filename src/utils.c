#include "include/utils.h"
#include <stdint.h>
#include <string.h>

size_t get_number_of_line(char* input, size_t strlen)
{
    size_t counter = 0;
    for (size_t i = 0; i < strlen; i++) {
        if (input[i] == '\n') {
            counter++;
        }
    }

    return counter;
}

string* split_by_lines(char* input, size_t strlen, size_t* vector_length)
{
    size_t number_of_line = get_number_of_line(input, strlen);
    size_t string_vector_id = 0;
    string* string_vector = (string*)malloc(number_of_line * sizeof(string));

    char *new_pointer, *last_pointer = input;
    while ((new_pointer = strchr(last_pointer, '\n'))) {
        size_t substring_length = new_pointer - last_pointer + 1;
        string_vector[string_vector_id].length = substring_length;

        string_vector[string_vector_id].text = (char*)malloc(sizeof(char) * substring_length);
        memcpy(string_vector[string_vector_id].text, last_pointer, substring_length - 1);
        string_vector[string_vector_id].text[substring_length - 1] = '\0';

        string_vector_id++;
        if ((size_t)(new_pointer - input + 1) < strlen) {
            last_pointer = new_pointer + 1;
        } else {
            break;
        }
    }

    *vector_length = string_vector_id;
    return string_vector;
}

int compare_uint32_t(const void* a, const void* b)
{
    return (*(uint32_t*)a - *(uint32_t*)b);
}

void init_list(list* list)
{
    if (list->vector != NULL) {
        free(list->vector);
    }
    list->current_capacity = CAPACITY_STEP;
    list->vector = (LIST_TYPE*)malloc(sizeof(LIST_TYPE) * list->current_capacity);
    list->length = 0;
}
void parse_text_to_list(list* list, char* text, char sep)
{
    char* new_text = (char*)malloc(sizeof(char) * (strlen(text) + 1));
    strcpy(new_text, text);
    char *start_pointer = new_text, *next_pointer;
    while ((next_pointer = strchr(start_pointer, sep))) {
        *next_pointer = '\0';
        add_to_list(list, atoi(start_pointer));
        start_pointer = next_pointer + 1;
    }
    add_to_list(list, atoi(start_pointer));
    free(new_text);
}
void add_unique_to_list(list* list, LIST_TYPE value)
{
    for (uint32_t i = 0; i < list->length; i++) {
        if (list->vector[i] == value) {
            return;
        }
    }

    if (list->length >= list->current_capacity) {
        list->current_capacity += CAPACITY_STEP;
        list->vector = (LIST_TYPE*)realloc(list->vector, sizeof(LIST_TYPE) * list->current_capacity);
    }

    list->vector[list->length++] = value;
}
void add_to_list(list* list, LIST_TYPE value)
{
    if (list->length >= list->current_capacity) {
        list->current_capacity += CAPACITY_STEP;
        list->vector = (LIST_TYPE*)realloc(list->vector, sizeof(LIST_TYPE) * list->current_capacity);
    }

    list->vector[list->length++] = value;
}
uint8_t is_value_in_list(list* list, LIST_TYPE value)
{
    for (uint32_t k = 0; k < list->length; k++) {
        if (list->vector[k] == value) {
            return 1;
        }
    }
    return 0;
}
void insert_value_at(list* list, uint32_t index, LIST_TYPE value)
{
    if (index > list->length) {
        return;
    }

    if (list->length >= list->current_capacity) {
        list->current_capacity += CAPACITY_STEP;
        list->vector = (LIST_TYPE*)realloc(list->vector, sizeof(LIST_TYPE) * list->current_capacity);
    }

    for (uint32_t i = list->length; i > index; i--) {
        list->vector[i] = list->vector[i - 1];
    }
    list->vector[index] = value;
    list->length++;
}
