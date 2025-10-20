#include <locale.h>
#include "header.h"
#include <stdio.h> 

int main() {
	setlocale(LC_ALL, "Russian");
	SystemInit(1024);

	bufs* input = open("input.txt", 'r');
	bufs* output = open("output.txt", 'w');


	if (!input || !output) {
		printf("Ошибка при открытии файлов.\n");
		if (input) {
			close(input);
		}
		if (output) {
			close(output);
		}
		return 1;
	}

	char buffer[512];
	size_t bytes;

	while ((bytes = read(input, buffer, sizeof(buffer))) > 0) {


		write(output, buffer, bytes);

	}
	close(input);  
	close(output); 
	printf("Копирование завершено.\n");



	SystemClose();
	return 0;
}
