cmake_minimum_required(VERSION 3.10)
project(afile C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CORE_ROOT "${CMAKE_SOURCE_DIR}/../../")

add_subdirectory(${CORE_ROOT} core_build)

add_executable(afile
    main.c
    src/bash.c
    src/write.c
    src/terminal.c
)

target_include_directories(afile PRIVATE
    include
)

target_link_libraries(afile PRIVATE
    core
)