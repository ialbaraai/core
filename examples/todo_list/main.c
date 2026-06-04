#include <stdio.h>
#include <string.h>

#include <core.h>

#include "include/tasks.h"

void handle_loop(object_t* tasks_manager);

int main(void)
{
    Tasks_Initialize();

    object_t tasksInst = {0};
    Tasks_class.constructor(&tasksInst, 0, NULL);

    handle_loop(&tasksInst);

    Tasks_class.destructor(&tasksInst);

    core_class_destroy(&Tasks_class);
}

void handle_loop(object_t* tasks_manager)
{
    string_t input = {0};
    core_string_init(&input, 8);

    printf("---------- Tasks Manager ----------\n");

    do
    {
        printf("What would you like to do?\n\t1. List tasks\n\t2. Add task\n\t3. Adjust task\n\t4. Quit\nChoose an option: ");
        core_string_input_strict(&input, 4);

        if (strcmp(core_string_get_data(&input), "1") == 0)
        {
            core_object_call(tasks_manager, "print", 0, NULL);
        }
        else if (strcmp(core_string_get_data(&input), "2") == 0)
        {
            core_object_call(tasks_manager, "add", 0, NULL);
        }
        else if (strcmp(core_string_get_data(&input), "3") == 0)
        {
            core_object_call(tasks_manager, "adjust", 0, NULL);
        }
        else if (strcmp(core_string_get_data(&input), "4") == 0)
        {
            printf("Quitting...\n");
        }
        else
        {
            printf(" - Invalid input!\n");
        }
    } while (strcmp(core_string_get_data(&input), "4") != 0);
    
    core_string_destroy(&input);
}