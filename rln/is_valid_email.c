#include <string.h>
#include "function.h"

int is_valid_email(const char *email) {//логика: проверка на наличие собачки(1) и точки после собачки
    int at_count = 0;
    int at_pos = -1;
    int dot_pos = -1;
    int len = strlen(email);

    if (len < 5) {//защита от @l.u
        return 0;
    }

    for (int i = 0; i < len; i++) {//считаем что там у нас
        if (email[i] == ' ' || email[i] == '|') {//проверка на валидность, во первых пробелов не будет, во вторых структура не нарушенна | разделитель а не часть емейла.
            return 0;
        }
        if (email[i] == '@') {
            at_count++;
            at_pos = i;
        }
        if (email[i] == '.') {
            dot_pos = i;
        }
    }

    if (at_count != 1 || dot_pos == -1) { //защита от точки за собачкой
        return 0;
    }
    if (at_pos == 0 || dot_pos <= at_pos + 1 || dot_pos == len - 1) {//защита от собачки на первом месте, точки на последнем и точки где то за и сразу после собачки
        return 0;
    }

    return 1;
}

