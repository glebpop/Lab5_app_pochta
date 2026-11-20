#include <stdio.h>
#include <stdlib.h>
#include <string.h> // подключаем для работы со строками (strlen, strcpy, strstr, strtok)
#include "function.h"


// главная функция
int main() {
    Database db;
    db.data = NULL;
    db.count = 0;

    char filename[4096]; // буфер под имя файла (динамика не нужна, это короткая строка)
    printf("Введите имя файла базы (например, mail_db.txt): ");
    fgets(filename, sizeof(filename), stdin); // читаем безопасно
    filename[strcspn(filename, "\n")] = '\0'; // убираем \n

    printf("Загрузка...\n");
    int result = load_db(&db, filename);

    // проверяем коды ошибок
    if (result == -1) { 
        printf("Ошибка: файл не найден.\n"); 
        return 1; 
    }
    if (result == -2) { 
        printf("Ошибка: не хватило памяти.\n"); 
        return 1; 
    }

    func(&db, filename); // запускаем меню, передаем базу и имя файла

    free(db.data); // освобождаем память перед выходом
    printf("Память очищена. Выход.\n");
    return 0;
}

