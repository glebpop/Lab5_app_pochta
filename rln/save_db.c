#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


// функция сохранения
int save_db(Database *db, const char *filename) {
    FILE *file = fopen(filename, "w"); // открываем на запись (старое сотрется)
    if (file == NULL) { 
        printf("Ошибка записи.\n"); 
        return -1; 
    }
    
    // бежим по массиву и пишем в том же формате
    for (int i = 0; i < db->count; i++) {
        fprintf(file, "%d|%s|%s|%s|%s\n", db->data[i].id, db->data[i].sender,
                db->data[i].recipient, db->data[i].subject, db->data[i].text);
    }
    fclose(file); // закрыли
    printf("База сохранена.\n");
    return 0;
}

