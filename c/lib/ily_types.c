#include "ily_types.h"

size_t vector_append(vector* vector, const void* item);
size_t vector_remove(vector* vector, const size_t position);
size_t vector_subtract_string(vector* vector, const void* item);

size_t option_wrap(option* option, const void* item, const size_t itemSize);
void* option_unwrap(option* option);
size_t option_release(option* option);
size_t option_peek(option* option);
option* option_clone(option* option);


size_t vector_init(vector* vector, const size_t itemSize){
    vector->items = NULL;
    vector->count = 0;
    vector->capacity = 0;
    vector->size = itemSize;
    return 1;
}

size_t vector_append(vector* vector, const void* item){
    // first we need to check capacity and count.
}
