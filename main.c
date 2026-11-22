#include <stdio.h>
#include <stdlib.h>
#include "function.h"//заголовочный файл

int main() {
    EmailList db = { NULL, NULL, 0 }; //создается список в формате начало конец количество, null заглушка(место под будущий кусочек базы)
    char filename[256];//место под название файла \0 - 256 место

    printf("Введите имя файла базы (например, base.txt): ");
    if (scanf("%255s", filename) != 1) { //функция вернет 1 при успешном выполнении
        return 1;
    }

    if (load_db(&db, filename) != 0) {//там проверяется загрузилась ли база и тп
        return 1;
    }

    func(&db, filename); //вызов рекурсии, выход 0

    //освобождение цепочки узлов(их много) списка перед выходом
    Email *current = db.first;//указатель на первый элемент
    while (current != NULL) {//цикл пока значение не обнулится
        Email *next = current->next;//перед удалением узла сохраняем адрес
        free(current);
        current = next;//переход к следующему элементу
    }

    return 0;
}

