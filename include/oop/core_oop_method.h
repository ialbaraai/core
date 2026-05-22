#ifndef CORE_OOP_METHOD_H

#define CORE_OOP_METHOD_H

#include "core_oop_visibility.h"
#include "core_oop_types.h"
#include "../core/core_string.h"

typedef void(*method_function_t)(object_t* this, int argc, void** argv);

typedef struct METHOD_STRUCT
{
    method_function_t _Method_Function;
    visibility_t _Method_Visibility;

    string_t _Method_Name;
} method_t;

// METHOD INITIALIZER
method_t core_method_init(const char* name, method_function_t function, const visibility_t visibility);

// GETTERS
string_t* core_method_get_name(const method_t* method);
visibility_t* core_method_get_visibility(const method_t* method);

// METHOD DESTRUCTOR
void core_method_destroy(method_t* method);

#endif