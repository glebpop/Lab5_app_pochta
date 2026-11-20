#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


// функция проверки email
int is_valid_email(const char *email) {
    if (email == NULL || strlen(email) == 0) return 0; // если пусто - сразу брак
    
    int count_at = 0; // сколько собачек
    int at_index = -1; // где собачка (минус один = не нашли)
    int last_dot_index = -1; // где последняя точка
    
    // бежим по строке до нуля
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') { 
            count_at++; 
            at_index = i; // запомнили где собачка
        }
        if (email[i] == '.') { 
            last_dot_index = i; // перезаписываем, чтобы осталась последняя
        }
    }
    
    if (count_at != 1) return 0; // собачка должна быть ровно одна
    if (last_dot_index == -1) return 0; // точка должна быть
    if (last_dot_index < at_index) return 0; // точка не может быть до собачки
    if (at_index == 0 || last_dot_index == (int)strlen(email) - 1) return 0; // собачка не в начале, точка не в конце
    return 1; // если прошли все проверки - ок
}

