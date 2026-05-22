#ifndef CORE_OOP_MEMBER_H

#define CORE_OOP_MEMBER_H

#include "core_oop_visibility.h"
#include "core_oop_types.h"
#include "../core/core_string.h"
#include "../core_backend.h"

#include <stddef.h>

typedef struct MEMBER_STRUCT
{
    void* _Member_Value;
    size_t _Member_Value_Size;
    void* _Member_Holder;
    visibility_t _Member_Visibility;

    string_t _Member_Name;

    void (*_Member_Copy_Function)(void* destination, const void* source);
    void (*_Member_Destroy_Function)(void* object);
} member_t;

// MEMBER INITIALIZER
member_t core_member_init(const char* name, const void* holder, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function);
member_t core_member_init_data(const char* name, const void* holder, const void* value, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function);

// MEMBER MANIPULATION
int core_member_set_value(member_t* member, const void* value);

// MEMBER GETTER
string_t* core_member_get_name(const member_t* member);
size_t core_member_get_value_size(const member_t* member);
void* core_member_get_value(const member_t* member);
const void* core_member_get_holder(const member_t* member);
visibility_t* core_member_get_visibility(const member_t* member);

// MEMBER DESTRUCTOR
void core_member_destroy(member_t* member);

#endif