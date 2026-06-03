#include "../../include/oop/core_oop_method.h"

#include <string.h>

int core_method_init(method_t* method, const char* name, method_function_t function, const visibility_t visibility)
{
    if (!method) return METHOD_POINTER_NULL_ERROR;
    if (method->_Method_Function) return METHOD_ALREADY_INITIALIZED_ERROR;
    if (!name) return STRING_CSTR_NULL_ERROR;

    core_string_init_data(&method->_Method_Name, strlen(name) + 1, name);
    method->_Method_Function = function;
    method->_Method_Visibility = visibility;

    return SUCCESS;
}

string_t* core_method_get_name(const method_t* method)
{
    if (!method) return NULL;
    if (!method->_Method_Name._String_Data) return NULL;

    return &method->_Method_Name;
}
visibility_t* core_method_get_visibility(const method_t* method)
{
    if (!method) return NULL;

    return &method->_Method_Visibility;
}

void core_method_destroy(method_t* method)
{
    if (!method) return;

    if (method->_Method_Name._String_Data)
    {
        core_string_destroy(&method->_Method_Name);
    }

    method->_Method_Function = NULL;
}