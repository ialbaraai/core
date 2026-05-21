#ifndef CORE_OOP_OBJECT_H

#define CORE_OOP_OBJECT_H

#include <stddef.h>
#include "core_oop_class.h"
#include "core_oop_member.h"
#include "core_oop_method.h"

typedef struct OBJECT_STRUCT
{
    class_t* _Object_Class; // object `class type`, use `core_object_get_class(const object_t* object)` for safe getter

    member_t* _Object_Members;
    size_t _Object_Members_Size;

    method_t* _Object_Methods;
    size_t _Object_Methods_Size;

    string_t _Object_Name; // object `name`, use `core_object_get_name(const object_t* object)` for safe getter
} object_t;

// OBJECT MANIPULATION
int core_object_call(object_t* object, const char* method_name, int argc, void** argv);

// OBJECT GETTER
string_t* core_object_get_name(const object_t* object);
member_t* core_object_member(const object_t* object, const char* member_name);
class_t* core_object_get_class(const object_t* object);

#endif