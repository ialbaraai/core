/*
 * core_oop_class.h - Core Library
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


#ifndef CORE_OOP_CLASS_H

#define CORE_OOP_CLASS_H

#include "../core/core_string.h"
#include "core_oop_types.h"

#include <stddef.h>

#ifndef SUCCESS
#define SUCCESS 1
#endif

#define CLASS_ALREADY_INITIALIZED 72
#define CLASS_CANNOT_ALLOCATE_ERROR 73
#define CLASS_POINTER_NULL_ERROR 74
#define CLASS_STATIC_MEMBER_DATA_POINTER_NULL_ERROR 75
#define CLASS_STATIC_METHOD_DATA_POINTER_NULL_ERROR 76
#define CLASS_CSTR_NULL_ERROR 77

typedef struct CLASS_STRUCT
{
    class_t* _Class_Base_Class; // class `base class` for inheritance, use `core_class_get_base(const class_t* class)` for safe getter

    member_t* _Class_Static_Members; // class `static members`, use `core_class_get_static_members(const class_t* class)` for safe getter
    size_t _Class_Static_Members_Size; // class `static members size` (length), use `core_class_get_static_members_size(const class_t* class)` for safe getter

    method_t* _Class_Static_Methods; // class `static methods`, use `core_class_get_static_methods(const class_t* class)` for safe getter
    size_t _Class_Static_Methods_Size; // class `static methods size` (length), use `core_class_get_static_methods_size(const class_t* class)` for safe getter

    string_t _Class_Name; // class `name`, use `core_class_get_name(const class_t* class)` for safe getter

    int (*constructor)(object_t* object, int argc, void** argv); // class `constructor`, use `class.constructor(&instance, {argc}, {argv})` to construct `instance`
    void (*destructor)(object_t* object); // class `destructor`, use `class-destructor(&instance)` to destroy `instance`
} class_t;

// CLASS INITIALIZER
int core_class_init(class_t* class, const char* name, const class_t* base_class, size_t static_members_size, member_t* static_members, size_t static_methods_size, method_t* static_methods, int(*constructor)(object_t* object, int argc, void** argv), void(*destructor)(object_t* object)); // Initialize `class` with name of `name`, base class of `base_class`, static members size of `static_members_size`, static members data of `static_members`, static methods size of `static_methods_size`, static methods data of `static_methods`, constructor function of `int constructor(object_t* object, int argc, void** argv)`, and destructor function of `void destructor(object_t* object)`, returns `SUCCESS` on success, and `CLASS_*_ERROR` on failure

// CLASS MANIPULATION
int core_class_call(class_t* class, const char* method_name, int argc, void** argv); // Calling `class` method of name `method_name` with parameters of `argc,argv` through one layer of inheritance, returns `SUCCESS` on success, and `OBJECT_*_ERROR` on failure, and `0` if not found

// CLASS GETTER
const member_t* core_class_get_static_members(const class_t* class); // Getter for class `static members`
const method_t* core_class_get_static_methods(const class_t* class); // Getter for class `static methods`

string_t* core_class_get_name(const class_t* class); // Getter for class `name`
member_t* core_class_static_member(const class_t* class, const char* member_name); // Returning `member_t*` with the name of `member_name`, returns `NULL` if not found 
method_t* core_class_static_method(const class_t* class, const char* method_name); // Returning `method_t*` with the name of `method_name`, returns `NULL` if not found
const class_t* core_class_get_base(const class_t* class); // Getter for class `base class`

size_t core_class_get_static_members_size(const class_t* class); // Getter for class `static members size`
size_t core_class_get_static_methods_size(const class_t* class); // Getter for class `static methods size`

// CLASS DESTRUCTOR
void core_class_destroy(class_t* class); // Free `class` allocated `static members` and `static methods` and `name` data and clear its content

#endif