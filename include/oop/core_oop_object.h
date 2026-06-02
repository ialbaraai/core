/*
 * core_oop_object.h - Core Library
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


#ifndef CORE_OOP_OBJECT_H

#define CORE_OOP_OBJECT_H

#include "../core/core_string.h"
#include "core_oop_types.h"

#include <stddef.h>

#ifndef SUCCESS
#define SUCCESS 1
#endif

#define OBJECT_ALREADY_INITIALIZED_ERROR 62
#define OBJECT_CANNOT_ALLOCATE_ERROR 63
#define OBJECT_POINTER_NULL_ERROR 64
#define OBJECT_MEMBER_DATA_POINTER_NULL_ERROR 65
#define OBJECT_METHOD_DATA_POINTER_NULL_ERROR 66
#define OBJECT_CSTR_NULL_ERROR 67

typedef struct OBJECT_STRUCT
{
    class_t* _Object_Class; // object `class type`, use `core_object_get_class(const object_t* object)` for safe getter

    member_t* _Object_Members; // object `members`, use `core_object_get_members(const object_t* object)` for safe getter
    size_t _Object_Members_Size; // object `members size` (length), use `core_object_get_members_size(const object_t* object)` for safe getter

    method_t* _Object_Methods; // object `methods`, use `core_object_get_methods(const object_t* object)` for safe getter
    size_t _Object_Methods_Size; // object `methods size` (length), use `core_object_get_methods_size(const object_t* object)` for safe getter

    string_t _Object_Name; // object `name`, use `core_object_get_name(const object_t* object)` for safe getter
} object_t;

// OBJECT INITIALIZER
int core_object_init(object_t* object, const class_t* base_class, const char* name, size_t members_size, member_t* members, size_t methods_size, method_t* methods); // Initialize `object` with base class of `base_class`, name of `name`, members size of `members_size`, members data of `members`, methods size of `methods_size`, methods data of `methods`, returns `SUCCESS` on success, and `OBJECT_*_ERROR` on failure

// OBJECT MANIPULATION
int core_object_call(object_t* object, const char* method_name, int argc, void** argv); // Calling `object` method of name `method_name` with parameters `argc,argv`, returns `SUCCESS` on success, and `OBJECT_*_ERROR` on failure, and `0` if not found

// OBJECT GETTER
const member_t* core_object_get_members(const object_t* object); // Getter for object `members`
const method_t* core_object_get_methods(const object_t* object); // Getter for object `methods`

string_t* core_object_get_name(const object_t* object); // Getter for object `name`
member_t* core_object_member(const object_t* object, const char* member_name); // Returning `member_t*` with the name `member_name`, returns `NULL` if not found
method_t* core_object_method(const object_t* object, const char* method_name); // Returning `method_t*` with the name `method_name`, returns `NULL` if not found
class_t* core_object_get_class(const object_t* object); // Getter for object `class type`

size_t core_object_get_members_size(const object_t* object); // Getter for object `members size`
size_t core_object_get_methods_size(const object_t* object); // Getter for object `methods size`

// OBJECT DESTRUCTOR
void core_object_destroy(object_t* object); // Free `object` allocated `members` and `methods` and `name` data and clear its content

#endif