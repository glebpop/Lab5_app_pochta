cmake_minimum_required(VERSION 3.10)
project(EmailDatabase C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)
#Задает список папок, где компилятор ищет заголовочные
include_directories(. rln) #По какой то причине заголовочный файл искался в папке с функциями, так что я добавил директорию с функциями чтобы компилятор все же обнаружил файлы.

# Перечисляем main.c и все 4 файла из папки rln
add_executable(program #Имя исполняемого файла
    main.c
    rln/is_valid_email.c
    rln/load_db.c
    rln/save_db.c
    rln/func.c
)
