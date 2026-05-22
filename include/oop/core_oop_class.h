#ifndef CORE_OOP_CLASS_H

#define CORE_OOP_CLASS_H

#include "../core/core_string.h"
#include "core_oop_types.h"

#include <stddef.h>

typedef struct CLASS_STRUCT
{
    class_t* _Class_Base_Class;

    member_t* _Class_Static_Members;
    size_t _Class_Static_Members_Size;

    method_t* _Class_Static_Methods;
    size_t _Class_Static_Methods_Size;

    string_t _Class_Name;

    object_t (*constructor)(int argc, void** argv);
    void (*destructor)(object_t* object);
} class_t;

// CLASS INITIALIZER
class_t core_class_init(const char* name, const class_t* base_class, size_t static_members_size, member_t* static_members, size_t static_methods_size, method_t* static_methods, object_t(*constructor)(int argc, void** argv), void(*destructor)(object_t* object));

// CLASS MANIPULATION
int core_class_call(class_t* class, const char* method_name, int argc, void** argv);

// CLASS GETTER
const member_t* core_class_get_static_members(const class_t* class);
const method_t* core_class_get_static_methods(const class_t* class);

string_t* core_class_get_name(const class_t* class);
member_t* core_class_static_member(const class_t* class, const char* member_name);
method_t* core_class_static_method(const class_t* class, const char* method_name);
const class_t* core_class_get_base(const class_t* class);

size_t core_class_get_static_members_size(const class_t* class);
size_t core_class_get_static_methods_size(const class_t* class);

// CLASS DESTRUCTOR
void core_class_destroy(class_t* class);

#endif