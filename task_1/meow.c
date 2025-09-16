#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <windows.h>
#include <locale.h>


bool func(const char* s)
{
    //char s[] = "12+-3sad2asd-23sad13";
    // ans : num: 3, oper: 1
    int cnt_num = 0;
    int cnt_oper = 0;
    int gr_num = 0;
    int gr_oper = 0;

    for (int i = 0; i < strlen(s); i++) {


        char el = s[i];

        if (isalpha(el)) {
            //printf("aplha: %c\n", el);

            if (cnt_num > 0) {
                if (cnt_num > 1) {
                    gr_num++;
                }
                cnt_num = 0;
            }
            if (cnt_oper > 0) {
                if (cnt_oper > 1) {
                    gr_oper++;
                }
                cnt_oper = 0;
            }
            continue;
        }

        else if (isdigit(el)) {
            //printf("digit: %c\n", el);


            if (cnt_oper > 0) {
                if (cnt_oper > 1) {
                    gr_oper++;
                }

                cnt_oper = 0;
            }

            cnt_num++;

        }
        else {

            printf("oper: %c\n", el);




            if (cnt_num > 0) {
                if (cnt_num > 1) {
                    gr_num++;
                }

                cnt_num = 0;
            }


            cnt_oper++;

            /*if (cnt_oper > 0) {
                cnt_oper++;

            }
            else {
                if (cnt_num > 0 ) {
                    gr_num++;
                    cnt_num = 0;
                }

            cnt_oper++;
            */
        }
    }

    if (cnt_num > 1) {
        gr_num++;
    }
    if (cnt_oper > 1) {
        gr_oper++;
    }


    printf("num groups: %d \noper groups: %d\n", gr_num, gr_oper);
    if (gr_num > gr_oper) {
        printf("true");
        return true;

    }
    else {
        printf("false");
        return false;

    }



}



int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    //setlocale(LC_ALL, "ru_RU.UTF-8");
	setlocale(LC_ALL, "Russian");

    char meow[100];
    printf("Введите строку:\n");
    fgets(meow, 100, stdin);
    func(meow);
    

    return 0;
}
