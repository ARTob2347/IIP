cmake_minimum_required(VERSION 3.10)
project(A1)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Добавляем путь к заголовочным файлам
include_directories(components/declaration)

# Собираем исполняемый файл
add_executable(TEST 
    components/definition/code_generator.cpp
    main.cpp
)

# Если нужны дополнительные библиотеки
# target_link_libraries(my_program some_library)