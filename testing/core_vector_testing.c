#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/core_vector.h"
#include "../include/core_string.h"

// ── helpers for string_t elements ────────────────────────────────────────────

static void copy_str(void* dst, const void* src)
{
    const string_t* s = (const string_t*)src;
    string_t copy = core_string_init_data(core_string_get_capacity(s), core_string_get_data(s));
    memcpy(dst, &copy, sizeof(string_t));
}

static void destroy_str(void* obj)
{
    core_string_destroy((string_t*)obj);
}

// ── init ──────────────────────────────────────────────────────────────────────

void test_init(void)
{
    vector_t v = core_vector_init(8, sizeof(int), NULL, NULL);
    assert(v._Vector_Data != NULL);
    assert(core_vector_get_capacity(&v) == 8);
    assert(core_vector_get_size(&v) == 0);
    assert(core_vector_get_element_size(&v) == sizeof(int));
    core_vector_destroy(&v);
}

void test_init_data(void)
{
    int arr[] = {1, 2, 3};
    vector_t v = core_vector_init_data(8, sizeof(int), 3, NULL, NULL, arr);
    assert(core_vector_get_size(&v) == 3);
    assert(*(int*)core_vector_get(&v, 0) == 1);
    assert(*(int*)core_vector_get(&v, 2) == 3);
    core_vector_destroy(&v);
}

// ── push_back / get ───────────────────────────────────────────────────────────

void test_push_back_primitives(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    for (int i = 0; i < 10; ++i)
        assert(core_vector_push_back(&v, &i) == 1);

    assert(core_vector_get_size(&v) == 10);
    for (int i = 0; i < 10; ++i)
        assert(*(int*)core_vector_get(&v, i) == i);

    core_vector_destroy(&v);
}

void test_push_back_grows(void)
{
    vector_t v = core_vector_init(2, sizeof(int), NULL, NULL);
    int x = 99;
    core_vector_push_back(&v, &x);
    core_vector_push_back(&v, &x);
    core_vector_push_back(&v, &x); // triggers realloc
    assert(core_vector_get_size(&v) == 3);
    assert(core_vector_get_capacity(&v) >= 3);
    core_vector_destroy(&v);
}

void test_push_back_strings(void)
{
    vector_t v = core_vector_init(4, sizeof(string_t), copy_str, destroy_str);
    const char* words[] = {"alpha", "beta", "gamma"};

    for (int i = 0; i < 3; ++i)
    {
        string_t s = core_string_init_data(32, words[i]);
        core_vector_push_back(&v, &s);
        core_string_destroy(&s);
    }

    assert(core_vector_get_size(&v) == 3);
    for (int i = 0; i < 3; ++i)
        assert(strcmp(core_string_get_data((string_t*)core_vector_get(&v, i)), words[i]) == 0);

    core_vector_destroy(&v);
}

// ── pop_back ──────────────────────────────────────────────────────────────────

void test_pop_back(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    int x = 5;
    core_vector_push_back(&v, &x);
    assert(core_vector_pop_back(&v) == 1);
    assert(core_vector_get_size(&v) == 0);
    assert(core_vector_pop_back(&v) == 0); // empty
    core_vector_destroy(&v);
}

void test_pop_back_calls_destroy(void)
{
    vector_t v = core_vector_init(4, sizeof(string_t), copy_str, destroy_str);
    string_t s = core_string_init_data(16, "test");
    core_vector_push_back(&v, &s);
    core_string_destroy(&s);
    assert(core_vector_pop_back(&v) == 1); // should call destroy_str without crashing
    assert(core_vector_get_size(&v) == 0);
    core_vector_destroy(&v);
}

// ── set ───────────────────────────────────────────────────────────────────────

void test_set(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    int x = 1, y = 99;
    core_vector_push_back(&v, &x);
    assert(core_vector_set(&v, 0, &y) == 1);
    assert(*(int*)core_vector_get(&v, 0) == 99);
    assert(core_vector_set(&v, 99, &y) == 0); // out of bounds
    core_vector_destroy(&v);
}

void test_set_strings_destroys_old(void)
{
    vector_t v = core_vector_init(4, sizeof(string_t), copy_str, destroy_str);
    string_t a = core_string_init_data(16, "old");
    string_t b = core_string_init_data(16, "new");
    core_vector_push_back(&v, &a);
    core_string_destroy(&a);
    core_vector_set(&v, 0, &b);  // must destroy "old" before writing "new"
    core_string_destroy(&b);
    assert(strcmp(core_string_get_data((string_t*)core_vector_get(&v, 0)), "new") == 0);
    core_vector_destroy(&v);
}

// ── remove ────────────────────────────────────────────────────────────────────

void test_remove(void)
{
    vector_t v = core_vector_init(8, sizeof(int), NULL, NULL);
    for (int i = 0; i < 5; ++i)
        core_vector_push_back(&v, &i);

    assert(core_vector_remove(&v, 2) == 1);
    assert(core_vector_get_size(&v) == 4);
    assert(core_vector_remove(&v, 99) == 0); // out of bounds
    core_vector_destroy(&v);
}

void test_remove_last(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    int x = 42;
    core_vector_push_back(&v, &x);
    assert(core_vector_remove(&v, 0) == 1);
    assert(core_vector_get_size(&v) == 0);
    core_vector_destroy(&v);
}

// ── at ────────────────────────────────────────────────────────────────────────

void test_at(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    int x = 7;
    core_vector_push_back(&v, &x);
    int out = 0;
    assert(core_vector_at(&v, 0, &out) == 1);
    assert(out == 7);
    assert(core_vector_at(&v, 99, &out) == 0);
    core_vector_destroy(&v);
}

// ── first / last ──────────────────────────────────────────────────────────────

void test_first_last(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    int a = 1, b = 2, c = 3;
    core_vector_push_back(&v, &a);
    core_vector_push_back(&v, &b);
    core_vector_push_back(&v, &c);
    assert(*(int*)core_vector_first(&v) == 1);
    assert(*(int*)core_vector_last(&v) == 3);
    core_vector_destroy(&v);
}

void test_last_empty(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    assert(core_vector_last(&v) == NULL);
    core_vector_destroy(&v);
}

// ── foreach ───────────────────────────────────────────────────────────────────

static int foreach_sum = 0;
static void sum_fn(size_t index, void* data) { (void)index; foreach_sum += *(int*)data; }

void test_foreach(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    for (int i = 1; i <= 4; ++i)
        core_vector_push_back(&v, &i);
    foreach_sum = 0;
    core_vector_foreach(&v, sum_fn);
    assert(foreach_sum == 10);
    core_vector_destroy(&v);
}

// ── destroy ───────────────────────────────────────────────────────────────────

void test_destroy(void)
{
    vector_t v = core_vector_init(4, sizeof(int), NULL, NULL);
    int x = 1;
    core_vector_push_back(&v, &x);
    core_vector_destroy(&v);
    assert(v._Vector_Data == NULL);
    assert(v._Vector_Size == 0);
    assert(v._Vector_Capacity == 0);
}

// ── null guard sanity ─────────────────────────────────────────────────────────

void test_null_guards(void)
{
    assert(core_vector_get_size(NULL) == 0);
    assert(core_vector_get_capacity(NULL) == 0);
    assert(core_vector_get_element_size(NULL) == 0);
    assert(core_vector_get_data(NULL) == NULL);
    assert(core_vector_get(NULL, 0) == NULL);
    assert(core_vector_first(NULL) == NULL);
    assert(core_vector_last(NULL) == NULL);
    assert(core_vector_push_back(NULL, NULL) == 0);
    assert(core_vector_pop_back(NULL) == 0);
    assert(core_vector_set(NULL, 0, NULL) == 0);
    assert(core_vector_remove(NULL, 0) == 0);
    assert(core_vector_at(NULL, 0, NULL) == 0);
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(void)
{
    test_init();
    test_init_data();

    test_push_back_primitives();
    test_push_back_grows();
    test_push_back_strings();

    test_pop_back();
    test_pop_back_calls_destroy();

    test_set();
    test_set_strings_destroys_old();

    test_remove();
    test_remove_last();

    test_at();
    test_first_last();
    test_last_empty();

    test_foreach();
    test_destroy();
    test_null_guards();

    printf("All core_vector tests passed.\n");
    return 0;
}