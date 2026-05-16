cmake_minimum_required(VERSION 3.10)
project(core C)

set_target_properties(core PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

add_library(core STATIC
    src/core_string.c
    src/core_vector.c
    src/core_map.c
    # src/core_oop.c
    # src/core_linkedlist.c
    src/core_file.c
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)