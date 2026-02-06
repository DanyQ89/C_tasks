#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <io.h>
#define MAX_SIZE 100

void pline() {
	printf("\n");
}

void print_help() {
	printf("Вот схема:\nmain.exe [каталог] [маска(можно сколько угодно)]\n");
}



int check_el(char set[][MAX_SIZE], char* name, int counter) {

	for (int i = 0; i < counter; i++) {
		if (strcmp(set[i], name) == 0) {
			return 0;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	pline();

	if (argc == 2 && (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "-?") == 0)) {
		print_help();
		return 0;
	}

	if (argc < 3) {
		printf("Слишком мало параметров (нужно минимум 2 не считая .exe) \n");
		print_help();
		return 1; // нужно для тестов
	}


	struct _finddata_t  file;
	char* catalog = argv[1]; 
	//char* need = "\0";
	char need[512];
	int cnt = 0;

	char meow_set[MAX_SIZE][MAX_SIZE];
	int has = 0;
	char* el = "\0";

	for (int i = 2; i < argc; i++) {
		sprintf(need, "%s/%s", catalog, argv[i]);


		// указатель может быть слишком большой
		intptr_t res = _findfirst(need, &file);
		//int res = _findfirst(need, &file);

		if (res == -1) {
			continue;
		}
		do
		{
			el = file.name;
			has = check_el(meow_set, el, cnt);

			if (has == 0) {
				strcpy(meow_set[cnt], el);
				cnt++;
			}
		}
		while (_findnext(res, &file) != -1);


			//if (res != -1) {

			//	el = file.name;
			//	has = check_el(meow_set, el, cnt);

			//	if (has == 0) {
			//		strcpy(meow_set[cnt], el);
			//		cnt++;
			//	}

			//	//printf("%s\n", file.name);
			//	while (_findnext(res, &file) != -1) {

			//		el = file.name;
			//		has = check_el(meow_set, el, cnt);

			//		if (has == 0) {
			//			strcpy(meow_set[cnt], el);
			//			cnt++;
			//		}

			//		//printf("%s\n", file.name);

			//	}
			//	
			//	_findclose(res);
			//}


			//if (res != -1) {
			//	printf("%s\n", file.name);
			//	while (_findnext(res, &file) != -1) {
			//		printf("%s\n", file.name);

			//	}
			//}

	}

	//for (int i = 0; i < cnt; i++) {
		//printf("%s\n", meow_set[i]);
	//}

	if (cnt) {
		for (int i = 0; i < cnt; i++) {
			printf("%s\n", meow_set[i]);
		}
	}
	else {
		printf("Ничего не найдено");
	}

	return 0;
}


//F:\ProgramFilesX\Codes
