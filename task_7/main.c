#include <stdio.h>
#include <string.h>
#include "header.h"
//#include "dirty_business.c"
#define num 1000

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    FILE* file = fopen("test.txt", "r");
    if (!file) {
        printf("Cannot open test file\n");
        return 1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        solve_the_line(line);
    }

    fclose(file);
    return 0;
}