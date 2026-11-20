#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


int load_db(Database *db, const char *filename) {
    FILE *file = fopen(filename, "r"); // открываем только читать
    if (file == NULL) { 
        printf("Ошибка: файл не найден.\n"); 
        return -1; // код ошибки
    }

    int max_len = 0; // самая длинная строка
    int current_len = 0; // длина текущей
    int line_count = 0; // сколько строк
    int c; // переменная под символ

    // ПЕРВЫЙ ПРОХОД: читаем по символам, чтобы узнать сколько памяти надо
    while ((c = fgetc(file)) != EOF) { // пока не конец файла
        if (c == '\n') { // если перенос строки
            if (current_len > max_len) max_len = current_len; // обновляем максимум
            line_count++; // плюс строка
            current_len = 0; // сброс длины
        } else { 
            current_len++; // просто считаем символы
        }
    }
    // если файл кончился без \n в конце, обрабатываем остаток
    if (current_len > 0) {
        if (current_len > max_len) max_len = current_len;
        line_count++;
    }
    fclose(file); // закрыли файл после первого чтения

    if (line_count == 0) { // если файл пустой
        db->data = NULL; 
        db->count = 0; 
        return 0; 
    }

    // выделяем память под массив структур (ровно сколько строк)
    db->data = (Email*)malloc(line_count * sizeof(Email)); 
    if (db->data == NULL) { 
        printf("Ошибка памяти.\n"); 
        return -2; 
    }
    db->count = 0; // пока ничего не загрузили

    // выделяем динамический буфер под строку (макс длина + 2 под \n и \0)
    char *buffer = (char*)malloc((max_len + 2) * sizeof(char));
    if (buffer == NULL) { 
        free(db->data); 
        return -2; 
    }

    // ВТОРОЙ ПРОХОД: открываем заново и загружаем
    file = fopen(filename, "r");
    if (file == NULL) { // если файл пропал
        free(db->data); 
        free(buffer); 
        return -1; 
    }

    // читаем строки в наш динамический буфер
    while (fgets(buffer, max_len + 2, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // убираем \n в конце
        
        if (strlen(buffer) > 0) { // если не пустая
            // режем строку по |
            char *id_str = strtok(buffer, "|");
            char *sender = strtok(NULL, "|");
            char *recipient = strtok(NULL, "|");
            char *subject = strtok(NULL, "|");
            char *text = strtok(NULL, "|");

            // проверяем что все 5 кусковна месте
            if (id_str != NULL && sender != NULL && recipient != NULL && subject != NULL && text != NULL) {
                // проверяем email
                if (is_valid_email(sender) == 1 && is_valid_email(recipient) == 1) {
                    db->data[db->count].id = atoi(id_str); // строку в число
                    strcpy(db->data[db->count].sender, sender); // копируем строки
                    strcpy(db->data[db->count].recipient, recipient);
                    strcpy(db->data[db->count].subject, subject);
                    strcpy(db->data[db->count].text, text);
                    db->count++; // плюс письмо
                } else { 
                    printf("Внимание: неверный email пропущен.\n"); 
                }
            } else { 
                printf("Внимание: битая строка пропущена.\n"); 
            }
        }
    }
    fclose(file); 
    free(buffer); // буфер больше не нужен
    printf("Загружено писем: %d\n", db->count);
    return 0;
}
