#include <stdio.h>
#include "function.h"

int load_db(EmailList *list, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Файл %s не найден.\n", filename);
        return -1;
    }

    int id;
    char sender[MAX_STR];
    char recipient[MAX_STR];
    char subject[MAX_STR];
    char text[MAX_TEXT];

    while (fscanf(file, "%d|%2047[^|]|%2047[^|]|%2047[^|]|%8191[^\n]\n", &id, sender, recipient, subject, text) == 5) {//читает 5 полей через '|': %2047[^|] читает строку (включая пробелы) до символа '|'
	//%8191[^\n] — текст письма до переноса строки; == 5 проверяет, что считались все 5 полей
        if (is_valid_email(sender) && is_valid_email(recipient)) {//проверка на ошибки через валидатор
            append_email(list, id, sender, recipient, subject, text);//если все ок выделяет память под новое письмо
        }
    }

    fclose(file);
    printf("Загружено писем: %d\n", list->n);//обновление указателя до n письма
    return 0;
}
