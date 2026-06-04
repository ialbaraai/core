#include "tasks.h"

#include <stdio.h>
#include <string.h>

class_t Tasks_class = {0};

void vector_print_foreach(const size_t index, void* object)
{
    printf("\t%zu - %s\n", index + 1, core_string_get_data((string_t*)object));
}

void tasks_print(object_t* object, int argc, void** argv)
{
    vector_t* tasks_vect = (vector_t*)core_member_get_value(core_object_member(object, "list"));

    printf("-----------------------------------\n");

    if (tasks_vect)
    {
        core_vector_foreach(tasks_vect, vector_print_foreach);
    }

    printf("-----------------------------------\n");
}
void tasks_add(object_t* object, int argc, void** argv)
{
    vector_t* tasks_vect = (vector_t*)core_member_get_value(core_object_member(object, "list"));

    if (tasks_vect)
    {
        printf("-----------------------------------\n");

        string_t new_task = {0};
        core_string_init(&new_task, 32);

        do
        {
            printf("\t- Enter new task: ");
            core_string_input(&new_task);
        } while (strcmp(core_string_get_data(&new_task), "") == 0);
        
        core_vector_push_back(tasks_vect, &new_task);

        core_string_destroy(&new_task);

        printf("-----------------------------------\n");
    }
}
void tasks_adjust(object_t* object, int argc, void** argv)
{
    vector_t* tasks_vect = (vector_t*)core_member_get_value(core_object_member(object, "list"));

    if (tasks_vect)
    {
        printf("-----------------------------------\n");

        if (core_vector_get_size(tasks_vect) < 1)
        {
            printf("No tasks to adjust!\n");
        }
        else
        {
            string_t target = {0};
            core_string_init(&target, 32);

            printf("\t- Enter task to adjust: ");
            core_string_input(&target);

            for (size_t i = 0; i < core_vector_get_size(tasks_vect); ++i)
            {
                if (core_string_compare(&target, (string_t*)core_vector_get(tasks_vect, i)))
                {
                    string_t new_task = {0};
                    core_string_init(&new_task, 32);

                    printf("\t\t- Enter new task: ");
                    core_string_input(&new_task);

                    core_vector_set(tasks_vect, i, &new_task);

                    core_string_destroy(&new_task);
                }
            }

            core_string_destroy(&target);
        }
        printf("-----------------------------------\n");
    }
}

int tasks_constructor(object_t* instance, int argc, void** argv)
{
    vector_t tasksvect = {0};
    core_vector_init(&tasksvect, 8, sizeof(string_t), core_string_copy_callback, core_string_destroy_callback);

    member_t memb = {0};
    core_member_init_data(&memb, "list", NULL, sizeof(vector_t), &tasksvect, PUBLIC, core_vector_copy_callback, core_vector_destroy_callback);

    method_t printm = {0};
    method_t addm = {0};
    method_t adjustm = {0};

    core_method_init(&printm, "print", tasks_print, PUBLIC);
    core_method_init(&addm, "add", tasks_add, PUBLIC);
    core_method_init(&adjustm, "adjust", tasks_adjust, PUBLIC);

    method_t methods[] = {printm, addm, adjustm};

    core_object_init(instance, &Tasks_class, "tasks", 1, &memb, 3, methods);

    core_vector_destroy(&tasksvect);
    core_member_destroy(&memb);

    for (size_t i = 0; i < 3; ++i)
    {
        core_method_destroy(&methods[i]);
    }

    return SUCCESS;
}
void tasks_destructor(object_t* object)
{
    core_object_destroy(object);
}

void Tasks_Initialize()
{
    core_class_init(&Tasks_class, "tasks", NULL, 0, NULL, 0, NULL, tasks_constructor, tasks_destructor);
}