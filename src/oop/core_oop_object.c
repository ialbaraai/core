#include "../../include/oop/core_oop_object.h"

#include <string.h>

int core_object_call(object_t* object, const char* method_name, int argc, void** argv)
{
    for (size_t i = 0; i < object->_Object_Methods_Size; ++i)
    {
        if (strcmp(method_name, core_string_get_data(&object->_Object_Methods[i]._Method_Name)) == 0)
        {
            if (object->_Object_Methods[i]._Method_Function)
                object->_Object_Methods[i]._Method_Function(object, argc, argv);
            
            return 1;
        }
    }

    return 0;
}

string_t* core_object_get_name(const object_t* object)
{

}
member_t* core_object_member(const object_t* object)
{

}
class_t* core_object_get_class(const object_t* object)
{
    
}