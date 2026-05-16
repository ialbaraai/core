/*
 * core_file.h - Core Library
 *
 * Copyright (c) 2026 albaraa (ialbaraai)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 */


 #ifndef CORE_FILE_H

#define CORE_FILE_H

#include "core_vector.h"
#include "core_string.h"

typedef struct FILE_STRUCT
{
    string_t _File_Filepath; // file `filepath`, use `core_file_get_filepath(const file_t* file)` for safe getter
    vector_t _File_Data; // file `data` (lines), use `core_file_get_lines(const file_t* file)` for safe getter
    size_t _File_Initial_Line_Size; // file `line size`, use `core_file_get_line_size(const file_t* file)` for safe getter
} file_t;

// FILE INITIALIZATION
file_t core_file_init(const char* filepath, const size_t initial_lines_size, const size_t initial_line_size); // Initialize a file struct with a filepath of `filepath`, initial file lines capacity of `initial_lines_size`, and initial line capacity of `initial_line_size`

// FILE MANIPULATION
int core_file_read(file_t* file); // Reading file `filepath` data into file `data` vector, returns 1 on success, and 0 on failure
int core_file_write(const file_t* file); // Writing file `filepath` data using file `data`, returns 1 on success, and 0 on failure

// GETTERS
string_t* core_file_get_filepath(const file_t* file); // Getter for file `filepath`, use `core_string_set(string_t* filepath, const char* new_path)` to adjust `file` filepath
vector_t* core_file_get_lines(const file_t* file); // Getter for file `data`
size_t core_file_get_line_size(const file_t* file); // Getter for file `line size`

// FILE DESTRUCTOR
void core_file_destroy(file_t* file); // Free `file` allocated `filepath` and `data` memory and clear its data

#endif