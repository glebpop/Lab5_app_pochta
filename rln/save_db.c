#include <stdio.h>
#include "function.h"

int save_db(EmailList *list, const char *filename) {//принимает указатель на список и имя файла const защита
    FILE *file = fopen(filename, "w");//открывается файл в режиме записи старый файл стирается и создается новый(если такого нету)
    if (file == NULL) {
        printf("Не удалось открыть файл для записи.\n");
        return -1;
    }

    Email *current = list->first;
    while (current != NULL) {//циклом записываем
        fprintf(file, "%d|%s|%s|%s|%s\n", //записывает не в терминал а в файл, сохраняются все поля письма
                current->id,
                current->sender,
                current->recipient,
                current->subject,
                current->text);
        current = current->next;//следующий узел
    }

    fclose(file);//ползволяет записать файл из оперативки на диск, без него файл не сохранится
    return 0;
}

