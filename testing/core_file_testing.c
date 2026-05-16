#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/core_file.h"

// ── init ──────────────────────────────────────────────────────────────────────

void test_init(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    assert(core_file_get_filepath(&f) != NULL);
    assert(strcmp(core_string_get_data(core_file_get_filepath(&f)), "test_read.txt") == 0);
    assert(core_file_get_lines(&f) != NULL);
    assert(core_vector_get_size(core_file_get_lines(&f)) == 0);
    assert(core_file_get_line_size(&f) == 64);
    core_file_destroy(&f);
}

void test_init_null_filepath(void)
{
    file_t f = core_file_init(NULL, 16, 64);
    assert(core_file_get_filepath(&f) == NULL);
    assert(core_file_get_lines(&f) == NULL);
    assert(core_file_get_line_size(&f) == 0);
}

// ── read ──────────────────────────────────────────────────────────────────────

void test_read(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    assert(core_file_read(&f) == 1);
    assert(core_vector_get_size(core_file_get_lines(&f)) > 0);
    core_file_destroy(&f);
}

void test_read_invalid_file(void)
{
    file_t f = core_file_init("nonexistent.txt", 16, 64);
    assert(core_file_read(&f) == 0);
    assert(core_vector_get_size(core_file_get_lines(&f)) == 0);
    core_file_destroy(&f);
}

void test_read_clears_previous_data(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    core_file_read(&f);
    size_t first_size = core_vector_get_size(core_file_get_lines(&f));
    core_file_read(&f);
    assert(core_vector_get_size(core_file_get_lines(&f)) == first_size);
    core_file_destroy(&f);
}

void test_read_different_files(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    core_file_read(&f);
    size_t first_size = core_vector_get_size(core_file_get_lines(&f));

    core_string_set(core_file_get_filepath(&f), "test_read2.txt");
    core_file_read(&f);
    size_t second_size = core_vector_get_size(core_file_get_lines(&f));

    assert(first_size != second_size);
    core_file_destroy(&f);
}

void test_read_line_content(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    core_file_read(&f);
    string_t* first = (string_t*)core_vector_get(core_file_get_lines(&f), 0);
    assert(first != NULL);
    assert(core_string_get_size(first) > 0);
    core_file_destroy(&f);
}

// ── write ─────────────────────────────────────────────────────────────────────

void test_write(void)
{
    file_t f = core_file_init("test_write.txt", 16, 64);
    core_file_read(&f);
    assert(core_file_write(&f) == 1);
    core_file_destroy(&f);
}

void test_write_mutated_line(void)
{
    file_t f = core_file_init("test_write.txt", 16, 64);
    core_file_read(&f);

    string_t* line = (string_t*)core_vector_get(core_file_get_lines(&f), 0);
    assert(line != NULL);
    core_string_set(line, "mutated line");
    assert(core_file_write(&f) == 1);

    core_file_read(&f);
    string_t* verify = (string_t*)core_vector_get(core_file_get_lines(&f), 0);
    assert(strcmp(core_string_get_data(verify), "mutated line") == 0);

    core_file_destroy(&f);
}

void test_write_invalid_path(void)
{
    file_t f = core_file_init("/invalid/path/file.txt", 16, 64);
    core_file_read(&f);
    assert(core_file_write(&f) == 0);
    core_file_destroy(&f);
}

void test_write_to_different_file(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    core_file_read(&f);
    size_t line_count = core_vector_get_size(core_file_get_lines(&f));

    core_string_set(core_file_get_filepath(&f), "test_write.txt");
    assert(core_file_write(&f) == 1);

    core_file_read(&f);
    assert(core_vector_get_size(core_file_get_lines(&f)) == line_count);

    core_file_destroy(&f);
}

// ── filepath set via getter ───────────────────────────────────────────────────

void test_set_filepath_via_getter(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    core_string_set(core_file_get_filepath(&f), "updated.txt");
    assert(strcmp(core_string_get_data(core_file_get_filepath(&f)), "updated.txt") == 0);
    core_file_destroy(&f);
}

// ── getters ───────────────────────────────────────────────────────────────────

void test_get_line_size(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    assert(core_file_get_line_size(&f) == 64);
    core_file_destroy(&f);
}

// ── destroy ───────────────────────────────────────────────────────────────────

void test_destroy(void)
{
    file_t f = core_file_init("test_read.txt", 16, 64);
    core_file_read(&f);
    core_file_destroy(&f);
    assert(f._File_Filepath._String_Data == NULL);
    assert(f._File_Data._Vector_Data == NULL);
    assert(f._File_Data._Vector_Size == 0);
    assert(f._File_Data._Vector_Capacity == 0);
    assert(f._File_Initial_Line_Size == 0);
}

// ── null guard sanity ─────────────────────────────────────────────────────────

void test_null_guards(void)
{
    assert(core_file_read(NULL) == 0);
    assert(core_file_write(NULL) == 0);
    assert(core_file_get_filepath(NULL) == NULL);
    assert(core_file_get_lines(NULL) == NULL);
    assert(core_file_get_line_size(NULL) == 0);
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(void)
{
    test_init();
    test_init_null_filepath();

    test_read();
    test_read_invalid_file();
    test_read_clears_previous_data();
    test_read_different_files();
    test_read_line_content();

    test_write();
    test_write_mutated_line();
    test_write_invalid_path();
    test_write_to_different_file();

    test_set_filepath_via_getter();

    test_get_line_size();

    test_destroy();
    test_null_guards();

    printf("All core_file tests passed.\n");
    return 0;
}