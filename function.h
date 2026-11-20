#ifndef FUCTION_H
#define FUCTION_H

#define MAX_STR 2048 // макс длина строки для email и темы
#define MAX_TEXT 8192 // макс длина текста письма

// структура одного письма
typedef struct {
    int id; // айдишник
    char sender[MAX_STR]; // от кого
    char recipient[MAX_STR]; // кому
    char subject[MAX_STR]; // тема
    char text[MAX_TEXT]; // текст
} Email;

// структура базы (обертка над массивом)
typedef struct {
    Email *data; // сюда будем маллочить массив писем
    int count; // сколько сейчас писем
} Database;

// прототипы наших функций
int is_valid_email(const char *email);
int load_db(Database *db, const char *filename);
int save_db(Database *db, const char *filename);
void func(Database *db, const char *filename);

#endif // FUCTION_H
