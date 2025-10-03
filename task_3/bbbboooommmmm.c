#include <stdio.h> 
#include <locale.h>
#include <stdlib.h>



int check_bit(int num, int position) {
	return ((num & (1 << position)) != 0);
}

int switch_bit(int num, int position) {
	return (num ^ (1 << position));
}


int get_len(int num) {
	int counter;
	int oper = 0;
	for (int i = 31; i >= 0; i--) {
		if (check_bit(num, i)) {
			return i + 1;
		}
	}
	return 0;
}

void print_num_in_2(int num, int lenn) {
	printf("\n");

	for (int i = lenn - 1; i >= 0; i--) {
		printf("%d", check_bit(num, i));
	}
	printf("\n");
}


int to_16(int num) {
	if (num == 0) {
		return 0;
	}
	int ost = 0;
	int syst = 16;


	//printf("%d %d meow\n", num, ost);

	ost = num & (syst - 1);
	num = num >> syst;
	printf("%d num\n", num);
	to_16(num);

	if (ost < 10) {
		//printf("1");

		printf("%d", ost);

	}
	else {
		//printf("2");
		printf("%c", 'A' + (ost - 10));

	}

	return 0;
}


int main() {
	//setlocale(LC_ALL, "Russian");
	setlocale(LC_ALL, "ru_RU.UTF-8");

	//SetConsoleOutputCP(1251);
	//SetConsoleCP(1251);


	//int a = 235;

	int a;

	printf("Введите число: ");
	scanf_s("%d", &a);

	int first, second;

	size_t lenn = get_len(a);

	//print_num_in_2(a, lenn);
	if (a == 0) {
		printf("%d\n", 0);
		printf("%d", 0);

	}
	for (int i = 0; i < lenn / 2; i++) {
		int first = check_bit(a, i);
		int second = check_bit(a, lenn - i - 1);
		if (first != second) {
			a = switch_bit(a, i);
			a = switch_bit(a, lenn - i - 1);

		}
	}

	lenn = get_len(a);


	printf("\n%d need \n", a);
	print_num_in_2(a, lenn);
	to_16(a);


	return 0;
}
