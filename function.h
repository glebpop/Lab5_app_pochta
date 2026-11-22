#ifndef FUCTION_H
#define FUCTION_H

#define MAX_STR 2048 // макс длина строки для email и темы
#define MAX_TEXT 8192 // макс длина текста письма

// структура одного письма
typedef struct Email {
    int id;
    char sender[MAX_STR];
    char recipient[MAX_STR];
    char subject[MAX_STR];
    char text[MAX_TEXT];
    struct Email *next;
} Email;

//Хранит количество писем и первое/последнее письмо
typedef struct EmailList {
    Email *first;
    Email *last;
    int n;
} EmailList;

// прототипы наших функций
int is_valid_email(const char *email);
void append_email(EmailList *list, int id, const char *sender, const char *recipient, const char *subject, const char *text);
int load_db(EmailList *list, const char *filename);
int save_db(EmailList *list, const char *filename);
void func(EmailList *list, const char *filename);

#endif // FUCTION_H
