#pragma warning(disable:4996)
#ifndef BUFF
#define BUFF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    FILE* file; 
    char* buf; 
    size_t buf_size; 
    size_t buf_pos; 
    char mode; 
} bufs;

void SystemInit(size_t buffer_size);

bufs* open(const char* name, const char mode);
void close(bufs* bf);
size_t read(bufs* bf, void* dest, size_t to_read);
size_t write(bufs* bf, const void* src, size_t to_write);
void SystemClose();

#endif



