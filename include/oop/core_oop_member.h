/*
 * core_oop_member.h - Core Library
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


#ifndef CORE_OOP_MEMBER_H

#define CORE_OOP_MEMBER_H

#include "core_oop_visibility.h"
#include "core_oop_types.h"
#include "../core/core_string.h"
#include "../core_backend.h"

#include <stddef.h>

#ifndef SUCCESS
#define SUCCESS 1
#endif

#define MEMBER_ALREADY_INITIALIZED_ERROR 42
#define MEMBER_CANNOT_ALLOCATE_DATA_ERROR 43
#define MEMBER_POINTER_NULL_ERROR 44
#define MEMBER_VALUE_NULL_ERROR 45
#define MEMBER_NEW_VALUE_POINTER_NULL_ERROR 46

typedef struct MEMBER_STRUCT
{
    void* _Member_Value; // member `value`, use `core_member_get_value(const member_t* member)` for safe getter
    size_t _Member_Value_Size; // member `value size`, use `core_member_get_value_size(const member_t* member)` for safe getter
    void* _Member_Holder; // member `holder` (pointer to holder `object_t` or `class_t` instance), use `core_member_get_holder(const member_t* member)` for safe getter
    visibility_t _Member_Visibility; // member `visibility`, use `core_member_get_visibility(const member_t* member)` for safe getter

    string_t _Member_Name; // member `name`, use `core_member_get_name(const member_t* member)` for safe getter

    void (*_Member_Copy_Function)(void* destination, const void* source);
    void (*_Member_Destroy_Function)(void* object);
} member_t;

// MEMBER INITIALIZER
int core_member_init(member_t* member, const char* name, const void* holder, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function); // Initialize `member` with a name of `name`, holder class/object pointer of `holder`, value element size of `value_size`, visibility of `visibility`, cop function callback `void copy_function(void* destination, const void* source)` to decide how to copy value into member (`NULL` for default), and destroy function callback `void destroy_function(void* object)` to decide how to destroy value from member (`NULL` for default), returns `SUCCESS` on success, and `MEMBER_*_ERROR` on failure
int core_member_init_data(member_t* member, const char* name, const void* holder, const size_t value_size, const void* value, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function); // Initialize `member` with a name of `name`, holder class/object pointer of `holder`, value element size of `value_size`, value of `value`, visibility of `visibility`, cop function callback `void copy_function(void* destination, const void* source)` to decide how to copy value into member (`NULL` for default), and destroy function callback `void destroy_function(void* object)` to decide how to destroy value from member (`NULL` for default), returns `SUCCESS` on success, and `MEMBER_*_ERROR` on failure

// MEMBER MANIPULATION
int core_member_set_value(member_t* member, const void* value); // Setting `member` value to `value`, returns `SUCCESS` on success, and `MEMBER_*_ERROR` on failure

// MEMBER GETTER
string_t* core_member_get_name(const member_t* member); // Getter for member `name`
size_t core_member_get_value_size(const member_t* member); // Getter for member `value size`
void* core_member_get_value(const member_t* member); // Getter for member `value`
const void* core_member_get_holder(const member_t* member); // Getter for member `holder`
visibility_t* core_member_get_visibility(const member_t* member); // Getter for member `visibility`

// MEMBER DESTRUCTOR
void core_member_destroy(member_t* member); // Free `member` allocated `value` memory through `destroy_function` if assigned and clear its data

// MEMBER CALLBACKS
void core_member_copy_callback(void* destination, const void* source); // Callback to deep-copy member from `source` to `destination`, used for `vector`/`map`/`oop` copy function
void core_member_destroy_callback(void* object);// Callback to `core_member_destroy(member_t* member)`, used for `vector`/`map`/`oop` copy function

#endif