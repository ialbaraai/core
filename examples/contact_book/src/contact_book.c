#include "contact_book.h"

#include <stdio.h>
#include <string.h>

class_t ContactBook_Class = {0};

void map_print_foreach(void* name, void* number)
{
    printf("\t\"%s\" - %s\n", core_string_get_data((string_t*)name), core_string_get_data((string_t*)number));
}

void print_contacts(object_t* object, int argc, void** argv)
{
    map_t* contacts_map = (map_t*)core_member_get_value(core_object_member(object, "list"));


    if (contacts_map)
    {
        printf("-----------------------------------\n");
        core_map_foreach(contacts_map, map_print_foreach);

        printf("-----------------------------------\n");
    }

}
void add_contact(object_t* object, int argc, void** argv)
{
    map_t* contacts_map = (map_t*)core_member_get_value(core_object_member(object, "list"));

    if (contacts_map)
    {
        printf("-----------------------------------\n");

        string_t input_str = {0};
        core_string_init(&input_str, 8);

        string_t new_name = {0};
        string_t new_number = {0};

        core_string_init(&new_name, 32);
        core_string_init(&new_number, 32);

        bool is_added = false;

        do
        {
            do
            {
                printf("\t- Enter new contact name: ");
                core_string_input(&new_name);
            } while (strcmp(core_string_get_data(&new_name), "") == 0);

            core_string_lower(&new_name);

            if (core_map_get_value_at(contacts_map, &new_name, &new_number))
            {
                printf("\t- Found a contact with the same name, what would you like to do?\n\t\t1. Enter new name\n\t\t2. Override name's number\n\t\t3. Cancel\n\tChoose an option: ");
                core_string_input_strict(&input_str, 1);

                if (strcmp(core_string_get_data(&input_str), "1") == 0)
                {
                    is_added = false;
                }
                else if (strcmp(core_string_get_data(&input_str), "2") == 0)
                {
                    do
                    {
                        printf("\t- Enter new number for \"%s\": ", core_string_get_data(&new_name));
                        core_string_input(&new_number);
                    } while (strcmp(core_string_get_data(&new_number), "") == 0);

                    core_map_set(contacts_map, &new_name, &new_number);

                    is_added = true;
                }
                else if (strcmp(core_string_get_data(&input_str), "3") == 0)
                {
                    break;
                }
            }
            else
            {
                do
                {
                    printf("\t- Enter new number for \"%s\": ", core_string_get_data(&new_name));
                    core_string_input(&new_number);
                } while (strcmp(core_string_get_data(&new_number), "") == 0);

                core_map_push_back(contacts_map, &new_name, &new_number);

                is_added = true;
            }
        } while (!is_added);

        core_string_destroy(&new_number);
        core_string_destroy(&new_name);
        core_string_destroy(&input_str);

        printf("-----------------------------------\n");
    }
}
void adjust_contact(object_t* object, int argc, void** argv)
{
    map_t* contacts_map = (map_t*)core_member_get_value(core_object_member(object, "list"));

    if (contacts_map)
    {
        printf("-----------------------------------\n");

        string_t name = {0};
        core_string_init(&name, 32);
        string_t number = {0};
        core_string_init(&number, 32);

        do
        {
            printf("\t- Enter contact name to adjust: ");
            core_string_input(&name);
        } while (strcmp(core_string_get_data(&name), "") == 0);

        core_string_lower(&name);
        
        if (core_map_get_value_at(contacts_map, &name, &number))
        {
            printf("\t\t- Current number associated with \"%s\": %s\n", core_string_get_data(&name), core_string_get_data(&number));

            do
            {
                printf("\t\t- Enter new number for \"%s\": ", core_string_get_data(&name));
                core_string_input(&number);
            } while (strcmp(core_string_get_data(&number), "") == 0);
            
            core_map_set(contacts_map, &name, &number);
        }
        else
        {
            printf("\t\t- Couldn't find name in contact book!\n");
        }

        core_string_destroy(&name);
        core_string_destroy(&number);

        printf("-----------------------------------\n");
    }
}

int contact_book_constructor(object_t* object, int argc, void** argv)
{
    map_t contacts_map = {0};
    core_map_init(&contacts_map, 8, sizeof(string_t), sizeof(string_t), core_string_copy_callback, core_string_destroy_callback, core_string_copy_callback, core_string_destroy_callback, core_string_compare_callback);

    member_t member = {0};
    core_member_init_data(&member, "list", NULL, sizeof(map_t), &contacts_map, PUBLIC, core_map_copy_callback, core_map_destroy_callback);

    method_t printm = {0};
    method_t addm = {0};
    method_t adjustm = {0};

    core_method_init(&printm, "print", print_contacts, PUBLIC);
    core_method_init(&addm, "add", add_contact, PUBLIC);
    core_method_init(&adjustm, "adjust", adjust_contact, PUBLIC);

    method_t methods[] = {printm, addm, adjustm};

    core_object_init(object, &ContactBook_Class, "contacts", 1, &member, 3, methods);

    for (size_t i = 0; i < 3; ++i)
    {
        core_method_destroy(&methods[i]);
    }

    core_member_destroy(&member);
    core_map_destroy(&contacts_map);

    return SUCCESS;
}
void contact_book_destructor(object_t* object) 
{
    core_object_destroy(object);
}

void ContactBook_Initialize()
{
    core_class_init(&ContactBook_Class, "contact book", NULL, 0, NULL, 0, NULL, contact_book_constructor, contact_book_destructor);
}