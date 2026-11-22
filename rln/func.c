#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
//все кейсы реализованны как отдельные функции
void clear_buffer() { //защита от зацикливания. чтобы буквы не сломали меню
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//добавление нового письма в конец списка
void append_email(EmailList *list, int id, const char *sender, const char *recipient, const char *subject, const char *text) {
    Email *node = (Email *)calloc(1, sizeof(Email));//создание письма
    if (node == NULL) {
        printf("Ошибка памяти.\n");
        return;
    }

    node->id = id;//заполнение письма
    strcpy(node->sender, sender);
    strcpy(node->recipient, recipient);
    strcpy(node->subject, subject);
    strcpy(node->text, text);
    node->next = NULL;

    if (list->first == NULL) {
        list->first = node;//добавляется как бы 2 письма, ведь если такого не сделать будет падение
        list->last = node;
    } else {
        list->last->next = node;//берется последний элемент, за ним привязывается новое письмо,  сдвигаем указатель на новый элемент
        list->last = node;
    }
    list->n++;
}

//поиск узла по ID
Email *get_email_by_id(EmailList *list, int id) {
    Email *current = list->first;
    while (current) {
        if (current->id == id) {//ищет айдишник, как только нашли выводим
            return current;
        }
        current = current->next;
    }
    return NULL;
}

//удаление узла по ID
int del_email_by_id(EmailList *list, int id) {
    if (list->first == NULL) {
        return 0;
    }

    if (list->first->id == id) {//запоминаем удалемое письмо (в данном случае если удалено первое)
        Email *temp = list->first;//происходит сдвиг, и обнуление конца если элемент один
        list->first = temp->next;
        if (list->first == NULL) {
            list->last = NULL;
        }
        list->n--;
        free(temp);
        return 1;
    }

    Email *prev = list->first;//ищется айди стоящий перед удаленным
    while (prev->next && prev->next->id != id) {//так сказать стрелка уже не 1-2-3 а 1-3
        prev = prev->next;
    }

    if (prev->next == NULL) {
        return 0;
    }

    Email *current = prev->next;//то что удаляется
    prev->next = current->next;//стрелка перемещается в обход удаленного
    if (current == list->last) {//сдвиг хвоста при удалении последнего
        list->last = prev;
    }
    list->n--;
    free(current);
    return 1;
}

void print_emails(EmailList *list) {
    if (!list->first) {
        printf("База пуста.\n");//очередная проверка
        return;
    }
    Email *current = list->first;//проходка по всем полям письма и его печать
    while (current) {
        printf("ID: %d | От: %s | Кому: %s | Тема: %s\n", current->id, current->sender, current->recipient, current->subject);
        current = current->next;//переход к следующему
    }
}
//можно воспользоваться * тоесть отключить опредленный поиск по отправителю/получателю
void search_emails(EmailList *list) {
    char sender[MAX_STR], recipient[MAX_STR];
    int found = 0;//флаг: 0 — совпадений пока нет, 1 — хотя бы одно письмо найдено

    printf("Отправитель (или *): ");
    scanf("%2047s", sender);

    printf("Получатель (или *): ");
    scanf("%2047s", recipient);

    Email *current = list->first;
    while (current) {
        int ok_s = (strcmp(sender, "*") == 0) || (strstr(current->sender, sender) != NULL); //сравнение, если * то да, если совпадение то да
        int ok_r = (strcmp(recipient, "*") == 0) || (strstr(current->recipient, recipient) != NULL);

        if (ok_s && ok_r) {
            printf("ID: %d | От: %s | Кому: %s | Тема: %s\n", current->id, current->sender, current->recipient, current->subject);//все совпало то выводим
            found = 1;
        }
        current = current->next;
    }
    if (found == 0) {
        printf("Ничего не найдено.\n");
    }
}
//создание нового письма
void add_email_ui(EmailList *list, const char *filename) {
    char sender[MAX_STR], recipient[MAX_STR], subject[MAX_STR], text[MAX_TEXT];

    printf("Отправитель: ");
    scanf("%2047s", sender);

    printf("Получатель: ");
    scanf("%2047s", recipient);

    if (!is_valid_email(sender) || !is_valid_email(recipient)) {
        printf("Ошибка: неверный email.\n");
        return;
    }

    printf("Тема: ");
    scanf(" %2047[^\n]", subject);

    printf("Текст: ");
    scanf(" %8191[^\n]", text);

    int new_id = (list->last) ? (list->last->id + 1) : 1; //создание номера письма
    append_email(list, new_id, sender, recipient, subject, text);////если список не пуст, берём ID последнего письма + 1; иначе начинаем с 1
    save_db(list, filename);//сохранение в файл
    printf("Письмо добавлено (ID: %d).\n", new_id);
}
//удаление письма, защита от некоректного
void delete_email_ui(EmailList *list, const char *filename) {
    int id;
    printf("Введите ID для удаления: ");
    if (scanf("%d", &id) != 1) {
        clear_buffer();
        printf("Неверный ввод.\n");
        return;
    }

    if (del_email_by_id(list, id)) {//удаление
        save_db(list, filename);
        printf("Письмо удалено.\n");
    } else {
        printf("Письмо с ID %d не найдено.\n", id);
    }
}
//редактирование письма
void edit_email_ui(EmailList *list, const char *filename) {
    int id;
    printf("Введите ID для редактирования: ");
    if (scanf("%d", &id) != 1) {//проерки
        clear_buffer();
        printf("Неверный ввод.\n");
        return;
    }

    Email *item = get_email_by_id(list, id);//нахождение письма по айди
    if (item == NULL) {
        printf("Письмо с ID %d не найдено.\n", id);
        return;
    }

    int field;
    printf("Поле (1-Отправитель, 2-Получатель, 3-Тема, 4-Текст): ");
    if (scanf("%d", &field) != 1) {
        clear_buffer();
        printf("Неверный выбор.\n");
        return;
    }

    char buf[MAX_TEXT];
    printf("Новое значение: ");
    scanf(" %8191[^\n]", buf);

    if (field == 1) {//заполнение письма
        if (!is_valid_email(buf)) { printf("Неверный email.\n"); return; }
        strcpy(item->sender, buf);
    } else if (field == 2) {
        if (!is_valid_email(buf)) { printf("Неверный email.\n"); return; }
        strcpy(item->recipient, buf);
    } else if (field == 3) {
        strcpy(item->subject, buf);
    } else if (field == 4) {
        strcpy(item->text, buf);
    } else {
        printf("Нет такого поля.\n");
        return;
    }

    save_db(list, filename);
    printf("Обновлено.\n");
}

void func(EmailList *list, const char *filename) {
    int choice = -1;
//реализация интерфейса
    printf("\n--- МЕНЮ (всего писем: %d) ---\n", list->n);
    printf("1. Показать все\n");
    printf("2. Поиск\n");
    printf("3. Добавить\n");
    printf("4. Удалить\n");
    printf("5. Редактировать\n");
    printf("0. Выход\n");
    printf("Выбор: ");

    if (scanf("%d", &choice) != 1) {//чистка буфера(не вводите буквы пожалуйста)
        clear_buffer();
        choice = -1;
    }
//реализация свича и выборов
    switch (choice) {
        case 0:
            printf("Выход...\n");
            return;
        case 1:
            print_emails(list);
            break;
        case 2:
            search_emails(list);
            break;
        case 3:
            add_email_ui(list, filename);
            break;
        case 4:
            delete_email_ui(list, filename);
            break;
        case 5:
            edit_email_ui(list, filename);
            break;
        default:
            printf("Неверный пункт меню.\n");
            break;
    }

    func(list, filename);
}
