/*
 * core_map.h - Core Library
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


#ifndef CORE_MAP_H

#define CORE_MAP_H

#include <stddef.h>

#define MAP_MAX_CAPACITY (1024ULL * 1024ULL * 1024ULL)

typedef struct MAP_STRUCT
{
    void* _Map_Key_Data; // map `key data`, use `core_map_get_key_data(const map_t* map)` for safe getter
    void* _Map_Value_Data; // map `value data`, use `core_map_get_value_data(const map_t* map)` for safe getter
    size_t _Map_Key_Element_Size; // map `key element size`, use `core_map_get_key_size(const map_t* map)` for safe getter
    size_t _Map_Value_Element_Size; // map `value element size`, use `core_map_get_value_size(const map_t* map)` for safe getter
    size_t _Map_Capacity; // map `capacity`, use `core_map_get_capacity(const map_t* map)` for safe getter
    size_t _Map_Size; // map `size` (length), use `core_map_get_size(const map_t* map)` for safe getter
    void (*_Map_Key_Copy_Function)(void* destination, const void* source);
    void (*_Map_Key_Destroy_Function)(void* object);
    void (*_Map_Value_Copy_Function)(void* destination, const void* source);
    void (*_Map_Value_Destroy_Function)(void* object);
    int (*_Map_Compare_Function)(const void* first, const void* second);
} map_t;

// MAP INITIALIZATION
map_t core_map_init(const size_t capacity, const size_t key_size, const size_t value_size, void (*key_copy_function)(void* destination, const void* source), void (*key_destroy_function)(void* object), void (*value_copy_function)(void* destination, const void* source), void (*value_destroy_function)(void* object), int (*compare_function)(const void* first, const void* second)); // Initialize a map struct with a capacity of `capacity`, key element size of `key_size`, value element size of `value_size`, key copy function callback `void key_copy_function(void* destination, const void* source)` to decide how to insert keys into map (`NULL` for default), key destroy function callback `void key_destroy_function(void* object)` to decide how to destroy key objects from map (`NULL` for default), value copy function callback `void value_copy_function(void* destination, const void* source)` to decide how to insert values into map (`NULL` for default), value destroy function callback `void value_destroy_function(void* object)` to decide how to destroy value objects from map (`NULL` for default), and compare function callback `int compare_function(const void* first, const void* second)` to decide how elements are compared (REQUIRED, return `0` if same)
map_t core_map_init_data(const size_t capacity, const size_t key_size, const size_t value_size, const size_t count, void (*key_copy_function)(void* destination, const void* source), void (*key_destroy_function)(void* object), void (*value_copy_function)(void* destination, const void* source), void (*value_destroy_function)(void* object), int (*compare_function)(const void* first, const void* second), const void* key_data, const void* value_data); // Initialize a map struct with a capacity of `capacity`, key element size of `key_size`, value element size of `value_size`, key copy function callback `void key_copy_function(void* destination, const void* source)` to decide how to insert keys into map (`NULL` for default), key destroy function callback `void key_destroy_function(void* object)` to decide how to destroy key objects from map (`NULL` for default), value copy function callback `void value_copy_function(void* destination, const void* source)` to decide how to insert values into map (`NULL` for default), value destroy function callback `void value_destroy_function(void* object)` to decide how to destroy value objects from map (`NULL` for default), compare function callback `int compare_function(const void* first, const void* second)` to decide how elements are compared (REQUIRED, return `0` if same), and key data of `key_data`, and value data of `value_data`, copies `count` amount of elements from `key_data` and `value_data`

// MAP MANIPULATION
int core_map_push_back(map_t* map, const void* key, const void* value); // Pushing a pair with the key of `key` and value of `value` to `map` through `key_copy_function` and `value_copy_function` if assigned, returns 1 on success, and 0 on failure
int core_map_pop_back(map_t* map); // Decrementing `map` size by one, returns 1 on success, and 0 on failure
int core_map_set(map_t* map, const void* key, const void* value); // Setting map value at `key` to `value`, returns 1 on success, and 0 on failure
int core_map_remove(map_t* map, const void* key); // Removing a pair at `key`, returns 1 on succes, and 0 on failure

void core_map_foreach(map_t* map, void (*function)(void* key, void* value)); // Calling `function(void* key, void* value)` for each pair in map `data`

// GETTERS
size_t core_map_get_key_size(const map_t* map); // Getter for map `key element size`
size_t core_map_get_value_size(const map_t* map); // Getter for map `value element size`
size_t core_map_get_size(const map_t* map); // Getter for map `size` (length)
size_t core_map_get_capacity(const map_t* map); // Getter for map `capacity`
const void* core_map_get_key_data(const map_t* map); // Getter for map raw `key data` (key content)
const void* core_map_get_value_data(const map_t* map); // Getter for map raw `value data` (value content)

void* core_map_first_key(const map_t* map); // Getter for the first key of `map`
void* core_map_last_key(const map_t* map); // Getter for the last key of `map`

void* core_map_first_value(const map_t* map); // Getter for the first value of `map`
void* core_map_last_value(const map_t* map); // Getter for the last value of `map`

int core_map_get_value_at(const map_t* map, const void* key, void* value); // Assigns parameter `value` to the value at a given `key` of `map` if found, returns 1 if found, and 0 on failure
void* core_map_get_value(const map_t* map, const void* key); // Returning the value `void*` at a given `key` of `map`, returns `NULL` if not found

// MAP DESTRUCTOR
void core_map_destroy(map_t* map); // Free `map` allocated `key data` and `value data` memory through `key_destroy_function` and `value_destroy_function` if assigned and clear its data

#endif