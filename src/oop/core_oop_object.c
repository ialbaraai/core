#include "../../include/oop/core_oop_object.h"
#include "../../include/oop/core_oop_member.h"
#include "../../include/oop/core_oop_method.h"
#include "../../include/oop/core_oop_class.h"

#include <stdlib.h>
#include <string.h>

object_t core_object_init(const class_t* base_class, const char* name, size_t members_size, member_t* members, size_t methods_size, method_t* methods)
{
    object_t object = {._Object_Class = base_class, ._Object_Name = core_string_init_data(strlen(name) + 1, name), ._Object_Members_Size = members_size, ._Object_Methods_Size = methods_size};

    object._Object_Members = malloc(members_size * sizeof(member_t));
    object._Object_Methods = malloc(methods_size * sizeof(method_t));

    if (!object._Object_Members)
    {
        object._Object_Members_Size = 0;
        object._Object_Members = NULL;
    }
    if (!object._Object_Methods)
    {
        object._Object_Methods_Size = 0;
        object._Object_Methods = NULL;
    }

    for (size_t i = 0; i < object._Object_Members_Size; ++i)
    {
        object._Object_Members[i] = core_member_init_data(core_string_get_data(core_member_get_name(&members[i])), core_member_get_holder(&members[i]), core_member_get_value(&members[i]), core_member_get_value_size(&members[i]), *core_member_get_visibility(&members[i]), members[i]._Member_Copy_Function, members[i]._Member_Destroy_Function);
    }
    for (size_t i = 0; i < object._Object_Methods_Size; ++i)
    {
        object._Object_Methods[i] = core_method_init(core_string_get_data(core_method_get_name(&methods[i])), methods[i]._Method_Function, *core_method_get_visibility(&methods[i]));
    }

    return object;
}

int core_object_call(object_t* object, const char* method_name, int argc, void** argv)
{
    if (!object) return 0;
    if (!object->_Object_Methods) return 0;
    if (!method_name) return 0;

    for (size_t i = 0; i < object->_Object_Methods_Size; ++i)
    {
        if (strcmp(method_name, core_string_get_data(core_method_get_name(&object->_Object_Methods[i]))) == 0)
        {
            if (object->_Object_Methods[i]._Method_Function)
                object->_Object_Methods[i]._Method_Function(object, argc, argv);
            
            return 1;
        }
    }

    return 0;
}

const member_t* core_object_get_members(const object_t* object)
{
    if (!object) return NULL;
    if (!object->_Object_Members) return NULL;

    return object->_Object_Members;
}
const method_t* core_object_get_methods(const object_t* object)
{
    if (!object) return NULL;
    if (!object->_Object_Methods) return NULL;

    return object->_Object_Methods;
}

string_t* core_object_get_name(const object_t* object)
{
    if (!object) return NULL;
    if (!object->_Object_Name._String_Data) return NULL;

    return &object->_Object_Name;
}
member_t* core_object_member(const object_t* object, const char* member_name)
{
    if (!object) return NULL;
    if (!object->_Object_Members) return NULL;
    if (!member_name) return NULL;

    for (size_t i = 0; i < object->_Object_Members_Size; ++i)
    {
        if (strcmp(member_name, core_string_get_data(core_member_get_name(&object->_Object_Members[i]))) == 0)
        {
            return &object->_Object_Members[i];
        }
    }

    return NULL;
}
method_t* core_object_method(const object_t* object, const char* method_name)
{
    if (!object) return NULL;
    if (!object->_Object_Methods) return NULL;
    if (!method_name) return NULL;

    for (size_t i = 0; i < object->_Object_Methods_Size; ++i)
    {
        if (strcmp(method_name, core_string_get_data(core_method_get_name(&object->_Object_Methods[i]))) == 0)
        {
            return &object->_Object_Methods[i];
        }
    }

    return NULL;
}
class_t* core_object_get_class(const object_t* object)
{
    if (!object) return NULL;
    if (!object->_Object_Class) return NULL;

    return object->_Object_Class;
}

size_t core_object_get_members_size(const object_t* object)
{
    if (!object) return 0;
    if (!object->_Object_Members) return 0;

    return object->_Object_Members_Size;
}
size_t core_object_get_methods_size(const object_t* object)
{
    if (!object) return 0;
    if (!object->_Object_Methods) return 0;

    return object->_Object_Methods_Size;
}

void core_object_destroy(object_t* object)
{
    if (!object) return;

    if (object->_Object_Members)
    {
        for (size_t i = 0; i < object->_Object_Members_Size; ++i)
        {
            core_member_destroy(&object->_Object_Members[i]);
        }
        
        free(object->_Object_Members);
    }

    if (object->_Object_Methods)
    {
        for (size_t i = 0; i < object->_Object_Methods_Size; ++i)
        {
            core_method_destroy(&object->_Object_Methods[i]);
        }

        free(object->_Object_Methods);
    }

    core_string_destroy(&object->_Object_Name);

    object->_Object_Class = NULL;

    object->_Object_Members = NULL;
    object->_Object_Methods = NULL;

    object->_Object_Members_Size = 0;
    object->_Object_Methods_Size = 0;
}