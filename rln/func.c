#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


// функция меню
void func(Database *db, const char *filename) {
    int n;

    printf("\n--- МЕНЮ ---\n");
    printf("1. Показать все\n2. Поиск\n3. Добавить\n4. Удалить\n5. Редактировать\n0. Выход\n");
    printf("Выбор: ");

    if (scanf("%d", &n) != 1) { // защита от ввода букв
        while (getchar() != '\n'); // чистим буфер от мусора
        n = -1; // ставим кривое значение
    }

    switch (n) {
        case 0:
            printf("Выход...\n");
            return; // тормозим рекурсию и выходим в main

        case 1:
            if (db->count == 0) printf("База пуста.\n");
            for (int i = 0; i < db->count; i++) {
                printf("ID: %d | От: %s | Кому: %s | Тема: %s\n",
                       db->data[i].id, db->data[i].sender,
                       db->data[i].recipient, db->data[i].subject);
            }
            break;

        case 2: {
            char search_sender[256], search_recipient[256];
            int found = 0;
            printf("Отправитель (или *): "); scanf("%255s", search_sender);
            printf("Получатель (или *): "); scanf("%255s", search_recipient);

            for (int i = 0; i < db->count; i++) {
                // если ввели * - пропускаем проверку, иначе ищем подстроку
                int match_s = (strcmp(search_sender, "*") == 0) || (strstr(db->data[i].sender, search_sender) != NULL);
                int match_r = (strcmp(search_recipient, "*") == 0) || (strstr(db->data[i].recipient, search_recipient) != NULL);

                if (match_s && match_r) {
                    printf("ID: %d | От: %s | Кому: %s | Тема: %s\n",
                           db->data[i].id, db->data[i].sender, db->data[i].recipient, db->data[i].subject);
                    found = 1;
                }
            }
            if (found == 0) printf("Не найдено.\n");
            break;
        }

        // 3. ДОБАВЛЕНИЕ
        case 3: {
            Email new_email; // временная структура под новое письмо
            while (getchar() != '\n'); // вычищаем \n из буфера после scanf, иначе fgets пролетит мимо

            printf("Отправитель: ");
            fgets(new_email.sender, MAX_STR, stdin);
            new_email.sender[strcspn(new_email.sender, "\n")] = '\0'; // отрезаем перенос строки

            printf("Получатель: ");
            fgets(new_email.recipient, MAX_STR, stdin);
            new_email.recipient[strcspn(new_email.recipient, "\n")] = '\0';

            // прогоняем адреса через нашу гордость
            if (!is_valid_email(new_email.sender) || !is_valid_email(new_email.recipient)) {
                printf("Ошибка: кривой email. Добавление отменено.\n");
                break;
            }

            printf("Тема: ");
            fgets(new_email.subject, MAX_STR, stdin);
            new_email.subject[strcspn(new_email.subject, "\n")] = '\0';

            printf("Текст письма: ");
            fgets(new_email.text, MAX_TEXT, stdin);
            new_email.text[strcspn(new_email.text, "\n")] = '\0';

            // ищем самый жирный айдишник в базе, чтобы дать новому +1
            int max_id = 0;
            for (int i = 0; i < db->count; i++) {
                if (db->data[i].id > max_id) {
                    max_id = db->data[i].id;
                }
            }
            new_email.id = max_id + 1;

            // реаллочим массив под +1 письмо (через temp, чтоб не потерять старые данные если память кончится)
            Email *temp = (Email*)realloc(db->data, (db->count + 1) * sizeof(Email));
            if (temp == NULL) {
                printf("Ошибка памяти, некуда пихать письмо!\n");
                break;
            }
            db->data = temp;

            // кладем письмо тупо в самый конец и плюсуем счетчик
            db->data[db->count] = new_email;
            db->count++;

            save_db(db, filename); // сразу сохраняем в файл
            printf("Письмо добавлено (ID: %d).\n", new_email.id);
            break;
        }

        // 4. УДАЛЕНИЕ
        case 4: {
            if (db->count == 0) {
                printf("База и так пустая, удалять нечего.\n");
                break;
            }

            int del_id;
            printf("Введите ID письма для удаления: ");
            if (scanf("%d", &del_id) != 1) {
                while (getchar() != '\n'); // опять чистим буфер
                printf("Неверный ввод ID.\n");
                break;
            }

            // ищем где в массиве прячется этот айдишник
            int found_idx = -1; // минус один = пока не нашли
            for (int i = 0; i < db->count; i++) {
                if (db->data[i].id == del_id) {
                    found_idx = i;
                    break;
                }
            }

            if (found_idx == -1) {
                printf("Письмо с ID %d не найдено.\n", del_id);
                break;
            }

            // затираем удаляемое письмо, сдвигая все остальные влево (айдишники не трогаем)
            for (int i = found_idx; i < db->count - 1; i++) {
                db->data[i] = db->data[i + 1];
            }
            db->count--; // писем стало на одно меньше

            // подрезаем лишнюю память в конце
            if (db->count == 0) {
                free(db->data); // если удалили вообще всё - чистим полностью
                db->data = NULL;
            } else {
                Email *temp = (Email*)realloc(db->data, db->count * sizeof(Email));
                if (temp != NULL) {
                    db->data = temp;
                }
            }

            save_db(db, filename);
            printf("Письмо с ID %d удалено.\n", del_id);
            break;
        }

        // 5. РЕДАКТИРОВАНИЕ
        case 5: {
            if (db->count == 0) {
                printf("База пуста.\n");
                break;
            }

            int edit_id;
            printf("Введите ID письма для редактирования: ");
            if (scanf("%d", &edit_id) != 1) {
                while (getchar() != '\n');
                printf("Неверный ввод ID.\n");
                break;
            }

            // ищем индекс нужной структуры
            int idx = -1;
            for (int i = 0; i < db->count; i++) {
                if (db->data[i].id == edit_id) {
                    idx = i;
                    break;
                }
            }

            if (idx == -1) {
                printf("Письмо с ID %d не найдено.\n", edit_id);
                break;
            }

            int field;
            printf("Какое поле поменять? (1 - Отправитель, 2 - Получатель, 3 - Тема, 4 - Текст): ");
            if (scanf("%d", &field) != 1) {
                while (getchar() != '\n');
                printf("Неверный выбор.\n");
                break;
            }
            while (getchar() != '\n'); // убираем \n перед fgets

            char buf[MAX_TEXT]; // буфер под новое значение
            switch (field) {
                case 1:
                    printf("Новый отправитель: ");
                    fgets(buf, MAX_STR, stdin);
                    buf[strcspn(buf, "\n")] = '\0';
                    if (is_valid_email(buf)) {
                        strcpy(db->data[idx].sender, buf); // заменяем поле в структуре
                    } else {
                        printf("Брак: кривой email. Отмена.\n");
                    }
                    break;
                case 2:
                    printf("Новый получатель: ");
                    fgets(buf, MAX_STR, stdin);
                    buf[strcspn(buf, "\n")] = '\0';
                    if (is_valid_email(buf)) {
                        strcpy(db->data[idx].recipient, buf);
                    } else {
                        printf("Брак: кривой email. Отмена.\n");
                    }
                    break;
                case 3:
                    printf("Новая тема: ");
                    fgets(buf, MAX_STR, stdin);
                    buf[strcspn(buf, "\n")] = '\0';
                    strcpy(db->data[idx].subject, buf);
                    break;
                case 4:
                    printf("Новый текст: ");
                    fgets(buf, MAX_TEXT, stdin);
                    buf[strcspn(buf, "\n")] = '\0';
                    strcpy(db->data[idx].text, buf);
                    break;
                default:
                    printf("Нет такого поля.\n");
                    break;
            }

            save_db(db, filename);
            printf("Обновлено.\n");
            break;
        }

        default:
            printf("Неверный ввод! Введите 0-5.\n"); // на месте
            break;
    }

    // рекурсивно вызываем меню снова, чтобы прога не закрывалась
    func(db, filename);
}
