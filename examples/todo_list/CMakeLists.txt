cmake_minimum_required(VERSION 3.10)
project(todo_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CORE_ROOT "${CMAKE_SOURCE_DIR}/../../")

add_executable(todo_app
    main.c
    src/tasks.c
)

target_include_directories(todo_app PRIVATE
    include
    ${CORE_ROOT}/include
)

target_link_libraries(todo_app PRIVATE
    ${CORE_ROOT}/build/libcore.a
)
