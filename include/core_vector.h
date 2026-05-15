/*
 * core_vector.h - Core Library
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


#ifndef CORE_VECTOR_H

#define CORE_VECTOR_H

#include <stddef.h>

#define VECTOR_MAX_CAPACITY (1024ULL * 1024ULL * 1024ULL)

typedef struct VECTOR_STRUCT
{
    void* _Vector_Data; // vector `data`, use `core_vector_get_data(const vector_t* vector)` for safe getter
    size_t _Vector_Element_Size; // vector `element size`, use `core_vector_get_element_size(const vector_t* vector)` for safe getter
    size_t _Vector_Size; // vector `size` (length), use `core_vector_get_size(const vector_t* vector)` for safe getter
    size_t _Vector_Capacity; // vector `capacity`, use `core_vector_get_capacity(const vector_t* vector)` for safe getter
    void (*_Vector_Copy_Function)(void* destination, const void* source);
    void (*_Vector_Destroy_Function)(void* object);
} vector_t;

// VECTOR INITIALIZATION
vector_t core_vector_init(const size_t capacity, const size_t element_size, void (*copy_function)(void* destination, const void* source), void(*destroy_function)(void* object)); // Initialize a vector struct with a capacity of `capacity`, element size of `element_size`, copy function callback `void copy_function(void* destination, const void* source)` to decide how to insert values into vector (`NULL` for default), and destroy function callback `void destroy_function(void* object)` to decide how to destroy objects from vector (`NULL` for default)
vector_t core_vector_init_data(const size_t capacity, const size_t element_size, const size_t count, void (*copy_function)(void* destination, const void* source), void (*destroy_function)(void* object), const void* data); // Initialize a vector struct with a capacity of `capacity`, element size of `element_size`, copy function callback `void copy_function(void* destination, const void* source)` to decide how to insert values into vector (`NULL` for default), destroy function callback `void destroy_function(void* object)` to decide how to destroy objects from vector (`NULL` for default), and data of `data`, copies `count` amount of elements from `data`

// VECTOR MANIPULATION
int core_vector_push_back(vector_t* vector, const void* value); // Pushing back an element with the value of `value` to `vector` through `copy_function` if assigned, returns 1 on success, and 0 on failure
int core_vector_pop_back(vector_t* vector); // Decrementing `vector` size by one, returns 1 on success, and 0 on failure
int core_vector_set(vector_t* vector, const size_t index, const void* value); // Setting vector content at `index` to `value`, returns 1 on success, and 0 on failure
int core_vector_remove(vector_t* vector, const size_t index); // Removing an element at `index`, returns 1 on success, and 0 on failure

void core_vector_foreach(vector_t* vector, void (*function)(const size_t index, void* data)); // Calling `function(const size_t index, void* data)` for each element in vector `data`

// GETTERS
size_t core_vector_get_size(const vector_t* vector); // Getter for vector `size` (length)
size_t core_vector_get_element_size(const vector_t* vector); // Getter for vector `element size`
size_t core_vector_get_capacity(const vector_t* vector); // Getter for vector `capacity`
const void* core_vector_get_data(const vector_t* vector); // Getter for vector raw `data` (content)

void* core_vector_first(const vector_t* vector); // Getter for the first element of `vector`
void* core_vector_last(const vector_t* vector); // Getter for the last element of `vector`

int core_vector_at(const vector_t* vector, const size_t index, void* value); // Assigns parameter `void*` to the value at a given `index` of `vector` if found, returns 1 if found, and 0 on failure
void* core_vector_get(const vector_t* vector, const size_t index); // Returning the value `void*` at a given `index` of `vector`, returns `NULL` if not found

// VECTOR DESTRUCTOR
void core_vector_destroy(vector_t* vector); // Free `vector` allocated `data` memory through `destroy_function` if assigned and clear its data

#endif