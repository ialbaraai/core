cmake_minimum_required(VERSION 3.10)
project(core C)

add_library(core STATIC
    src/core/core_string.c
    src/containers/core_vector.c
    src/containers/core_map.c
    src/oop/core_oop_class.c
    src/oop/core_oop_object.c
    src/oop/core_oop_method.c
    src/oop/core_oop_member.c
    # src/containers/core_linkedlist.c
    src/core/core_file.c
)

set_target_properties(core PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
