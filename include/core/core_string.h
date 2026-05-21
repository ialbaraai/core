/*
 * core_string.h - Core Library
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


#ifndef CORE_STRING_H

#define CORE_STRING_H

#include <stddef.h>

#define STRING_MAX_CAPACITY (1024ULL * 1024ULL * 1024ULL)

typedef struct STRING_STRUCT
{
    char* _String_Data; // string `data`, use `core_string_get_data(const string_t* string)` for safe getter
    size_t _String_Capacity; // string `capacity`, use `core_string_get_capacity(const string_t* string)` for safe getter
    size_t _String_Size; // string `size` (length), use `core_string_get_size(const string_t* string)` for safe getter
} string_t;

// STRING INITIALIZATION
string_t core_string_init(const size_t capacity); // Initialize a string struct with a capacity of `capacity`
string_t core_string_init_data(const size_t capacity, const char* data); // Initialize a string struct with a capacity of `capacity` and content of `data`, truncates content to fit `capacity`

// STRING MANIPULATIONS
int core_string_set(string_t* string, const char* data); // Setting string content to `data`, returns 1 on success, and 0 on failure
int core_string_copy(string_t* destination, const string_t* source); // Copying `source` content into `destination` content, returns 1 on success, and 0 on failure
int core_string_compare(const string_t* first, const string_t* second); // Comparing the content of `first` and `second`, returns 1 if same

int core_string_pop_back(string_t* string); // Decrementing `string` size by one, returns 1 on success, and 0 on failure

int core_string_append_char(string_t* string, const char character); // Appending `character` at the end of `string` content, returns 1 on success, and 0 on failure
int core_string_append_cstr(string_t* string, const char* source); // Appending `source` C-style string at the end of `string` content, retruns 1 on success, and 0 on failure
int core_string_append_string(string_t* string, const string_t* source); // Appending `source` content at the end of `string` content, returns 1 on success, and 0 on failure

void core_string_input(string_t* string); // Accepting user input into `string` content

void core_string_lower(string_t* string); // Lowering all characters of `string` content
void core_string_upper(string_t* string); // Uppering all characters of `string` content

// GETTERS
size_t core_string_get_size(const string_t* string); // Getter for string `size` (length)
size_t core_string_get_capacity(const string_t* string); // Getter for string `capacity`
const char* core_string_get_data(const string_t* string); // Getter for string raw `data` (content)

char core_string_at(const string_t* string, size_t index); // Returning a `char` at a given `index` of `string` content if found, and returns `\0` if not found
int core_string_find(const string_t* string, const char character, size_t* index); // Finding `character` from `string` content, returns 1 and assigns its index to `index` if found, and returns 0 if not found

int core_string_replace_at(string_t* string, const size_t index, const char character); // Replacing `char` of `string` content at `index` to `character`, returns 1 on success, and 0 on failure
void core_string_replace(string_t* string, const char oldc, const char newc); // Replacing all `oldc` character in `string` content to `newc` character

// STRING DESTRUCTOR
void core_string_destroy(string_t* string); // Free `string` allocated `data` memory and clear its data

#endif