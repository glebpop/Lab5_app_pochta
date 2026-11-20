cmake_minimum_required(VERSION 3.10)
project(EmailDatabase C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

include_directories(. rln)

# Перечисляем main.c и все 4 файла из папки rln
add_executable(program
    main.c
    rln/is_valid_email.c
    rln/load_db.c
    rln/save_db.c
    rln/func.c
)
