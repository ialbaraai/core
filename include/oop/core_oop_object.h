#ifndef CORE_OOP_OBJECT_H

#define CORE_OOP_OBJECT_H

#include "../core/core_string.h"
#include "core_oop_types.h"

#include <stddef.h>

typedef struct OBJECT_STRUCT
{
    class_t* _Object_Class; // object `class type`, use `core_object_get_class(const object_t* object)` for safe getter

    member_t* _Object_Members; // object `members values`, use `core_object_get_members(const object_t* object)` for safe getter
    size_t _Object_Members_Size; // object `members size` (length), use `core_object_get_members_size(const object_t* object)` for safe getter

    method_t* _Object_Methods; // object `methods values`, use `core_object_get_methods(const object_t* object)` for safe getter
    size_t _Object_Methods_Size; // object `methods size` (length), use `core_object_get_methods_size(const object_t* object)` for safe getter

    string_t _Object_Name; // object `name`, use `core_object_get_name(const object_t* object)` for safe getter
} object_t;

// OBJECT INITIALIZER
object_t core_object_init(const class_t* base_class, const char* name, size_t members_size, member_t* members, size_t methods_size, method_t* methods);

// OBJECT MANIPULATION
int core_object_call(object_t* object, const char* method_name, int argc, void** argv);

// OBJECT GETTER
const member_t* core_object_get_members(const object_t* object);
const method_t* core_object_get_methods(const object_t* object);

string_t* core_object_get_name(const object_t* object);
member_t* core_object_member(const object_t* object, const char* member_name);
method_t* core_object_method(const object_t* object, const char* method_name);
class_t* core_object_get_class(const object_t* object);

size_t core_object_get_members_size(const object_t* object);
size_t core_object_get_methods_size(const object_t* object);

// OBJECT DESTRUCTOR
void core_object_destroy(object_t* object);

#endif