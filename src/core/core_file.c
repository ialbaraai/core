#include "../../include/core/core_file.h"

#include <stdio.h>
#include <string.h>

int core_file_init(file_t* file, const char* filepath, const size_t initial_lines_size, const size_t initial_line_size)
{
    if (!file) return FILE_POINTER_NULL_ERROR;
    if (!filepath) return STRING_CSTR_NULL_ERROR;

    core_string_init_data(&file->_File_Filepath, strlen(filepath) + 1, filepath);
    core_vector_init(&file->_File_Data, initial_lines_size, sizeof(string_t), core_string_copy_callback, core_string_destroy_callback);
    file->_File_Initial_Line_Size = initial_line_size;

    return SUCCESS;
}

int core_file_read(file_t* file)
{
    if (!file) return FILE_POINTER_NULL_ERROR;
    if (!file->_File_Data._Vector_Data) return 0;
    if (!file->_File_Filepath._String_Data) return STRING_DATA_NULL_ERROR;

    FILE* cfile = fopen(core_string_get_data(&file->_File_Filepath), "r");

    if (!cfile) return FILE_CANNOT_OPEN_ERROR;

    core_vector_clear(&file->_File_Data);

    int curchar = 0;

    string_t curstr = {0};
    core_string_init(&curstr, file->_File_Initial_Line_Size);

    while ((curchar = fgetc(cfile)) != EOF)
    {
        if (curchar == '\n')
        {
            core_vector_push_back(&file->_File_Data, &curstr);
            core_string_set(&curstr, "");
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

    return SUCCESS;
}
int core_file_write(const file_t* file)
{
    if (!file) return FILE_POINTER_NULL_ERROR;
    if (!file->_File_Data._Vector_Data) return 0;
    if (!file->_File_Filepath._String_Data) return STRING_DATA_NULL_ERROR;

    FILE* cfile = fopen(core_string_get_data(&file->_File_Filepath), "w");

    if (!cfile) return FILE_CANNOT_OPEN_ERROR;

    for (size_t i = 0; i < file->_File_Data._Vector_Size; ++i)
    {
        if (i > 0) fputc('\n', cfile);
        fprintf(cfile, "%s", core_string_get_data(core_vector_get(&file->_File_Data, i)));
    }

    fclose(cfile);

    return SUCCESS;
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