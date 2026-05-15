#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/core_string.h"

// ── init ──────────────────────────────────────────────────────────────────────

void test_init(void)
{
    string_t s = core_string_init(16);
    assert(s._String_Data != NULL);
    assert(core_string_get_capacity(&s) == 16);
    assert(core_string_get_size(&s) == 0);
    assert(core_string_get_data(&s)[0] == '\0');
    core_string_destroy(&s);
}

void test_init_data(void)
{
    string_t s = core_string_init_data(32, "hello");
    assert(s._String_Data != NULL);
    assert(core_string_get_size(&s) == 5);
    assert(strcmp(core_string_get_data(&s), "hello") == 0);
    core_string_destroy(&s);
}

void test_init_data_truncate(void)
{
    // capacity smaller than data — should truncate gracefully
    string_t s = core_string_init_data(4, "hello");
    assert(s._String_Data != NULL);
    assert(core_string_get_size(&s) <= 3);
    assert(core_string_get_data(&s)[core_string_get_size(&s)] == '\0');
    core_string_destroy(&s);
}

// ── set ───────────────────────────────────────────────────────────────────────

void test_set(void)
{
    string_t s = core_string_init(8);
    int r = core_string_set(&s, "world");
    assert(r == 1);
    assert(strcmp(core_string_get_data(&s), "world") == 0);
    assert(core_string_get_size(&s) == 5);
    core_string_destroy(&s);
}

void test_set_grows(void)
{
    string_t s = core_string_init(4);
    int r = core_string_set(&s, "this is a long string");
    assert(r == 1);
    assert(strcmp(core_string_get_data(&s), "this is a long string") == 0);
    core_string_destroy(&s);
}

void test_set_null(void)
{
    string_t s = core_string_init(8);
    int r = core_string_set(&s, NULL);
    assert(r == 0);
    core_string_destroy(&s);
}

// ── copy ──────────────────────────────────────────────────────────────────────

void test_copy(void)
{
    string_t src = core_string_init_data(16, "copy me");
    string_t dst = core_string_init(16);
    int r = core_string_copy(&dst, &src);
    assert(r == 1);
    assert(strcmp(core_string_get_data(&dst), "copy me") == 0);
    assert(core_string_get_size(&dst) == core_string_get_size(&src));
    core_string_destroy(&src);
    core_string_destroy(&dst);
}

void test_copy_grows_destination(void)
{
    string_t src = core_string_init_data(32, "a longer source string");
    string_t dst = core_string_init(4);
    int r = core_string_copy(&dst, &src);
    assert(r == 1);
    assert(strcmp(core_string_get_data(&dst), "a longer source string") == 0);
    core_string_destroy(&src);
    core_string_destroy(&dst);
}

// ── compare ───────────────────────────────────────────────────────────────────

void test_compare_equal(void)
{
    string_t a = core_string_init_data(16, "same");
    string_t b = core_string_init_data(16, "same");
    assert(core_string_compare(&a, &b) == 1);
    core_string_destroy(&a);
    core_string_destroy(&b);
}

void test_compare_not_equal(void)
{
    string_t a = core_string_init_data(16, "hello");
    string_t b = core_string_init_data(16, "world");
    assert(core_string_compare(&a, &b) == 0);
    core_string_destroy(&a);
    core_string_destroy(&b);
}

// ── append ────────────────────────────────────────────────────────────────────

void test_append_char(void)
{
    string_t s = core_string_init_data(16, "ab");
    int r = core_string_append_char(&s, 'c');
    assert(r == 1);
    assert(strcmp(core_string_get_data(&s), "abc") == 0);
    assert(core_string_get_size(&s) == 3);
    core_string_destroy(&s);
}

void test_append_char_null_rejected(void)
{
    string_t s = core_string_init_data(16, "ab");
    int r = core_string_append_char(&s, '\0');
    assert(r == 0);
    assert(core_string_get_size(&s) == 2);
    core_string_destroy(&s);
}

void test_append_cstr(void)
{
    string_t s = core_string_init_data(16, "hello");
    int r = core_string_append_cstr(&s, " world");
    assert(r == 1);
    assert(strcmp(core_string_get_data(&s), "hello world") == 0);
    core_string_destroy(&s);
}

void test_append_string(void)
{
    string_t a = core_string_init_data(16, "foo");
    string_t b = core_string_init_data(16, "bar");
    int r = core_string_append_string(&a, &b);
    assert(r == 1);
    assert(strcmp(core_string_get_data(&a), "foobar") == 0);
    core_string_destroy(&a);
    core_string_destroy(&b);
}

// ── pop_back ──────────────────────────────────────────────────────────────────

void test_pop_back(void)
{
    string_t s = core_string_init_data(16, "abc");
    int r = core_string_pop_back(&s);
    assert(r == 1);
    assert(core_string_get_size(&s) == 2);
    assert(strcmp(core_string_get_data(&s), "ab") == 0);
    core_string_destroy(&s);
}

void test_pop_back_empty(void)
{
    string_t s = core_string_init(8);
    int r = core_string_pop_back(&s);
    assert(r == 0);
    core_string_destroy(&s);
}

// ── case ──────────────────────────────────────────────────────────────────────

void test_lower(void)
{
    string_t s = core_string_init_data(16, "HELLO");
    core_string_lower(&s);
    assert(strcmp(core_string_get_data(&s), "hello") == 0);
    core_string_destroy(&s);
}

void test_upper(void)
{
    string_t s = core_string_init_data(16, "hello");
    core_string_upper(&s);
    assert(strcmp(core_string_get_data(&s), "HELLO") == 0);
    core_string_destroy(&s);
}

// ── at / find / replace ───────────────────────────────────────────────────────

void test_at(void)
{
    string_t s = core_string_init_data(16, "abc");
    assert(core_string_at(&s, 0) == 'a');
    assert(core_string_at(&s, 2) == 'c');
    assert(core_string_at(&s, 99) == '\0');
    core_string_destroy(&s);
}

void test_find(void)
{
    string_t s = core_string_init_data(16, "hello");
    size_t idx = 0;
    assert(core_string_find(&s, 'l', &idx) == 1);
    assert(idx == 2);
    assert(core_string_find(&s, 'z', NULL) == 0);
    core_string_destroy(&s);
}

void test_replace_at(void)
{
    string_t s = core_string_init_data(16, "abc");
    int r = core_string_replace_at(&s, 1, 'X');
    assert(r == 1);
    assert(strcmp(core_string_get_data(&s), "aXc") == 0);
    core_string_destroy(&s);
}

void test_replace(void)
{
    string_t s = core_string_init_data(16, "aabbaa");
    core_string_replace(&s, 'a', 'Z');
    assert(strcmp(core_string_get_data(&s), "ZZbbZZ") == 0);
    core_string_destroy(&s);
}

// ── destroy ───────────────────────────────────────────────────────────────────

void test_destroy(void)
{
    string_t s = core_string_init_data(16, "bye");
    core_string_destroy(&s);
    assert(s._String_Data == NULL);
    assert(s._String_Capacity == 0);
    assert(s._String_Size == 0);
}

// ── null guard sanity ─────────────────────────────────────────────────────────

void test_null_guards(void)
{
    assert(core_string_get_size(NULL) == 0);
    assert(core_string_get_capacity(NULL) == 0);
    assert(core_string_get_data(NULL) == NULL);
    assert(core_string_at(NULL, 0) == '\0');
    assert(core_string_find(NULL, 'a', NULL) == 0);
    assert(core_string_set(NULL, "x") == 0);
    assert(core_string_copy(NULL, NULL) == 0);
    assert(core_string_compare(NULL, NULL) == 0);
    assert(core_string_pop_back(NULL) == 0);
    assert(core_string_append_char(NULL, 'x') == 0);
    assert(core_string_append_cstr(NULL, "x") == 0);
    assert(core_string_append_string(NULL, NULL) == 0);
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(void)
{
    test_init();
    test_init_data();
    test_init_data_truncate();

    test_set();
    test_set_grows();
    test_set_null();

    test_copy();
    test_copy_grows_destination();

    test_compare_equal();
    test_compare_not_equal();

    test_append_char();
    test_append_char_null_rejected();
    test_append_cstr();
    test_append_string();

    test_pop_back();
    test_pop_back_empty();

    test_lower();
    test_upper();

    test_at();
    test_find();
    test_replace_at();
    test_replace();

    test_destroy();
    test_null_guards();

    printf("All core_string tests passed.\n");
    return 0;
}