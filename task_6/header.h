#pragma warning(disable:4996)
#ifndef BUFF
#define BUFF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    FILE* file; //указатель на файл
    char* buf; //указатель на буфе
    size_t buf_size; //размер буфера
    size_t buf_pos; //текущее положение в буфере
    char mode; //режим открытия файла
} bufs;

void SystemInit(size_t buffer_size);

bufs* open(const char* name, const char mode);
void close(bufs* bf);
size_t read(bufs* bf, void* dest, size_t to_read);
size_t write(bufs* bf, const void* src, size_t to_write);
void SystemClose();

#endif


