#include "../include/core_file.h"

#include <stdio.h>
#include <string.h>

void str_copy(void* dest, const void* src)
{
    string_t str = core_string_init_data(core_string_get_capacity((string_t*)src), core_string_get_data((string_t*)src));

    memcpy(dest, &str, sizeof(string_t));
}
void str_destroy(void* object)
{
    if ((string_t*)object != NULL)
    {
        core_string_destroy((string_t*)object);
    }
}

file_t core_file_init(const char* filepath, const size_t initial_lines_size, const size_t initial_line_size)
{
    file_t empty = {0};

    if (!filepath) return empty;

    file_t file = {._File_Filepath = core_string_init_data(strlen(filepath) + 1, filepath), ._File_Data = core_vector_init(initial_lines_size, sizeof(string_t), str_copy, str_destroy), ._File_Initial_Line_Size = initial_line_size};

    return file;
}

int core_file_read(file_t* file)
{
    if (!file) return 0;
    if (!file->_File_Data._Vector_Data) return 0;
    if (!file->_File_Filepath._String_Data) return 0;

    FILE* cfile = fopen(core_string_get_data(&file->_File_Filepath), "r");

    if (!cfile) return 0;

    core_vector_clear(&file->_File_Data);

    int curchar = 0;

    string_t curstr = core_string_init(file->_File_Initial_Line_Size);

    while ((curchar = fgetc(cfile)) != EOF)
    {
        if (curchar == '\n')
        {
            core_vector_push_back(&file->_File_Data, &curstr);
            core_string_destroy(&curstr);
            curstr = core_string_init(file->_File_Initial_Line_Size);
        }
        else
        {
            core_string_append_char(&curstr, (char)curchar);
        }
    }

    if (core_string_get_size(&curstr) > 0)
    {
        core_vector_push_back(&file->_File_Data, &curstr);
    }

    core_string_destroy(&curstr);

    fclose(cfile);

    return 1;
}
int core_file_write(const file_t* file)
{
    if (!file) return 0;
    if (!file->_File_Filepath._String_Data) return 0;
    if (!file->_File_Data._Vector_Data) return 0;

    FILE* cfile = fopen(core_string_get_data(&file->_File_Filepath), "w");

    if (!cfile) return 0;

    for (size_t i = 0; i < file->_File_Data._Vector_Size; ++i)
    {
        if (i > 0) fputc('\n', cfile);
        fprintf(cfile, "%s", core_string_get_data(core_vector_get(&file->_File_Data, i)));
    }

    fclose(cfile);

    return 1;
}

string_t* core_file_get_filepath(const file_t* file)
{
    if (!file) return NULL;
    if (!file->_File_Filepath._String_Data) return NULL;

    return &file->_File_Filepath;
}
vector_t* core_file_get_lines(const file_t* file)
{
    if (!file) return NULL;
    if (!file->_File_Data._Vector_Data) return NULL;

    return &file->_File_Data;
}
size_t core_file_get_line_size(const file_t* file)
{
    if (!file) return 0;

    return file->_File_Initial_Line_Size;
}

void core_file_destroy(file_t* file)
{
    if (!file) return;

    core_vector_destroy(&file->_File_Data);
    core_string_destroy(&file->_File_Filepath);

    file->_File_Initial_Line_Size = 0;
}