#include "ily_types.h"
#include <cstring>
#include <stdlib.h>

size_t option_wrap(option* option, const void* item, const size_t itemSize);
void* option_unwrap(option* option);
size_t option_release(option* option);
size_t option_peek(option* option);
option* option_clone(option* option);

int vector_init(vector* vector, const size_t itemSize) {
    vector->items = NULL;
    vector->count = 0;
    vector->capacity = 0;
    vector->size = itemSize;
    return 1;
}

int vector_append(vector* vector, const void* item) {
    if (vector->count >= vector->capacity) {
        // this is just saying if cap == 0, make it 5. otherwise, double it
        size_t new_capacity = vector->capacity == 0 ? 5 : vector->capacity * 2;
        void* new_items = realloc(vector->items, new_capacity * vector->size);
        if (!new_items) {
            return 0;
        }
        vector->items = new_items;
        vector->capacity = new_capacity;
    }

    // originally this was just vector->items[vector->count++] = item but since its void* we cant use array
    // thtas also why we coerce items here into char*
    char* dest = (char*)vector->items + (vector->count * vector->size);

    memcpy(dest, item, vector->size);
    vector->count++;
    return 1;
}

void* vector_get(vector* vector, const size_t index) {
    if (vector->count >= index) {
        return NULL;
    }
    return (char*)vector->items + (index * vector->size);
}

//eventually we can support negative indexing. for now, size_t is unsigned so
void* vector_remove(vector* vector, const size_t index) {
    if (vector->count <= index) {
        return NULL;
    }
    char* item =(char*) malloc(vector->size);
    if(!item){
        return NULL;
    }
    char* target = (char*)vector->items + (index * vector->size);
    memcpy(item,target,vector->size);
    if (index < vector->count - 1){
        memmove(target, target + vector->size, (vector->count - index - 1) * vector->size);
    }

    vector->count--;
    return item;
}

void vector_free(vector* vector) {
    free(vector->items);
    vector->items = NULL;
    vector->count = 0;
    vector->capacity = 0;
}
