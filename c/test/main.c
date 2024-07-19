#include "ily_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_vector_init() {
    vector test_vector;
    size_t test_size = sizeof(int);

    void* expected_items = NULL;
    uint32_t expected_count = 0;
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
    uint32_t expected_count = 3;
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
    uint32_t expected_count = 3;
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
    assert(test_vector.count == expected_count);
    assert(test_vector.capacity == expected_capacity);
    assert(test_vector.size == expected_size);

    vector_free(&test_vector);
    assert(test_vector.items == NULL);

}

void test_vector_remove(){
    vector test_vector;
    size_t test_size = sizeof(int);

    int expected_removed_item = 2;
    int expected_items[3] = {1, 3};
    uint32_t expected_count = 2;
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
    assert(test_vector.count == 3);
    assert(test_vector.capacity == 5);

    int* p_removed_item = (int*)vector_remove(&test_vector, 1);
    int removed_item = *p_removed_item;
    assert(removed_item == expected_removed_item);


    int* p_vector_1 = (int*)vector_get(&test_vector, 0);
    int* p_vector_2 = (int*)vector_get(&test_vector, 1);
    int vector_1 = *p_vector_1;
    int vector_2 = *p_vector_2;
    assert(vector_1 == expected_items[0]);
    assert(vector_2 == expected_items[1]);


    assert(test_vector.count == expected_count);
    assert(test_vector.capacity == expected_capacity);
    assert(test_vector.size == expected_size);


    vector_free(&test_vector);
    assert(test_vector.items == NULL);

}


void test_option_wrap(){
    int test_item = 12;
    int test_size = sizeof(int);
    int refcount = 1;

    option wrapped = option_wrap(&test_item, test_size);

    int* p_value = (int*)wrapped.value;
    assert(*p_value == test_item);
    assert(wrapped.isPresent == 1);
    assert(wrapped.size == test_size);
    assert(*wrapped.refCount == refcount);
}

void test_option_clone_release(){
    int test_item = 12;
    int test_size = sizeof(int);

    option wrapped = option_wrap(&test_item, test_size);
    option* clone = option_clone(&wrapped);
    assert(*(wrapped.refCount) == 2);
    option* clone2 = option_clone(clone);
    assert(*(wrapped.refCount) == 3);
    option* clone3 = option_clone(clone2);

    assert(*(wrapped.refCount) == 4);
    assert(*(clone3->refCount) ==4);
    assert(*(clone2->refCount) ==4);
    assert(*(clone->refCount) ==4);

    option_release(clone3);
    assert(*(wrapped.refCount) == 3);
    option_release(clone2);
    assert(*(wrapped.refCount) == 2);
    option_release(clone);
    assert(*(wrapped.refCount) == 1);
}

void test_option_peek(){
    int test_item = 12;
    int test_size = sizeof(int);
    option wrapped = option_wrap(&test_item, test_size);
    assert(option_peek(&wrapped));
    wrapped.isPresent = 0;
    assert(!option_peek(&wrapped));
}

void test_option_unwrap(){
    int test_item = 12;
    int test_size = sizeof(int);
    option wrapped = option_wrap(&test_item, test_size);
    option* clone = option_clone(&wrapped);
    option* clone2 = option_clone(&wrapped);

    int* value_in_option = (int*)option_unwrap(clone2);
    assert(*(wrapped.refCount) == 2);
    assert(clone2 == NULL);
    assert(*value_in_option == 12);

}

int main() {
    test_vector_init();
    test_vector_append();
    test_vector_get();
    test_vector_remove();
    test_option_wrap();
    test_option_clone_release();
    test_option_peek();
    test_option_unwrap();
}

