cmake_minimum_required(VERSION 3.10)
project(contact_book_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CORE_ROOT "${CMAKE_SOURCE_DIR}/../../")

add_subdirectory(${CORE_ROOT} core_build)

add_executable(contact_book_app
    main.c
    src/contact_book.c
)

target_include_directories(contact_book_app PRIVATE
    include
)

target_link_libraries(contact_book_app PRIVATE
    core
)