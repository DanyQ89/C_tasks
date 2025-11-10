#pragma warning(disable:4996)
#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void* memory_pool;
    size_t pool_size;
} Memory;

typedef struct {
    void* ptr;
    int size;
    bool is_allocated;
} PointerInfo;

typedef struct FreeBlock {
    void* address;
    struct FreeBlock* next;
} FreeBlock;

typedef struct FreeList {
    FreeBlock* head;
    int block_size;
    int count;
} FreeList;

// Объявления функций
void* malloc_buddy(int size);
void free_buddy(void* ptr);
void init_memory(void); 
int make_weight(int num);
void solve_the_line(char* line);
void print_memory_state();