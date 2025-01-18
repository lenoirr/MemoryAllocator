#include "test.h"
#include "main.h"

// Test Implementations

void test_allocate_full_pool() {
    TEST_START("Allocate Full Pool");

    initialize_memory_pool();
    void* ptr = magic_malloc(MEMORY_POOL_SIZE - BLOCK_SIZE);
    assert(ptr != NULL && "Failed to allocate full pool.");

    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);
    assert(block->size == MEMORY_POOL_SIZE - BLOCK_SIZE && block->free == 0);

    TEST_SUCCESS("Allocate Full Pool");
    
    visualize_memory_pool();
    clear_memory_pool();
}

void test_free_middle_block() {
    TEST_START("Free Middle Block");

    initialize_memory_pool();
    void* ptr1 = magic_malloc(128);
    void* ptr2 = magic_malloc(256);
    void* ptr3 = magic_malloc(512);

    assert(ptr1 && ptr2 && ptr3 && "Failed to allocate multiple blocks.");

    magic_free(ptr2);

    Block* block = (Block*)((char*)ptr2 - BLOCK_SIZE);
    assert(block->free == 1 && "Middle block was not freed.");
    assert(block->size == 256);
    assert(block->next->free == 1 && "Did not point freed block node to next free block");

    TEST_SUCCESS("Free Middle Block");
    visualize_memory_pool();
    clear_memory_pool();
}

void test_allocate_zero_bytes() {
    TEST_START("Allocate Zero Bytes");

    initialize_memory_pool();
    void* ptr = magic_malloc(0);
    assert(ptr == NULL && "Allocating zero bytes should fail.");

    TEST_SUCCESS("Allocate Zero Bytes");
    visualize_memory_pool();
    clear_memory_pool();
}

void test_allocate_larger_than_pool() {
    TEST_START("Allocate Larger Than Pool");

    initialize_memory_pool();
    void* ptr = magic_malloc(MEMORY_POOL_SIZE);
    assert(ptr == NULL && "Allocating more memory than pool size should fail.");

    TEST_SUCCESS("Allocate Larger Than Pool");
    visualize_memory_pool();
    clear_memory_pool();
}

void test_allocate_and_free_all() {
    TEST_START("Allocate and Free All");

    initialize_memory_pool();
    void* ptr1 = magic_malloc(MEMORY_POOL_SIZE - BLOCK_SIZE);

    assert(ptr1 && "Failed to allocate memory blocks.");

    magic_free(ptr1);

    assert(free_list->size == MEMORY_POOL_SIZE - BLOCK_SIZE && "Incorrect free block size after freeing all blocks.");
    assert(free_list->next == NULL && "Free list should only contain one block.");
    assert(free_list->free == 1 && "Free block should be marked as free.");

    TEST_SUCCESS("Allocate and Free All");
    visualize_memory_pool();
    clear_memory_pool();
}

void test_fragmentation_and_coalescing() {
    TEST_START("Fragmentation and Coalescing");

    initialize_memory_pool();
    void* ptr1 = magic_malloc(128);
    void* ptr2 = magic_malloc(256);
    void* ptr3 = magic_malloc(64);

    magic_free(ptr2);
    magic_free(ptr1);

    assert(free_list->size == 128 + 256 + BLOCK_SIZE && "Incorrect free block size after freeing two blocks.");
    assert(free_list->next != NULL && "Free list should only contain one block.");
    assert(free_list->free == 1 && "Free block should be marked as free.");

    TEST_SUCCESS("Fragmentation and Coalescing");

    visualize_memory_pool();
    clear_memory_pool();
}

void test_double_free() {
    TEST_START("Double Free");

    initialize_memory_pool();
    void* ptr = magic_malloc(128);
    magic_free(ptr);

    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);
    assert(block->free == 1 && "Block should be marked as free.");

    printf("Attempting double free...\n");
    magic_free(ptr); // Should not crash or corrupt memory

    assert(block->free == 1 && "Double free should not corrupt block state.");

    TEST_SUCCESS("Double Free");

    visualize_memory_pool();
    clear_memory_pool();
}

void clear_memory_pool() {
    for (int i = 0; i < MEMORY_POOL_SIZE; i++) {
        memory_pool[i] = 0;
    }
    initialize_memory_pool();
}

// Run all tests
void run_all_tests() {
    test_allocate_full_pool();
    test_free_middle_block();
    test_allocate_zero_bytes();
    test_allocate_larger_than_pool();
    test_allocate_and_free_all();
    test_fragmentation_and_coalescing();
    test_double_free();
}
