#ifndef H_UTILS
#define H_UTILS
#include <stddef.h>
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
#define BUILT_IN_CMP(TYPE)             \
    uint8_t cmp_##TYPE(TYPE a, TYPE b) \
    {                                  \
        return a == b;                 \
    }

#define LIST_DECL(TYPE, STEP_SIZE, EQ_FCT)                                                      \
    typedef struct list_##TYPE {                                                                \
        TYPE* vector;                                                                           \
        size_t length;                                                                          \
        size_t current_capacity;                                                                \
    } list_##TYPE;                                                                              \
                                                                                                \
    void init_list_##TYPE(list_##TYPE* list)                                                    \
    {                                                                                           \
        if (list->vector != NULL) {                                                             \
            free(list->vector);                                                                 \
        }                                                                                       \
        list->current_capacity = STEP_SIZE;                                                     \
        list->vector = (TYPE*)malloc(sizeof(TYPE) * list->current_capacity);                    \
        list->length = 0;                                                                       \
    }                                                                                           \
    uint8_t is_value_in_list_##TYPE(list_##TYPE* list, TYPE value)                              \
    {                                                                                           \
        for (size_t k = 0; k < list->length; k++) {                                             \
            if (EQ_FCT(list->vector[k], value)) {                                               \
                return 1;                                                                       \
            }                                                                                   \
        }                                                                                       \
        return 0;                                                                               \
    }                                                                                           \
    void add_unique_to_list_##TYPE(list_##TYPE* list, TYPE value)                               \
    {                                                                                           \
        if (is_value_in_list_##TYPE(list, value)) {                                             \
            return;                                                                             \
        }                                                                                       \
                                                                                                \
        if (list->length >= list->current_capacity) {                                           \
            list->current_capacity += STEP_SIZE;                                                \
            list->vector = (TYPE*)realloc(list->vector, sizeof(TYPE) * list->current_capacity); \
        }                                                                                       \
                                                                                                \
        list->vector[list->length++] = value;                                                   \
    }                                                                                           \
    void add_to_list_##TYPE(list_##TYPE* list, TYPE value)                                      \
    {                                                                                           \
        if (list->length >= list->current_capacity) {                                           \
            list->current_capacity += STEP_SIZE;                                                \
            list->vector = (TYPE*)realloc(list->vector, sizeof(TYPE) * list->current_capacity); \
        }                                                                                       \
                                                                                                \
        list->vector[list->length++] = value;                                                   \
    }                                                                                           \
    void insert_value_at_##TYPE(list_##TYPE* list, size_t index, TYPE value)                    \
    {                                                                                           \
        if (index > list->length) {                                                             \
            return;                                                                             \
        }                                                                                       \
                                                                                                \
        if (list->length >= list->current_capacity) {                                           \
            list->current_capacity += STEP_SIZE;                                                \
            list->vector = (TYPE*)realloc(list->vector, sizeof(TYPE) * list->current_capacity); \
        }                                                                                       \
                                                                                                \
        for (size_t i = list->length; i > index; i--) {                                         \
            list->vector[i] = list->vector[i - 1];                                              \
        }                                                                                       \
        list->vector[index] = value;                                                            \
        list->length++;                                                                         \
    }                                                                                           \
    void delete_last_value_##TYPE(list_##TYPE* list)                                            \
    {                                                                                           \
        if (list->length < list->current_capacity - STEP_SIZE) {                                \
            list->current_capacity -= STEP_SIZE;                                                \
            list->vector = (TYPE*)realloc(list->vector, sizeof(TYPE) * list->current_capacity); \
        }                                                                                       \
                                                                                                \
        list->length--;                                                                         \
    }                                                                                           \
    void delete_value_at_##TYPE(list_##TYPE* list, size_t index)                                \
    {                                                                                           \
        for (size_t i = index; i < list->length - 1; i++) {                                     \
            list->vector[i] = list->vector[i + 1];                                              \
        }                                                                                       \
        if (list->length < list->current_capacity - STEP_SIZE) {                                \
            list->current_capacity -= STEP_SIZE;                                                \
            list->vector = (TYPE*)realloc(list->vector, sizeof(TYPE) * list->current_capacity); \
        }                                                                                       \
                                                                                                \
        list->length--;                                                                         \
    }

#define LIST_FOR_INT(TYPE)                                                  \
    void parse_text_to_list_##TYPE(list_##TYPE* list, char* text, char sep) \
    {                                                                       \
        char* new_text = (char*)malloc(sizeof(char) * (strlen(text) + 1));  \
        strcpy(new_text, text);                                             \
        char *start_pointer = new_text, *next_pointer;                      \
        while ((next_pointer = strchr(start_pointer, sep))) {               \
            *next_pointer = '\0';                                           \
            add_to_list_##TYPE(list, atoi(start_pointer));                  \
            start_pointer = next_pointer + 1;                               \
        }                                                                   \
        add_to_list_##TYPE(list, atoi(start_pointer));                      \
        free(new_text);                                                     \
    }

// POSITION
typedef struct pos {
    int line;
    int column;
} pos;

typedef enum direction {
    up,
    right,
    down,
    left
} direction;

void apply_direction(pos* current_pos, direction dir);

// QUEUE
#define QUEUE_DECL(TYPE)                                                                                 \
    typedef struct queue_##TYPE##_wrapper {                                                              \
        TYPE value;                                                                                      \
        struct queue_##TYPE##_wrapper* next_in_list;                                                     \
        struct queue_##TYPE##_wrapper* prev_in_list;                                                     \
    } queue_##TYPE##_wrapper;                                                                            \
    typedef struct queue_##TYPE {                                                                        \
        size_t queue_length;                                                                             \
        queue_##TYPE##_wrapper* first;                                                                   \
        queue_##TYPE##_wrapper* last;                                                                    \
    } queue_##TYPE;                                                                                      \
                                                                                                         \
    void init_queue_##TYPE(queue_##TYPE* queue)                                                          \
    {                                                                                                    \
        queue->queue_length = 0;                                                                         \
        queue->first = NULL;                                                                             \
        queue->last = NULL;                                                                              \
    }                                                                                                    \
    uint8_t is_queue_empty_##TYPE(queue_##TYPE* queue)                                                   \
    {                                                                                                    \
        return queue->queue_length == 0;                                                                 \
    }                                                                                                    \
    void push_to_queue_##TYPE(queue_##TYPE* queue, TYPE value)                                           \
    {                                                                                                    \
        if (queue->last == NULL) {                                                                       \
            queue->first = (queue_##TYPE##_wrapper*)malloc(sizeof(queue_##TYPE##_wrapper));              \
            queue->first->value = value;                                                                 \
            queue->first->next_in_list = NULL;                                                           \
            queue->first->prev_in_list = NULL;                                                           \
                                                                                                         \
            queue->last = queue->first;                                                                  \
        } else {                                                                                         \
            queue->last->next_in_list = (queue_##TYPE##_wrapper*)malloc(sizeof(queue_##TYPE##_wrapper)); \
            queue->last->next_in_list->value = value;                                                    \
            queue->last->next_in_list->next_in_list = NULL;                                              \
            queue->last->next_in_list->prev_in_list = queue->last;                                       \
                                                                                                         \
            queue->last = queue->last->next_in_list;                                                     \
        }                                                                                                \
        queue->queue_length++;                                                                           \
    }                                                                                                    \
    void pop_queue_##TYPE(queue_##TYPE* queue)                                                           \
    {                                                                                                    \
        if (queue->queue_length == 0)                                                                    \
            return;                                                                                      \
                                                                                                         \
        if (queue->queue_length == 1) {                                                                  \
            free(queue->first);                                                                          \
            queue->first = NULL;                                                                         \
            queue->last = NULL;                                                                          \
        } else {                                                                                         \
            queue_##TYPE##_wrapper* to_delete = queue->last;                                             \
            queue->last = queue->last->prev_in_list;                                                     \
            queue->last->next_in_list = NULL;                                                            \
            free(to_delete);                                                                             \
        }                                                                                                \
        queue->queue_length--;                                                                           \
    }                                                                                                    \
    void empty_queue_##TYPE(queue_##TYPE* queue)                                                         \
    {                                                                                                    \
        while (!is_queue_empty_##TYPE(queue))                                                            \
            pop_queue_##TYPE(queue);                                                                     \
    }

#define HASHMAP_DECL(TYPE, TABLE_SIZE, hash_fct, eq_fct)                    \
    QUEUE_DECL(TYPE);                                                       \
    typedef struct hashmap_##TYPE {                                         \
        queue_##TYPE table[TABLE_SIZE];                                     \
    } hashmap_##TYPE;                                                       \
                                                                            \
    void init_hashmap_##TYPE(hashmap_##TYPE* hashmap)                       \
    {                                                                       \
        for (size_t i = 0; i < TABLE_SIZE; i++) {                           \
            init_queue_##TYPE(&hashmap->table[i]);                          \
        }                                                                   \
    }                                                                       \
                                                                            \
    TYPE* get_value_ptr_##TYPE(hashmap_##TYPE* hashmap, TYPE key)           \
    {                                                                       \
        size_t hash_val = hash_fct(key) % TABLE_SIZE;                       \
        queue_##TYPE##_wrapper* current_val = hashmap->table[hash_val].first; \
        while (current_val != NULL && !eq_fct(current_val->value, key)) {   \
            current_val = current_val->next_in_list;                        \
        }                                                                   \
        return &current_val->value;                                         \
    }                                                                       \
                                                                            \
    void add_value_unique_key_##TYPE(hashmap_##TYPE* hashmap, TYPE value)   \
    {                                                                       \
        size_t hash_val = hash_fct(value) % TABLE_SIZE;                     \
        if (get_value_ptr_##TYPE(hashmap, value) == NULL) {                 \
            push_to_queue_##TYPE(&hashmap->table[hash_val], value);         \
        }                                                                   \
    }                                                                       \
                                                                            \
    void free_hashmap_##TYPE(hashmap_##TYPE* hashmap)                       \
    {                                                                       \
        for (size_t i = 0; i < TABLE_SIZE; i++) {                           \
            empty_queue_##TYPE(&hashmap->table[i]);                         \
        }                                                                   \
    }

#endif
