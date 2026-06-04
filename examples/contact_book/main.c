#include <stdio.h>
#include <string.h>

#include <core.h>

#include "include/contact_book.h"

void handle_loop(object_t* contact_book);

int main(void)
{
    ContactBook_Initialize();

    object_t contact_book= {0};
    ContactBook_Class.constructor(&contact_book, 0, NULL);

    handle_loop(&contact_book);

    ContactBook_Class.destructor(&contact_book);

    core_class_destroy(&ContactBook_Class);

    return 0;
}

void handle_loop(object_t* contact_book)
{
    string_t input = {0};
    core_string_init(&input, 8);

    printf("---------- Contact Book ----------\n");

    do
    {
        printf("What would you like to do?\n\t1. List contacts\n\t2. Add contact\n\t3. Adjust contact\n\t4. Quit\nChoose an option: ");
        core_string_input_strict(&input, 1);

        if (strcmp(core_string_get_data(&input), "1") == 0)
        {
            core_object_call(contact_book, "print", 0, NULL);
        }
        else if (strcmp(core_string_get_data(&input), "2") == 0)
        {
            core_object_call(contact_book, "add", 0, NULL);
        }
        else if (strcmp(core_string_get_data(&input), "3") == 0)
        {
            core_object_call(contact_book, "adjust", 0, NULL);
        }
        else if (strcmp(core_string_get_data(&input), "4") == 0)
        {
            printf("Quitting...\n");
        }
        else
        {
            printf("\t\t- Invalid input!\n");
        }

    } while (strcmp(core_string_get_data(&input), "4") != 0);
    

    core_string_destroy(&input);
}