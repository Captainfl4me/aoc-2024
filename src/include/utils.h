#ifndef H_UTILS
#define H_UTILS
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) (void)(x)

// STRING
typedef struct string {
    char* text;
    size_t length;
} string;

size_t get_number_of_line(char* input, size_t strlen);
string* split_by_lines(char* input, size_t strlen, size_t* vector_length);
int compare_uint32_t(const void* a, const void* b);

// LIST
#define CAPACITY_STEP 5
#define LIST_TYPE uint32_t

typedef struct list {
    LIST_TYPE* vector;
    uint32_t length;
    uint32_t current_capacity;
} list;

void init_list(list* list);
void parse_text_to_list(list* list, char* text, char sep);
void add_unique_to_list(list* list, LIST_TYPE value);
void add_to_list(list* list, LIST_TYPE value);
void insert_value_at(list* list, uint32_t index, LIST_TYPE value);
uint8_t is_value_in_list(list* list, LIST_TYPE value);

#endif
