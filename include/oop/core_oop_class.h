#ifndef CORE_OOP_CLASS_H

#define CORE_OOP_CLASS_H

#include <stddef.h>
#include "core_oop_member.h"
#include "core_oop_method.h"
#include "core_oop_object.h"

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

#endif