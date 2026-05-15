/*
 * core_oop.h - Core Library
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


#ifndef CORE_OOP_H

#define CORE_OOP_H

#include <stddef.h>

typedef enum VISIBILITY_ENUM
{
    PUBLIC,
    PRIVATE
} visibility_t;

typedef struct ATTRIBUTE_STRUCT
{
    void* _Value;
    size_t _Value_Size;
    void* _Holder;
    visibility_t _Visibility;
} attribute_t;

typedef struct METHOD_STRUCT
{
    void* _this;
    void(*_function)(void* this, void* args);
    visibility_t _Visibility;
} method_t;

typedef struct CLASS_STRUCT
{
    void* m_Base;
    method_t m_Method;
    attribute_t m_Attribute;
} class_t;

#endif