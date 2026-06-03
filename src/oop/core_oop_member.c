#include "../../include/oop/core_oop_member.h"

#include <stdlib.h>
#include <string.h>

int core_member_init(member_t* member, const char* name, const void* holder, const size_t value_size, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function)
{
    if (!member) return MEMBER_POINTER_NULL_ERROR;
    if (member->_Member_Value) return MEMBER_ALREADY_INITIALIZED_ERROR;
    if (!name) return STRING_CSTR_NULL_ERROR;

    core_string_init_data(&member->_Member_Name, strlen(name) + 1, name);
    member->_Member_Holder = holder;
    member->_Member_Value_Size = value_size;
    member->_Member_Visibility = visibility;
    member->_Member_Value = malloc(value_size);

    if (!member->_Member_Value)
    {
        member->_Member_Value_Size = 0;

        return MEMBER_CANNOT_ALLOCATE_DATA_ERROR;
    }

    member->_Member_Copy_Function = copy_function;
    member->_Member_Destroy_Function = destroy_function;

    return SUCCESS;
}
int core_member_init_data(member_t* member, const char* name, const void* holder, const size_t value_size, const void* value, const visibility_t visibility, copy_function_t copy_function, destroy_function_t destroy_function)
{
    int ret = core_member_init(member, name, holder, value_size, visibility, copy_function, destroy_function);

    if (ret != SUCCESS)
    {
        return ret;
    }

    if (member->_Member_Value)
    {
        if (member->_Member_Copy_Function)
        {
            member->_Member_Copy_Function(member->_Member_Value, value);
        }
        else
        {
            memcpy(member->_Member_Value, value, value_size);
        }
    }

    return SUCCESS;
}

int core_member_set_value(member_t* member, const void* value)
{
    if (!member) return MEMBER_POINTER_NULL_ERROR;
    if (!member->_Member_Value) return MEMBER_VALUE_NULL_ERROR;
    if (!value) return MEMBER_NEW_VALUE_POINTER_NULL_ERROR;

    if (member->_Member_Destroy_Function)
    {
        member->_Member_Destroy_Function(member->_Member_Value);
    }

    if (member->_Member_Copy_Function)
    {
        member->_Member_Copy_Function(member->_Member_Value, value);
    }
    else
    {
        memcpy(member->_Member_Value, value, member->_Member_Value_Size);
    }

    return SUCCESS;
}

string_t* core_member_get_name(const member_t* member)
{
    if (!member) return NULL;
    if (!member->_Member_Name._String_Data) return NULL;

    return &member->_Member_Name;
}
size_t core_member_get_value_size(const member_t* member)
{
    if (!member) return 0;

    return member->_Member_Value_Size;
}
void* core_member_get_value(const member_t* member)
{
    if (!member) return NULL;
    if (!member->_Member_Value) return NULL;

    return member->_Member_Value;
}
const void* core_member_get_holder(const member_t* member)
{
    if (!member) return NULL;
    if (!member->_Member_Holder) return NULL;

    return member->_Member_Holder;
}
visibility_t* core_member_get_visibility(const member_t* member)
{
    if (!member) return NULL;

    return &member->_Member_Visibility;
}

void core_member_destroy(member_t* member)
{
    if (!member) return;

    if (member->_Member_Value)
    {
        if (member->_Member_Destroy_Function)
        {
            member->_Member_Destroy_Function(member->_Member_Value);
        }

        free(member->_Member_Value);
    }

    if (member->_Member_Name._String_Data)
    {
        core_string_destroy(&member->_Member_Name);
    }

    member->_Member_Value = NULL;
    member->_Member_Holder = NULL;

    member->_Member_Value_Size = 0;
    member->_Member_Visibility = 0;

    member->_Member_Copy_Function = NULL;
    member->_Member_Destroy_Function = NULL;
}