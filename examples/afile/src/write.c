#include "write.h"

#include <stdio.h>
#include <string.h>
#include "terminal.h"

class_t Write_Class = {0};

void print_str(const size_t index, void* value)
{
    printf("%s\n", core_string_get_data((string_t*)value));
}

void write_run(object_t* object, int argc, void** argv)
{
    string_t* filepath = (string_t*)core_member_get_value(core_object_member(object, "file"));

    if (filepath)
    {
        file_t file = {0};
        if (core_file_init(&file, core_string_get_data(filepath), 16, 64) == SUCCESS)
        {
            core_file_read(&file);

            core_vector_foreach(core_file_get_lines(&file), print_str);

            string_t curline = {0};
            core_string_init(&curline, 64);

            terminal_raw_mode_enter();

            while (true)
            {
                printf("> ");
                fflush(stdout);

                while(true)
                {
                    char c = getchar();

                    if (c == 0x0F)
                    {
                        core_vector_push_back(core_file_get_lines(&file), &curline);
                        goto save;
                    }
                    else if (c == '\n')
                    {
                        core_vector_push_back(core_file_get_lines(&file), &curline);
                        core_string_set(&curline, "");
                        printf("\n");
                        break;
                    }
                    else if (c == 0x7F || c == 0x78)
                    {
                        if (core_string_get_size(&curline) > 0)
                        {
                            core_string_pop_back(&curline);
                            printf("\b \b");
                            fflush(stdout);
                        }
                    }
                    else
                    {
                        core_string_append_char(&curline, c);
                        putchar(c);
                        fflush(stdout);
                    }
                }
            }

            save:
            terminal_raw_mode_exit();

            core_file_write(&file);
            printf("\nFile saved to \"%s\"\n", core_string_get_data(filepath));
            
            core_string_destroy(&curline);
            core_file_destroy(&file);
        }
        else
        {
            printf(" - Couldn't open file\n");
        }
    }
}

int write_constructor(object_t* write, int argc, void** argv)
{
    if (argc == 1)
    {
        string_t* file = (string_t*)argv[0];

        member_t filememb = {0};
        core_member_init_data(&filememb, "file", write, sizeof(string_t), file, PUBLIC, core_string_copy_callback, core_string_destroy_callback);
        method_t run = {0};
        core_method_init(&run, "run", write_run, PUBLIC);

        core_object_init(write, &Write_Class, "write", 1, &filememb, 1, &run);

        core_member_destroy(&filememb);
        core_method_destroy(&run);

        return SUCCESS;
    }

    printf(" - Invalid usage of \"file (file name)\"\n");

    return OBJECT_CANNOT_ALLOCATE_ERROR;
}
void write_destructor(object_t* write)
{
    core_object_destroy(write);
}

void Write_Initialize()
{
    core_class_init(&Write_Class, "write", NULL, 0, NULL, 0, NULL, write_constructor, write_destructor);
}