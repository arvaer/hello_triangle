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
    uint32_t count;
    size_t capacity;
    size_t size;
} vector;

int vector_init(vector* vector, const size_t itemSize);
void vector_free(vector* vector);
int vector_append(vector* vector, const void* item);
int vector_empty(vector* vector);
void* vector_get(vector* vector, const size_t index);
/**
 * this function removes an item from the vector at the specified index and
 * returns a pointer to the removed item. The caller is responsible for
 * freeing the returned item to avoid memory leaks.
 *
 * pls seriously. make sure u free it later
 */
void* vector_remove(vector* vector, const size_t index);

option option_wrap(const void* item, const size_t itemSize);
void* option_unwrap(option* option);
int option_peek(option* option);
int option_release(option* option);
option* option_clone(option* option);

#endif // !ILY_TYPES_H
