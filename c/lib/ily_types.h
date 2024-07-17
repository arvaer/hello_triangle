#ifndef ILY_TYPES_H
#define ILY_TYPES_H
#include <stddef.h>
#include <stdint.h>

typedef struct {
    void* value;
    size_t isPresent;
    size_t size;
    size_t* refCount;
} option;

typedef struct {
    void* items;
    size_t count;
    size_t capacity;
    size_t size;
} vector;

size_t vector_init(vector* vector, const size_t itemSize);
size_t vector_append(vector* vector, const void* item);
size_t vector_remove(vector* vector, const size_t position);
size_t vector_subtract_string(vector* vector, const void* item);

size_t option_wrap(option* option, const void* item, const size_t itemSize);
void* option_unwrap(option* option);
size_t option_release(option* option);
size_t option_peek(option* option);
option* option_clone(option* option);



#endif // !ILY_TYPES_H
