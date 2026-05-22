#include "../../include/oop/core_oop_object.h"
#include "../../include/oop/core_oop_member.h"
#include "../../include/oop/core_oop_method.h"
#include "../../include/oop/core_oop_class.h"

#include <stdlib.h>
#include <string.h>

class_t core_class_init(const char* name, const class_t* base_class, member_t* static_members, size_t static_members_size, method_t* static_methods, size_t static_methods_size, object_t(*constructor)(int argc, void** argv), void(*destructor)(object_t* object))
{
    class_t class = {._Class_Name = core_string_init_data(strlen(name) + 1, name), ._Class_Base_Class = base_class, ._Class_Static_Members_Size = static_members_size, ._Class_Static_Methods_Size = static_methods_size, .constructor = constructor, .destructor = destructor};

    class._Class_Static_Members = static_members_size ? malloc(static_members_size * sizeof(member_t)) : NULL;
    class._Class_Static_Methods = static_methods_size ? malloc(static_methods_size * sizeof(method_t)) : NULL;

    if (!class._Class_Static_Members)
    {
        class._Class_Static_Members_Size = 0;
        class._Class_Static_Members = NULL;
    }
    if (!class._Class_Static_Methods)
    {
        class._Class_Static_Methods_Size = 0;
        class._Class_Static_Methods = NULL;
    }

    for (size_t i = 0; i < class._Class_Static_Members_Size; ++i)
    {
        class._Class_Static_Members[i] = core_member_init_data(core_string_get_data(core_member_get_name(&static_members[i])), core_member_get_holder(&static_members[i]), core_member_get_value(&static_members[i]), core_member_get_value_size(&static_members[i]), *core_member_get_visibility(&static_members[i]), static_members[i]._Member_Copy_Function, static_members[i]._Member_Destroy_Function);
    }
    for (size_t i = 0; i < class._Class_Static_Methods_Size; ++i)
    {
        class._Class_Static_Methods[i] = core_method_init(core_string_get_data(core_method_get_name(&static_methods[i])), static_methods[i]._Method_Function, *core_method_get_visibility(&static_methods[i]));
    }

    return class;
}

int core_class_call(class_t* class, const char* method_name, int argc, void** argv)
{
    if (!class) return 0;
    if (!method_name) return 0;

    if (class->_Class_Static_Methods)
    {
        for (size_t i = 0; i < class->_Class_Static_Methods_Size; ++i)
        {
            if (strcmp(method_name, core_string_get_data(core_method_get_name(&class->_Class_Static_Methods[i]))) == 0)
            {
                if (class->_Class_Static_Methods[i]._Method_Function)
                    class->_Class_Static_Methods[i]._Method_Function(NULL, argc, argv);
                
                return 1;
            }
        }
    }

    // INHERITANCE STATIC METHOD CALLS (IF HAS PARENT CLASS)
    if (class->_Class_Base_Class && class->_Class_Base_Class->_Class_Static_Methods)
    {
        for (size_t i = 0; i < class->_Class_Base_Class->_Class_Static_Methods_Size; ++i)
        {
            if (strcmp(method_name, core_string_get_data(core_method_get_name(&class->_Class_Base_Class->_Class_Static_Methods[i]))) == 0)
            {
                if (class->_Class_Base_Class->_Class_Static_Methods[i]._Method_Function)
                    class->_Class_Base_Class->_Class_Static_Methods[i]._Method_Function(NULL, argc, argv);

                return 1;
            }
        }
    }

    return 0;
}

const member_t* core_class_get_static_members(const class_t* class)
{
    if (!class) return NULL;
    if (!class->_Class_Static_Members) return NULL;

    return class->_Class_Static_Members;
}
const method_t* core_class_get_static_methods(const class_t* class)
{
    if (!class) return NULL;
    if (!class->_Class_Static_Methods) return NULL;

    return class->_Class_Static_Methods;
}

string_t* core_class_get_name(const class_t* class)
{
    if (!class) return NULL;
    if (!class->_Class_Name._String_Data) return NULL;

    return &class->_Class_Name;
}
member_t* core_class_static_member(const class_t* class, const char* member_name)
{
    if (!class) return NULL;
    if (!class->_Class_Static_Members) return NULL;
    if (!member_name) return NULL;

    for (size_t i = 0; i < class->_Class_Static_Members_Size; ++i)
    {
        if (strcmp(member_name, core_string_get_data(core_member_get_name(&class->_Class_Static_Members[i]))) == 0)
        {
            return &class->_Class_Static_Members[i];
        }
    }

    return NULL;
}
method_t* core_class_static_method(const class_t* class, const char* method_name)
{
    if (!class) return NULL;
    if (!class->_Class_Static_Methods) return NULL;
    if (!method_name) return NULL;

    for (size_t i = 0; i < class->_Class_Static_Methods_Size; ++i)
    {
        if (strcmp(method_name, core_string_get_data(core_method_get_name(&class->_Class_Static_Methods[i]))) == 0)
        {
            return &class->_Class_Static_Methods[i];
        }
    }

    return NULL;
}
const class_t* core_class_get_base(const class_t* class)
{
    if (!class) return NULL;
    if (!class->_Class_Base_Class) return NULL;

    return class->_Class_Base_Class;
}

size_t core_class_get_static_members_size(const class_t* class)
{
    if (!class) return 0;
    if (!class->_Class_Static_Members) return 0;

    return class->_Class_Static_Members_Size;
}
size_t core_class_get_static_methods_size(const class_t* class)
{
    if (!class) return 0;
    if (!class->_Class_Static_Methods) return 0;

    return class->_Class_Static_Methods_Size;
}

void core_class_destroy(class_t* class)
{
    if (!class) return;

    if (class->_Class_Static_Members)
    {
        for (size_t i = 0; i < class->_Class_Static_Members_Size; ++i)
        {
            core_member_destroy(&class->_Class_Static_Members[i]);
        }

        free(class->_Class_Static_Members);
    }

    if (class->_Class_Static_Methods)
    {
        for (size_t i = 0; i < class->_Class_Static_Methods_Size; ++i)
        {
            core_method_destroy(&class->_Class_Static_Methods[i]);
        }

        free(class->_Class_Static_Methods);
    }
    
    core_string_destroy(&class->_Class_Name);

    class->_Class_Base_Class = NULL;

    class->_Class_Static_Members = NULL;
    class->_Class_Static_Methods = NULL;

    class->_Class_Static_Members_Size = 0;
    class->_Class_Static_Methods_Size = 0;
}