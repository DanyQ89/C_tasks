#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>
#include "header.h"
#include <SDL.h>
#include <stdlib.h>


char* trim(char* str) {
	while (*str && (*str == ' ' || *str == '\t')) {
		str++;
	}

	return str;
}


Menu SystemInitialise(char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		Menu empty = { 0 };
		return empty;
	}

	char line[256];
	char* eq = "=";
	char* comma = ",";
	char* none = "";
	// инициализировать можно и так и memset
	Menu menu = { 0 };
	ButtonsList buttons_list = { 0 };


	while (fgets(line, sizeof(line), file)) {
		// поиск первого вхождения в строку и замена нулем
		line[strcspn(line, "\n")] = 0;

		// при совпадении строк выдает 0
		if (!strcmp(line, "MenuBegin")) {

			while (fgets(line, sizeof(line), file)) {
				line[strcspn(line, "\n")] = 0;

				if (!strcmp(line, "MenuEnd")) {
					menu.buttons_list = buttons_list;
					fclose(file);

					return menu;
				}

				if (!strcmp(line, "WindowBegin")) {
					Window window;

					// очищает память в окне 
					memset(&window, 0, sizeof(Window));

					while (fgets(line, sizeof(line), file)) {
						line[strcspn(line, "\n")] = 0;


						if (!strcmp(line, "WindowEnd")) {
							menu.window = window;
							break;
						}
						char* context = NULL;
						char* key = trim(strtok_s(line, eq, &context));
						char* value = trim(strtok_s(NULL, none, &context));

						if (!strcmp(key, "Position")) {
							// куда копировать, указатель на новую строку т.е. строку со сдвигом, длина новой строки + нулевой элемент
							memmove(value, value + 1, strlen(value + 1) + 1);

							// сдвигаем на элемент влево и ставим туда ноль, чтобы строки видели конец там
							value[strlen(value) - 1] = '\0';

							char* pos_context = NULL;
							int x, y;
							x = atoi(strtok_s(value, comma, &pos_context));
							y = atoi(strtok_s(NULL, none, &pos_context));

							window.position.x = x;
							window.position.y = y;


						}

						if (!strcmp(key, "Color")) {
							unsigned char r, g, b;

							char* color_context = NULL;
							r = atoi(strtok_s(value, comma, &color_context));
							g = atoi(strtok_s(NULL, comma, &color_context));
							b = atoi(strtok_s(NULL, none, &color_context));

							window.color.r = r;
							window.color.g = g;
							window.color.b = b;

						}

					}

				}
				if (!strcmp(line, "ButtonBegin")) {
					Button button;
					memset(&button, 0, sizeof(Button));

					while (fgets(line, sizeof(line), file)) {
						line[strcspn(line, "\n")] = 0;
						if (!strcmp(line, "ButtonEnd")) {
							buttons_list.buttons[buttons_list.count] = button;
							buttons_list.count++;
							break;
						}
						char* context = NULL;
						char* key = trim(strtok_s(line, eq, &context));
						char* value = trim(strtok_s(NULL, none, &context));

						if (!strcmp(key, "Position")) {

							memmove(value, value + 1, strlen(value + 1) + 1);
							value[strlen(value) - 1] = '\0';

							char* pos_context = NULL;
							int x, y;
							x = atoi(strtok_s(value, comma, &pos_context));
							y = atoi(strtok_s(NULL, none, &pos_context));

							button.position.x = x;
							button.position.y = y;


						}

						if (!strcmp(key, "Color")) {
							unsigned char r, g, b;

							char* color_context = NULL;
							r = atoi(strtok_s(value, comma, &color_context));
							g = atoi(strtok_s(NULL, comma, &color_context));
							b = atoi(strtok_s(NULL, none, &color_context));

							button.color.r = r;
							button.color.g = g;
							button.color.b = b;

						}

						if (!strcmp(key, "HighlightColor")) {

							unsigned char r, g, b;

							char* highlight_context = NULL;
							r = atoi(strtok_s(value, comma, &highlight_context));
							g = atoi(strtok_s(NULL, comma, &highlight_context));
							b = atoi(strtok_s(NULL, none, &highlight_context));

							button.highlight_color.r = r;
							button.highlight_color.g = g;
							button.highlight_color.b = b;

						}

						if (!strcmp(key, "Name")) {

							strcpy_s(button.name, sizeof(button.name), value);
							
						}

					}


				}

			}
		}
	}

}


void SystemRelease(Menu* menu) {
	memset(menu, 0, sizeof(Menu));
}