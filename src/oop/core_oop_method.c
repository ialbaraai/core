#include "../../include/oop/core_oop_method.h"

#include <string.h>

method_t core_method_init(const char* name, method_function_t function, const visibility_t visibility)
{
    method_t method = {._Method_Name = core_string_init_data(strlen(name) + 8, name), ._Method_Function = function, ._Method_Visibility = visibility};

    return method;
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