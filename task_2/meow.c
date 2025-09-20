#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/types.h>
#include <locale.h>

#define _GNU_SOURCE

int main() {
	// прочитал
	FILE* file = fopen("example.txt", "r");
	if (file == NULL) return NULL;

	// длина файла
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// в буфер все записали
	char* buffer = malloc(size + 1);
	fread(buffer, 1, size, file);
	buffer[size] = '\0';
	fclose(file);

	// ебанем строки
	int lines_cnt = 1;
	lines_cnt++;
	for (int i = 0; i < size; i++) {
		if (buffer[i] == '\n' && lines_cnt < 10) {
			lines_cnt++;
			printf("%c", buffer[i]);
		}
	}

	char** lines = malloc((lines_cnt + 1) * sizeof(char*));
	int count = 0;


	char* line = strtok(buffer, "\n");

	while (line != NULL) {
		int need_len = malloc(strlen(line) + 1);
		lines[count] = need_len;
		strcpy(lines[count], *line);
		count++;
		line = strtok(NULL, "\n");
	}

	char slc;
	sprintf(slc, "%d", lines_cnt);

	count++;
	lines[count] = malloc(strlen(slc) + 1);

	strcpy(&lines[count], slc);

	free(buffer);
	return 0;
}

//int main() {
//setlocale(LC_ALL, "Russian");
//
////char** arr = read_lines("example.txt");
////for (int i = 0; i < atoi(arr[-1]); i++) {
//	//printf("%s", arr[i]);
////}
//return 0;
//} 
