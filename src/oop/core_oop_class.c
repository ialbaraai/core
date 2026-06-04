#include "../../include/oop/core_oop_object.h"
#include "../../include/oop/core_oop_member.h"
#include "../../include/oop/core_oop_method.h"
#include "../../include/oop/core_oop_class.h"

#include <stdlib.h>
#include <string.h>

int core_class_init(class_t* class, const char* name, const class_t* base_class, size_t static_members_size, member_t* static_members, size_t static_methods_size, method_t* static_methods, int(*constructor)(object_t* object, int argc, void** argv), void(*destructor)(object_t* object))
{
    if (!class) return CLASS_POINTER_NULL_ERROR;
    if (!name) return CLASS_CSTR_NULL_ERROR;

    core_string_init_data(&class->_Class_Name, strlen(name) + 1, name);
    class->_Class_Base_Class = base_class;
    class->_Class_Static_Members_Size = static_members_size;
    class->_Class_Static_Methods_Size = static_methods_size;
    class->constructor = constructor;
    class->destructor = destructor;

    class->_Class_Static_Members = static_members_size ? calloc(static_members_size, sizeof(member_t)) : NULL;
    class->_Class_Static_Methods = static_methods_size ? calloc(static_methods_size, sizeof(method_t)) : NULL;

    if (static_members_size && !class->_Class_Static_Members)
    {
        class->_Class_Static_Members_Size = 0;
        class->_Class_Static_Members = NULL;

        return CLASS_CANNOT_ALLOCATE_ERROR;
    }
    if (static_methods_size && !class->_Class_Static_Methods)
    {
        class->_Class_Static_Methods_Size = 0;
        class->_Class_Static_Methods = NULL;

        return CLASS_CANNOT_ALLOCATE_ERROR;
    }

    for (size_t i = 0; i < class->_Class_Static_Members_Size; ++i)
    {
        core_member_init_data(&class->_Class_Static_Members[i], core_string_get_data(core_member_get_name(&static_members[i])), core_member_get_holder(&static_members[i]), core_member_get_value_size(&static_members[i]), core_member_get_value(&static_members[i]), *core_member_get_visibility(&static_members[i]), static_members[i]._Member_Copy_Function, static_members[i]._Member_Destroy_Function);
    }
    for (size_t i = 0; i < class->_Class_Static_Methods_Size; ++i)
    {
        core_method_init(&class->_Class_Static_Methods[i], core_string_get_data(core_method_get_name(&static_methods[i])), static_methods[i]._Method_Function, *core_method_get_visibility(&static_methods[i]));
    }

    return SUCCESS;
}

int core_class_call(class_t* class, const char* method_name, int argc, void** argv)
{
    if (!class) return CLASS_POINTER_NULL_ERROR;
    if (!method_name) return CLASS_CSTR_NULL_ERROR;

    if (class->_Class_Static_Methods)
    {
        for (size_t i = 0; i < class->_Class_Static_Methods_Size; ++i)
        {
            if (strcmp(method_name, core_string_get_data(core_method_get_name(&class->_Class_Static_Methods[i]))) == 0)
            {
                if (class->_Class_Static_Methods[i]._Method_Function)
                    class->_Class_Static_Methods[i]._Method_Function(NULL, argc, argv);
                
                return SUCCESS;
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

                return SUCCESS;
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

void core_class_copy_callback(void* destination, const void* source)
{
    if (destination && source)
    {
        const class_t* src = (class_t*)source;

        class_t copy = {0};
        core_class_init(&copy, core_string_get_data(core_class_get_name(src)), core_class_get_base(src), core_class_get_static_members_size(src), core_class_get_static_members(src), core_class_get_static_methods_size(src), core_class_get_static_methods(src), src->constructor, src->destructor);

        memcpy(destination, &copy, sizeof(class_t));
    }
}
void core_class_destroy_callback(void* object)
{
    if (object)
    {
        core_class_destroy((class_t*)object);
    }
}