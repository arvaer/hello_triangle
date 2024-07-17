#include "ily_types.h"
#include <assert.h>

void test_vector_init(){
    vector test_vector;
    size_t test_size = sizeof(int);

    void* expected_items = NULL;
    size_t expected_count = 0;
    size_t expected_capacity = 0;
    size_t expected_size = sizeof(int);

    size_t result = vector_init(&test_vector, sizeof(int));

    assert(result == 1);
    assert(test_vector.items == expected_items);
    assert(test_vector.count == expected_count);
    assert(test_vector.capacity == expected_capacity);
    assert(test_vector.size == expected_size);
}

void test_vector_append(){}



int main(){
    test_vector_init();
}
