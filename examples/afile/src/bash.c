#include "bash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class_t Bash_Class = {0};

void split_string(const string_t* string, vector_t* result, const char split)
{
    core_vector_clear(result);

    string_t curstr = {0};
    core_string_init(&curstr, 32);

    for (size_t i = 0; i < core_string_get_size(string); ++i)
    {
        if (core_string_at(string, i) == split)
        {
            core_vector_push_back(result, &curstr);
            core_string_set(&curstr, "");
        }
        else
        {
            core_string_append_char(&curstr, core_string_at(string, i));
        }
    }

    if (core_string_get_size(&curstr) > 0)
    {
        core_vector_push_back(result, &curstr);
    }

    core_string_destroy(&curstr);
}
void print_line(const size_t index, void* value)
{
    if (value)
        printf("%s,", core_string_get_data((string_t*)value));
}

void bash_run(object_t* bash, int argc, void** argv)
{
    string_t raw_input = {0};
    core_string_init(&raw_input, 128);

    vector_t line = {0};
    core_vector_init(&line, 8, sizeof(string_t), core_string_copy_callback, core_string_destroy_callback);

    while (true)
    {
        printf("$ ");
        core_string_input(&raw_input);
        split_string(&raw_input, &line, ' ');

        if (core_vector_get_size(&line) == 1)
        {
            if (strcmp(core_string_get_data((string_t*)core_vector_get(&line, 0)), "exit") == 0 || strcmp(core_string_get_data((string_t*)core_vector_get(&line, 0)), "quit") == 0)
            {
                break;
            }
            else if (strcmp(core_string_get_data((string_t*)core_vector_get(&line, 0)), "clear") == 0)
            {
                #ifdef _WIN32
                system("cls");
                #else
                system("clear");
                #endif
            }
            else
            {
                printf(" - invalid command: %s\n", core_string_get_data(&raw_input));
            }
        }
        else if (core_vector_get_size(&line) == 2)
        {
            if (strcmp(core_string_get_data((string_t*)core_vector_get(&line, 0)), "file") == 0)
            {
                string_t* file = (string_t*)core_vector_get(&line, 1);

                void* args[] = {file};

                object_t fileWrite = {0};
                Write_Class.constructor(&fileWrite, 1, args);
                core_object_call(&fileWrite, "run", 0, NULL);
                Write_Class.destructor(&fileWrite);
            }
            else
            {
                printf(" - invalid command: %s\n", core_string_get_data(&raw_input));
            }
        }
        else
        {
            printf(" - invalid command: %s\n", core_string_get_data(&raw_input));
        }
    }

    core_vector_destroy(&line);
    core_string_destroy(&raw_input);
}

int bash_constructor(object_t* bash, int argc, void** argv)
{
    method_t mthd = {0};
    core_method_init(&mthd, "run", bash_run, PUBLIC);

    core_object_init(bash, &Bash_Class, "bash", 0, NULL, 1, &mthd);

    core_method_destroy(&mthd);

    return SUCCESS;
}
void bash_destructor(object_t* bash)
{
    core_object_destroy(bash);
}

void Bash_Initialize()
{
    core_class_init(&Bash_Class, "bash", NULL, 0, NULL, 0, NULL, bash_constructor, bash_destructor);
}