#include "../../include/oop/core_oop_method.h"

#include <string.h>

method_t core_method_init(const char* name, method_function_t function, const visibility_t visibility)
{
    method_t method = {._Method_Name = core_string_init_data(strlen(name) + 1, name), ._Method_Function = function, ._Method_Visibility = visibility};

    return method;
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