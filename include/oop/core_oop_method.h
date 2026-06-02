/*
 * core_oop_method.h - Core Library
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


#ifndef CORE_OOP_METHOD_H

#define CORE_OOP_METHOD_H

#include "core_oop_visibility.h"
#include "core_oop_types.h"
#include "../core/core_string.h"

#ifndef SUCCESS
#define SUCCESS 1
#endif

#define METHOD_ALREADY_INITIALIZED_ERROR 52
#define METHOD_POINTER_NULL_ERROR 53
#define METHOD_FUNCTION_NULL_ERROR 54

typedef void(*method_function_t)(object_t* this, int argc, void** argv);

typedef struct METHOD_STRUCT
{
    method_function_t _Method_Function; // method `function`
    visibility_t _Method_Visibility; // method `visibility`, use `core_method_get_visibility(const method_t* method)` for safe geter

    string_t _Method_Name; // method `name`, use `core_method_get_name(const member_t* member)` for safe getter
} method_t;

// METHOD INITIALIZER
int core_method_init(method_t* method, const char* name, method_function_t function, const visibility_t visibility); // Initialize `method` with a name of `name`, method function of `function`, and visibility of `visibility`, returns `SUCCESS` on success, and `METHOD_*_ERROR` on failure

// GETTERS
string_t* core_method_get_name(const method_t* method); // Getter for method `name`
visibility_t* core_method_get_visibility(const method_t* method); // Getter for method `visibility`

// METHOD DESTRUCTOR
void core_method_destroy(method_t* method); // Free `member` allocated `name` data and clear its content

#endif