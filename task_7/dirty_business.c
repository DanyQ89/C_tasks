#include "header.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define START_MEMORY 1024
#define MIN_BLOCK_SIZE 16
#define MAX_BLOCKS 1000

int levels = 0;
bool initialized = false;
void* memory_pool = NULL;
size_t pool_size = 0;

FreeList* free_lists = NULL;
FreeList* locked_lists = NULL;

PointerInfo all_pointers[MAX_BLOCKS];
int all_pointers_count = 0;

int calculate_levels(int total_size, int min_size) {
    int levels = 0;
    int size = total_size;
    while (size >= min_size) {
        levels++;
        size /= 2;
    }
    return levels;
}

void init_memory(void) {
    pool_size = make_weight(START_MEMORY);
    memory_pool = malloc(pool_size);
    if (!memory_pool) {
        printf("Memory allocation failed!\n");
        return;
    }

    levels = calculate_levels(pool_size, MIN_BLOCK_SIZE);

    free_lists = (FreeList*)malloc(levels * sizeof(FreeList));
    locked_lists = (FreeList*)malloc(levels * sizeof(FreeList));

    for (int i = 0; i < levels; i++) {
        free_lists[i].head = NULL;
        locked_lists[i].head = NULL;
        free_lists[i].block_size = MIN_BLOCK_SIZE * (1 << i);
        locked_lists[i].block_size = MIN_BLOCK_SIZE * (1 << i);
    }

    // Добавляем весь пул как один свободный блок
    FreeBlock* block = (FreeBlock*)memory_pool;
    block->address = memory_pool;
    block->next = NULL;
    free_lists[levels - 1].head = block;
}

void add_to_list(FreeList* list, void* block_addr) {
    FreeBlock* new_block = (FreeBlock*)block_addr;
    new_block->address = block_addr;
    new_block->next = list->head;
    list->head = new_block;
} // ставлю в начало списка

void* remove_from_list(FreeList* list) { // функция для маллока, первый встречный блок
    if (list->head == NULL) return NULL;
    FreeBlock* block = list->head;
    list->head = block->next;
    return block->address;
} // просто ставлю голову некст элемент и вывожу адрес убранного

void* get_buddy(void* block, int level) {
    size_t block_size = free_lists[level].block_size;
    size_t offset = (size_t)((char*)block - (char*)memory_pool); // находим смещение от нач адреса пула
    size_t buddy_offset = offset ^ block_size; // XORом находим смещение зеркального блока, хор потому что (a ^ b) ^ b = a
    return (char*)memory_pool + buddy_offset; // просто к пулу плюсуем
}

int is_buddy_free(void* buddy, int level) {
    FreeBlock* current = free_lists[level].head;
    while (current != NULL) {
        if (current->address == buddy) {
            return 1;
        }
        current = current->next;
    }
    return 0;
} // чекаем адрес пока не закончатся элы или найдет его в свободных

void remove_from_free_list(int level, void* block) { // функция для баддис, чтобы конкретный блок нашла
    FreeBlock* prev = NULL;
    FreeBlock* current = free_lists[level].head;

    while (current != NULL) {
        if (current->address == block) {
            if (prev == NULL) {
                free_lists[level].head = current->next;
            }
            else {
                prev->next = current->next;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
} // проходимся по элам в массиве и чекаем адрес

void merge_buddies(void* block, int level) {
    if (level >= levels - 1) return;

    void* buddy = get_buddy(block, level);

    if (is_buddy_free(buddy, level)) {

        remove_from_free_list(level, block);
        remove_from_free_list(level, buddy);

        size_t block_offset = (size_t)((char*)block - (char*)memory_pool);
        size_t buddy_offset = (size_t)((char*)buddy - (char*)memory_pool);
        void* parent = (block_offset < buddy_offset) ? block : buddy; // у кого меньше смещение тот и батёк

        int parent_level = level + 1;

        add_to_list(&free_lists[parent_level], parent);

        // рекурсивно пытаемся слиться дальше
        merge_buddies(parent, parent_level);
    }
}

void* malloc_buddy(int size) {
    if (!initialized) {
        init_memory();
        initialized = true;
    }

    int needed_size = make_weight(size);

    int level = -1;
    for (int i = 0; i < levels; i++) {
        if (free_lists[i].block_size >= needed_size) {
            level = i;
            break;
        }
    }

    if (level == -1) {
        printf("Нет подходящего уровня %d\n", needed_size);
        return NULL;
    }


    for (int l = level; l < levels; l++) {
        if (free_lists[l].head != NULL) {

            void* block = remove_from_list(&free_lists[l]);

            // Делим блок если нужно
            while (l > level) {
                void* buddy = (char*)block + free_lists[l - 1].block_size;

                add_to_list(&free_lists[l - 1], buddy);
                l--;
            }

            // Добавляем в занятые
            add_to_list(&locked_lists[level], block);

            // Ищем свободный блок в all_pointers для переиспользования
            int reused_index = -1;
            for (int i = 0; i < all_pointers_count; i++) {
                if (!all_pointers[i].is_allocated && all_pointers[i].ptr == block) {
                    reused_index = i;
                    break;
                }
            }

            if (reused_index != -1) {
                // Переиспользуем существующую запись
                all_pointers[reused_index].size = needed_size;
                all_pointers[reused_index].is_allocated = true;
                printf("Reused pointer[%d] = %p\n", reused_index, block);
            }
            else {
                // Создаем новую запись
                if (all_pointers_count < MAX_BLOCKS) {
                    all_pointers[all_pointers_count].ptr = block;
                    all_pointers[all_pointers_count].size = needed_size;
                    all_pointers[all_pointers_count].is_allocated = true;
                    all_pointers_count++;
                }
                else {
                    printf("Error: Too many pointers allocated\n");
                    return NULL;
                }
            }

            return block;
        }
    }

    printf("Нет свободных блоков на уровне %d\n", level);
    return NULL;
}

void free_buddy(void* ptr) {
    // Проверяем валидность указателя
    if (ptr < memory_pool || ptr >= (char*)memory_pool + pool_size) {
        printf("Error: Invalid pointer %p\n", ptr);
        return;
    }

    // Сначала находим уровень блока
    int block_level = -1;
    for (int level = 0; level < levels; level++) {
        FreeBlock* current = locked_lists[level].head;
        while (current != NULL) {
            if (current->address == ptr) {
                block_level = level;
                break;
            }
            current = current->next;
        }
        if (block_level != -1) break;
    }

    if (block_level == -1) {
        printf("Error: Блок не найден в фри листах: %p\n", ptr);
        return;
    }

    // Удаляем из занятых
    FreeBlock* prev = NULL;
    FreeBlock* current = locked_lists[block_level].head;
    while (current != NULL) {
        if (current->address == ptr) {
            if (prev == NULL) {
                locked_lists[block_level].head = current->next;
            }
            else {
                prev->next = current->next;
            }
            break;
        }
        prev = current;
        current = current->next;
    }

    // Добавляем в свободные
    add_to_list(&free_lists[block_level], ptr);

    // Обновляем существующую запись
    for (int i = 0; i < all_pointers_count; i++) {
        if (all_pointers[i].ptr == ptr) {
            all_pointers[i].is_allocated = false;
            break;
        }
    }

    // Пытаемся слиться с buddy
    merge_buddies(ptr, block_level);
}

int make_weight(int num) {
    if (num <= 0) return MIN_BLOCK_SIZE;
    if ((num & (num - 1)) == 0) return num;

    int bit = 0;
    int temp = num;
    while (temp >>= 1) bit++;
    return 1 << (bit + 1);
}

void print_memory_state() {
    printf("\n=================================================\n");
    printf("              BUDDY SYSTEM MEMORY MAP\n");
    printf("=================================================\n");

    // Печатаем свободные блоки
    printf("\nFREE BLOCKS:\n");
    printf("+---------+----------+------------+--------------+\n");
    printf("|  Level  |   Size   |   Blocks   |   Addresses  |\n");
    printf("+---------+----------+------------+--------------+\n");

    int total_free = 0;
    for (int i = 0; i < levels; i++) {
        int count = 0;
        FreeBlock* current = free_lists[i].head;
        printf("|   %2d    | %6d B |", i, free_lists[i].block_size);

        char addresses[100] = "";
        while (current != NULL) {
            count++;
            if (count <= 3) {
                char addr_str[20];
                sprintf(addr_str, " %p", current->address);
                strcat(addresses, addr_str);
            }
            current = current->next;
        }
        total_free += count * free_lists[i].block_size;

        if (count > 3) {
            sprintf(addresses + strlen(addresses), " +%d more", count - 3);
        }

        if (count == 0) {
            printf("     -      |      -       |\n");
        }
        else {
            printf("    %2d      |%-14s|\n", count, addresses);
        }
    }
    printf("+---------+----------+------------+--------------+\n");
    printf("Total free: %d bytes\n", total_free);

    // Печатаем занятые блоки
    printf("\nALLOCATED BLOCKS:\n");
    int total_allocated = 0;
    int active_blocks = 0;

    if (all_pointers_count == 0) {
        printf("No allocated blocks\n");
    }
    else {
        printf("+------+----------+------------+--------------+\n");
        printf("| Index|   Size   |   Address  |    Status    |\n");
        printf("+------+----------+------------+--------------+\n");

        for (int i = 0; i < all_pointers_count; i++) {
            if (all_pointers[i].is_allocated) {
                printf("|  %2d  | %6d B | %p |   ALLOCATED  |\n",
                    i, all_pointers[i].size, all_pointers[i].ptr);
                total_allocated += all_pointers[i].size;
                active_blocks++;
            }
            else {
                printf("|  %2d  | %6d B | %p |    FREED     |\n",
                    i, all_pointers[i].size, all_pointers[i].ptr);
            }
        }
        printf("+------+----------+------------+--------------+\n");
        printf("Total allocated: %d bytes (%d active blocks)\n", total_allocated, active_blocks);
    }

    // Общая статистика
    printf("\nMEMORY USAGE:\n");
    int total_memory = pool_size;
    int used_percent = total_memory > 0 ? (total_allocated * 100) / total_memory : 0;
    int free_percent = total_memory > 0 ? (total_free * 100) / total_memory : 0;

    printf("Total pool: %d bytes\n", total_memory);
    printf("-- Allocated: %d bytes (%d%%)\n", total_allocated, used_percent);
    printf("-- Free:      %d bytes (%d%%)\n", total_free, free_percent);
    printf("-- Internal:  %d bytes (%d%%)\n",
        total_memory - total_allocated - total_free,
        100 - used_percent - free_percent);

    // Progress bar
    printf("\n[");
    int bars = used_percent / 2;
    for (int i = 0; i < 50; i++) {
        if (i < bars) printf("#");
        else printf(" ");
    }
    printf("] %d%% used\n", used_percent);

    printf("=================================================\n\n");
}

void solve_the_line(char* line) {
    char command[20] = { 0 };
    int size;

    // Для команд с числом (malloc, free)
    if (sscanf(line, "%19s %d", command, &size) == 2) {
        if (strcmp(command, "malloc") == 0) {
            void* ptr = malloc_buddy(size);
        }
        else if (strcmp(command, "free") == 0) {
            if (size >= 0 && size < all_pointers_count && all_pointers_count != 0) {
                if (all_pointers[size].is_allocated) {
                    free_buddy(all_pointers[size].ptr);
                }
                else {
                    printf("Error: pointer[%d] уже свободен\n", size);
                }
            }
            else {
                printf("Error:неправильный свободный индекс %d (max: %d)\n", size, all_pointers_count - 1);
            }
        }
    }
    // Для команд без числа (print)
    else if (sscanf(line, "%19s", command) == 1) {
        if (strcmp(command, "print") == 0) {
            print_memory_state();
        }
    }

}
