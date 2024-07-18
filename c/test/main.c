#include "ily_types.h"
#include <assert.h>

void test_vector_init() {
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

void test_vector_append() {
    vector test_vector;
    size_t test_size = sizeof(int);

    int expected_items[3] = {1, 2, 3};
    size_t expected_count = 3;
    size_t expected_capacity = 5;
    size_t expected_size = sizeof(int);

    size_t result = vector_init(&test_vector, sizeof(int));
    assert(result == 1);

    int item_1 = 1;
    int item_2 = 2;
    int item_3 = 3;
    vector_append(&test_vector, &item_1);
    vector_append(&test_vector, &item_2);
    vector_append(&test_vector, &item_3);

    // dude what the fu
    int vector_1 = int(((int*)test_vector.items)[0]);
    int vector_2 = int(((int*)test_vector.items)[1]);
    int vector_3 = int(((int*)test_vector.items)[2]);

    assert(vector_1 == expected_items[0]);
    assert(vector_2 == expected_items[1]);
    assert(vector_3 == expected_items[2]);

    vector_free(&test_vector);
    assert(test_vector.items == NULL);
}


void test_vector_get(){
    vector test_vector;
    size_t test_size = sizeof(int);

    int expected_items[3] = {1, 2, 3};
    size_t expected_count = 3;
    size_t expected_capacity = 5;
    size_t expected_size = sizeof(int);

    size_t result = vector_init(&test_vector, sizeof(int));
    assert(result == 1);
    int item_1 = 1;
    int item_2 = 2;
    int item_3 = 3;
    vector_append(&test_vector, &item_1);
    vector_append(&test_vector, &item_2);
    vector_append(&test_vector, &item_3);

    // dude what the fu
    int* p_vector_1 = (int*)vector_get(&test_vector, 0);
    int* p_vector_2 = (int*)vector_get(&test_vector, 1);
    int* p_vector_3 = (int*)vector_get(&test_vector, 2);

    int vector_1 = *p_vector_1;
    int vector_2 = *p_vector_2;
    int vector_3 = *p_vector_3;

    assert(vector_1 == expected_items[0]);
    assert(vector_2 == expected_items[1]);
    assert(vector_3 == expected_items[2]);

    vector_free(&test_vector);
    assert(test_vector.items == NULL);
}

int main() {
    test_vector_init();
    test_vector_append();
    test_vector_get();
}
