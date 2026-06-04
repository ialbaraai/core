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

void core_method_copy_callback(void* destination, const void* source)
{
    if (source && destination)
    {
        const method_t* src = (method_t*)source;

        method_t copy = {0};
        core_method_init(&copy, core_string_get_data(core_method_get_name(src)), src->_Method_Function, *core_method_get_visibility(src));
        
        memcpy(destination, &copy, sizeof(method_t));
    }
}
void core_method_destroy_callback(void* object)
{
    if (object)
    {
        core_method_destroy((method_t*)object);
    }
}