#include <stdio.h>
#include <string.h>
#include <io.h>
#include <windows.h>

#define MAX_FILES 1000
#define PATH_LEN 256

// Структура для хранения найденных файлов
typedef struct {
    char files[MAX_FILES][PATH_LEN];  // массив имён файлов
    int count;                        // текущее количество файлов
} FileList;

// Инициализация списка файлов
void init_list(FileList* list) {
    list->count = 0;
}

// Проверка, есть ли файл уже в списке
int file_exists(FileList* list, const char* filename) {
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->files[i], filename) == 0) {
            return 1;  // файл уже есть
        }
    }
    return 0;  // файла нет
}

// Добавление файла в список (если его ещё нет)
void add_file(FileList* list, const char* filename) {
    if (list->count >= MAX_FILES) {
        printf("Достигнут лимит файлов (%d)\n", MAX_FILES);
        return;
    }
    
    if (!file_exists(list, filename)) {
        strcpy(list->files[list->count], filename);
        list->count++;
    }
}

// Вывод справки
void show_help(const char* prog_name) {
    printf("Использование: %s <каталог> <маска1> [маска2] ...\n", prog_name);
    printf("Пример: %s C:\\test *.txt *.jpg\n", prog_name);
}

int main(int argc, char* argv[]) {
    // Настройка консоли для русских символов
    SetConsoleOutputCP(1251);
    
    // Проверка на запрос справки
    if (argc == 2 && (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "-?") == 0)) {
        show_help(argv[0]);
        return 0;
    }
    
    // Проверка количества аргументов
    if (argc < 3) {
        printf("Ошибка: нужно указать каталог и хотя бы одну маску\n");
        show_help(argv[0]);
        return 1;
    }
    
    FileList found_files;    // список найденных файлов
    init_list(&found_files); // инициализация списка
    
    char search_path[PATH_LEN];  // буфер для полного пути поиска
    
    // Обработка каждой маски (аргументы начиная с 3-го)
    for (int i = 2; i < argc; i++) {
        // Формируем путь: каталог + маска
        sprintf(search_path, "%s\\%s", argv[1], argv[i]);
        
        struct _finddata_t file_info;  // информация о файле
        intptr_t handle;               // дескриптор поиска
        
        // Начинаем поиск по маске
        handle = _findfirst(search_path, &file_info);
        
        // Если файлы найдены
        if (handle != -1) {
            do {
                // Пропускаем папки (нам нужны только файлы)
                if (!(file_info.attrib & _A_SUBDIR)) {
                    add_file(&found_files, file_info.name);
                }
            } while (_findnext(handle, &file_info) == 0);  // пока есть файлы
            
            _findclose(handle);  // закрываем дескриптор поиска
        }
    }
    
    // Вывод результатов
    if (found_files.count > 0) {
        for (int i = 0; i < found_files.count; i++) {
            printf("%s\n", found_files.files[i]);
        }
    } else {
        printf("Файлов не найдено.\n");
    }
    
    return 0;
}
