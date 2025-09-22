#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/types.h>
#include <locale.h>
#define len_of_str 25
#define _GNU_SOURCE

char* append_char_to_string(char* s, char c) {

	if (s == NULL) {
		char* new_str = malloc(2 * sizeof(char));
		if (new_str != NULL) {
			new_str[0] = c;
			new_str[1] = '\0';
		}
		return new_str;
	}

	int lenn = strlen(s);

	char* new_str = malloc(lenn + 2 * sizeof(char));

	if (new_str != NULL) {
		strcpy_s(new_str, (lenn + 2) * sizeof(char), s);
		new_str[lenn] = c;
		new_str[lenn + 1] = '\0';
		free(s);
	}
	return new_str;
}

void print_str(char* s, int lenn, int spaces) {

	for (int i = 0; i < lenn; i++) {
		if (s[i] != '_') {
			printf("%c", s[i]);
		}
		else {
			for (int j = 0; j < spaces; j++) {
				printf("_");
			}
		}
	}
	printf("\n");

	return NULL;
}

char* do_the_deal(char* s) {
	int space_cnt = 0;
	char* ost = malloc(1);
	ost[0] = '\0';
	//printf("%s", "meow");

	size_t lenn = strlen(s);

	//printf("%s", s);
	for (int i = 0; i < lenn; i++) {
		//printf("%c", s[i]);
		if (s[i] == '_') {
			space_cnt++;
		}

	}
	//printf("%d\n", space_cnt);

	//printf("%s %d\n", s, lenn);
	if (lenn == len_of_str) {
		printf("%s\n", s);
	}
	else if (space_cnt == 0) {
		//printf("%d", space_cnt);
		//printf("%s %d", "spacess in string\n", space_cnt);
		if (lenn > len_of_str) {
			for (int j = 0; j < len_of_str; j++) {
				printf("%c", s[j]);
			}
			for (int k = len_of_str; k < lenn; k++) {
				ost = append_char_to_string(ost, s[k]);
			}
			printf("%c", '\n');
			return ost;
		}
		//printf("%s", s);
		for (int j = 0; j < lenn; j++) {
			printf("%c", s[j]);
		}
		for (int j = 0; j < (len_of_str - lenn); j++) {
			printf("_");
		}
		printf("\n");
	}
	else if (lenn < len_of_str) {
		int now_spaces = 1;
		int max_iterations = 20;
		int new_lenn = lenn;
		//printf("%d", lenn);
		
		int need_spaces = len_of_str - (lenn - space_cnt);
		if (space_cnt > 0) {
			now_spaces = need_spaces / space_cnt;
			int extra_spaces = need_spaces % space_cnt;
			
			int space_index = 0;
			for (int i = 0; i < lenn; i++) {
				if (s[i] != '_') {
					printf("%c", s[i]);
				}
				else {
					int current_spaces = now_spaces;
					if (space_index < extra_spaces) {
						current_spaces++;
					}
					for (int j = 0; j < current_spaces; j++) {
						printf("_");
					}
					space_index++;
				}
			}
		}
		printf("\n");
	} 
	else if (lenn > len_of_str) {
		int last_space = -1;
		int now_lenn = 0;

		for (int i = 0; i < lenn; i++) {
			if (s[i] == '_') {
				if (now_lenn <= len_of_str) {
					last_space = i;
				}
			}
			now_lenn++;

			if (now_lenn > len_of_str) {
				break;
			}
		}

		if (last_space != -1) {
			int osnova_len = last_space;
			char* osnova = malloc(osnova_len + 1);
			strncpy_s(osnova, osnova_len + 1, s, osnova_len);
			osnova[osnova_len] = '\0';

			int space_cnt_in_part = 0;
			for (int i = 0; i < osnova_len; i++) {
				if (osnova[i] == '_') {
					space_cnt_in_part++;
				}
			}

			if (space_cnt_in_part > 0) {
				int need_spaces = len_of_str - (osnova_len - space_cnt_in_part);
				int now_spaces = need_spaces / space_cnt_in_part;
				int extra_spaces = need_spaces % space_cnt_in_part;

				int space_index = 0;
				for (int i = 0; i < osnova_len; i++) {
					if (osnova[i] != '_') {
						printf("%c", osnova[i]);
					}
					else {
						int current_spaces = now_spaces;
						if (space_index < extra_spaces) {
							current_spaces++;
						}
						for (int j = 0; j < current_spaces; j++) {
							printf("_");
						}
						space_index++;
					}
				}
			}
			else {
				printf("%s", osnova);
			}
			printf("\n");
			free(osnova);

			for (int i = last_space + 1; i < lenn; i++) {
				ost = append_char_to_string(ost, s[i]);
			}
			return ost;
		}
		else {
			for (int j = 0; j < len_of_str; j++) {
				printf("%c", s[j]);
			}
			for (int k = len_of_str; k < lenn; k++) {
				ost = append_char_to_string(ost, s[k]);
			}
			printf("%c", '\n');
			return ost;
		}
	}
	//	printf("%s %s\n", s, "больше чем надо");
	//	int lines_cnt = 1;
	//	//lines_cnt++;


	//	// количество строк		
	//	for (int i = 0; i < lenn; i++) {
	//		if (s[i] == '_') {
	//			lines_cnt++;
	//		}

	//	}
	//	//printf("%d %s\n", lines_cnt, "колво пробелов");
	//	int index_now = 0;
	//	char* now_str = malloc(1);
	//	now_str[0] = '\0';

	//	// выделяем строки в массив
	//	char** lines = malloc(lines_cnt * sizeof(char*));
	//	for (int i = 0; i < lines_cnt; i++) {
	//		lines[i] = NULL;
	//	}

	//	//char** lines[0];
	//	//printf("%d", 0);

	//	int count = 0;
	//	for (int i = 0; i < lenn; i++) {

	//		//printf("%s\n", buffer[]);

	//		if ((s[i] == '_')) {

	//			if (index_now < lines_cnt) {

	//				lines[index_now] = now_str;
	//				//printf("%s\n", lines[index_now]);
	//				index_now++;
	//				now_str = malloc(1);
	//				now_str[0] = '\0';
	//			}
	//		}
	//		else {
	//			//printf("%d\n", 3);
	//			now_str = append_char_to_string(now_str, s[i]);
	//		}
	//	}


		// теперь есть массив слов

	//	printf("%c", s[len_of_str]);
	//	printf("%c", s[len_of_str - 1]);
	//	printf("капец");
	//	if ((s[len_of_str - 1]) != '_' && (s[len_of_str] == '_')) {
	//		printf("%s нужная строка \n", s);
	//		for (int i = 0; i < len_of_str; i++) {
	//			printf("%c", s[i]);
	//		}
	//		printf("%c", '\n');
	//		for (int j = len_of_str; j < lenn; j++) {
	//			if (s[j] != '_') {
	//				ost = append_char_to_string(ost, s[j]);
	//			}
	//		}
	//		if (ost) {
	//			return ost;

	//		}
	//		return "null";
	//	}


	//	//for (int i = 0; i < lines_cnt; i++) {
	//		//printf("%s+++", lines[i]);
	//	//}

	//}

	return "null";
}

int main() {
	setlocale(LC_ALL, "Russian");

	// прочитал
	char* name = "F:/ProgramFilesX/Codes/meow_c/Project1/Project1/example.txt";
	FILE* file = fopen(name, "r");
	if (file == NULL) {
		return NULL;
	}

	// длина файла
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// в буфер все записали
	if (size <= 0) {
		fclose(file);
		return NULL;
	}
	char* buffer = malloc(size + 1);

	if (!buffer) {
		fclose(file);
		return NULL;
	}

	size_t read = fread(buffer, 1, size, file);
	buffer[read] = '\0';
	fclose(file);

	int lines_cnt = 1;
	//lines_cnt++;

	// количество строк		
	for (int i = 0; i < read; i++) {
		if (buffer[i] == '\n' && lines_cnt < 10) {
			lines_cnt++;
		}
		else if (buffer[i] == ' ') {
			buffer[i] = '_';
		}
	}

	int index_now = 0;
	char* now_str = malloc(1);
	now_str[0] = '\0';

	// выделяем строки в массив
	char** lines = malloc(lines_cnt * sizeof(char*));
	for (int i = 0; i < lines_cnt; i++) {
		lines[i] = NULL;
	}

	//char** lines[0];
	//printf("%d", 0);

	int count = 0;
	for (int i = 0; i < read; i++) {

		//printf("%s\n", buffer[]);

		if ((buffer[i] == '\n')) {

			if (index_now < lines_cnt) {

				lines[index_now] = now_str;
				index_now++;
				now_str = malloc(1);
				now_str[0] = '\0';
			}
		}
		else {
			//printf("%d\n", 3);
			now_str = append_char_to_string(now_str, buffer[i]);
		}
	}
	//printf("%d", 1);
	if (now_str) {
		lines[index_now] = now_str;
	}

	printf("колво строк было изначально: %d\n", lines_cnt);

	char* meow = "\0";

	for (int i = 0; i < lines_cnt; i++) {
		if (lines[i] != NULL) {
			char* new_string = lines[i];
			
			if (meow != NULL && strcmp(meow, "null") != 0 && strlen(meow) > 0) {
				char* temp = malloc(strlen(new_string) + strlen(meow) + 2);
				strcpy_s(temp, strlen(meow) + 1, meow);
				strcat_s(temp, strlen(temp) + strlen(new_string) + 1, "_");
				strcat_s(temp, strlen(temp) + strlen(new_string) + 1, new_string);
				new_string = temp;
			}
			
			meow = do_the_deal(new_string);
		}
	}

	int len_meow = strlen(meow);

	while (len_meow > len_of_str) {
		if (strcmp(meow, "null") == 0) {
			break;
		}
		meow = do_the_deal(meow);
		len_meow = strlen(meow);
	}
	
	if (len_meow > 0 && strcmp(meow, "null") != 0) {
		for (int i = 0; i < len_meow; i++) {
			printf("%c", meow[i]);
		}
		for (int i = 0; i < (len_of_str - len_meow); i++) {
			printf("_");
		}
	}

	return 0;
}
