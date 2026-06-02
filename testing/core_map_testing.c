#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/containers/core_map.h"
#include "../include/core/core_string.h"

// ── compare helpers ───────────────────────────────────────────────────────────

static int compare_int(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}

// ── init ──────────────────────────────────────────────────────────────────────

void test_init(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 8, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    assert(m._Map_Key_Data != NULL);
    assert(m._Map_Value_Data != NULL);

    assert(core_map_get_capacity(&m) == 8);
    assert(core_map_get_size(&m) == 0);

    assert(core_map_get_key_size(&m) == sizeof(int));
    assert(core_map_get_value_size(&m) == sizeof(int));

    core_map_destroy(&m);
}

// ── push_back / get_value ─────────────────────────────────────────────────────

void test_push_back_and_get(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    for (int i = 0; i < 5; ++i)
    {
        int v = i * 10;
        assert(core_map_push_back(&m, &i, &v) == SUCCESS);
    }

    assert(core_map_get_size(&m) == 5);

    for (int i = 0; i < 5; ++i)
    {
        void* val = core_map_get_value(&m, &i);

        assert(val != NULL);
        assert(*(int*)val == i * 10);
    }

    core_map_destroy(&m);
}

void test_push_back_grows(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 2, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 0;
    int v = 0;

    core_map_push_back(&m, &k, &v);
    core_map_push_back(&m, &k, &v);
    core_map_push_back(&m, &k, &v); // triggers realloc

    assert(core_map_get_size(&m) == 3);
    assert(core_map_get_capacity(&m) >= 3);

    core_map_destroy(&m);
}

void test_push_back_string_keys_and_values(void)
{
    map_t m = {0};

    assert(core_map_init(&m,
                          4,
                          sizeof(string_t),
                          sizeof(string_t),
                          core_string_copy_callback,
                          core_string_destroy_callback,
                          core_string_copy_callback,
                          core_string_destroy_callback,
                          core_string_compare_callback) == SUCCESS);

    const char* pairs[][2] =
    {
        {"name", "Albaraa"},
        {"lang", "C"},
        {"proj", "AlgoLab"}
    };

    for (int i = 0; i < 3; ++i)
    {
        string_t k = {0};
        string_t v = {0};

        assert(core_string_init_data(&k, 32, pairs[i][0]) == SUCCESS);
        assert(core_string_init_data(&v, 32, pairs[i][1]) == SUCCESS);

        core_map_push_back(&m, &k, &v);

        core_string_destroy(&k);
        core_string_destroy(&v);
    }

    assert(core_map_get_size(&m) == 3);

    string_t lookup = {0};

    assert(core_string_init_data(&lookup, 32, "lang") == SUCCESS);

    void* val = core_map_get_value(&m, &lookup);

    assert(val != NULL);
    assert(strcmp(core_string_get_data((string_t*)val), "C") == 0);

    core_string_destroy(&lookup);

    core_map_destroy(&m);
}

// ── get_value_at (copy-out) ───────────────────────────────────────────────────

void test_get_value_at(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 7;
    int v = 77;

    core_map_push_back(&m, &k, &v);

    int out = 0;

    assert(core_map_get_value_at(&m, &k, &out) == SUCCESS);
    assert(out == 77);

    int missing = 99;

    assert(core_map_get_value_at(&m, &missing, &out) == 0);

    core_map_destroy(&m);
}

// ── set ───────────────────────────────────────────────────────────────────────

void test_set(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 1;
    int v1 = 100;
    int v2 = 200;

    core_map_push_back(&m, &k, &v1);

    assert(core_map_set(&m, &k, &v2) == SUCCESS);
    assert(*(int*)core_map_get_value(&m, &k) == 200);

    core_map_destroy(&m);
}

void test_set_missing_key(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 99;
    int v = 1;

    assert(core_map_set(&m, &k, &v) == 0);

    core_map_destroy(&m);
}

void test_set_string_value_destroys_old(void)
{
    map_t m = {0};

    assert(core_map_init(&m,
                          4,
                          sizeof(int),
                          sizeof(string_t),
                          NULL,
                          NULL,
                          core_string_copy_callback,
                          core_string_destroy_callback,
                          compare_int) == SUCCESS);

    int k = 1;

    string_t old_v = {0};
    string_t new_v = {0};

    assert(core_string_init_data(&old_v, 16, "old") == SUCCESS);
    assert(core_string_init_data(&new_v, 16, "new") == SUCCESS);

    core_map_push_back(&m, &k, &old_v);

    core_string_destroy(&old_v);

    core_map_set(&m, &k, &new_v);

    core_string_destroy(&new_v);

    assert(strcmp(core_string_get_data((string_t*)core_map_get_value(&m, &k)), "new") == 0);

    core_map_destroy(&m);
}

// ── pop_back ──────────────────────────────────────────────────────────────────

void test_pop_back(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 5;
    int v = 50;

    core_map_push_back(&m, &k, &v);

    assert(core_map_pop_back(&m) == SUCCESS);
    assert(core_map_get_size(&m) == 0);

    assert(core_map_pop_back(&m) == 0);

    core_map_destroy(&m);
}

// ── remove ────────────────────────────────────────────────────────────────────

void test_remove(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 8, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    for (int i = 0; i < 5; ++i)
    {
        int v = i * 2;
        core_map_push_back(&m, &i, &v);
    }

    int k = 2;

    assert(core_map_remove(&m, &k) == SUCCESS);

    assert(core_map_get_size(&m) == 4);
    assert(core_map_get_value(&m, &k) == NULL);

    int missing = 99;

    assert(core_map_remove(&m, &missing) == 0);

    core_map_destroy(&m);
}

void test_remove_last_element(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 42;
    int v = 420;

    core_map_push_back(&m, &k, &v);

    assert(core_map_remove(&m, &k) == SUCCESS);
    assert(core_map_get_size(&m) == 0);

    core_map_destroy(&m);
}

void test_remove_string_pair(void)
{
    map_t m = {0};

    assert(core_map_init(&m,
                          4,
                          sizeof(string_t),
                          sizeof(string_t),
                          core_string_copy_callback,
                          core_string_destroy_callback,
                          core_string_copy_callback,
                          core_string_destroy_callback,
                          core_string_compare_callback) == SUCCESS);

    string_t k = {0};
    string_t v = {0};

    assert(core_string_init_data(&k, 16, "key") == SUCCESS);
    assert(core_string_init_data(&v, 16, "val") == SUCCESS);

    core_map_push_back(&m, &k, &v);

    core_string_destroy(&k);
    core_string_destroy(&v);

    string_t lookup = {0};

    assert(core_string_init_data(&lookup, 16, "key") == SUCCESS);

    assert(core_map_remove(&m, &lookup) == SUCCESS);
    assert(core_map_get_size(&m) == 0);

    core_string_destroy(&lookup);

    core_map_destroy(&m);
}

// ── first / last ──────────────────────────────────────────────────────────────

void test_first_last(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k0 = 0;
    int v0 = 100;

    int k1 = 1;
    int v1 = 200;

    core_map_push_back(&m, &k0, &v0);
    core_map_push_back(&m, &k1, &v1);

    assert(*(int*)core_map_first_key(&m) == 0);
    assert(*(int*)core_map_last_key(&m) == 1);

    assert(*(int*)core_map_first_value(&m) == 100);
    assert(*(int*)core_map_last_value(&m) == 200);

    core_map_destroy(&m);
}

void test_last_empty(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    assert(core_map_last_key(&m) == NULL);
    assert(core_map_last_value(&m) == NULL);

    core_map_destroy(&m);
}

// ── foreach ───────────────────────────────────────────────────────────────────

static int foreach_key_sum = 0;
static int foreach_val_sum = 0;

static void sum_pairs(void* key, void* value)
{
    foreach_key_sum += *(int*)key;
    foreach_val_sum += *(int*)value;
}

void test_foreach(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    for (int i = 1; i <= 3; ++i)
    {
        int v = i * 10;
        core_map_push_back(&m, &i, &v);
    }

    foreach_key_sum = 0;
    foreach_val_sum = 0;

    core_map_foreach(&m, sum_pairs);

    assert(foreach_key_sum == 6);
    assert(foreach_val_sum == 60);

    core_map_destroy(&m);
}

// ── destroy ───────────────────────────────────────────────────────────────────

void test_destroy(void)
{
    map_t m = {0};

    assert(core_map_init(&m, 4, sizeof(int), sizeof(int),
                          NULL, NULL, NULL, NULL,
                          compare_int) == SUCCESS);

    int k = 1;
    int v = 10;

    core_map_push_back(&m, &k, &v);

    core_map_destroy(&m);

    assert(m._Map_Key_Data == NULL);
    assert(m._Map_Value_Data == NULL);

    assert(m._Map_Size == 0);
    assert(m._Map_Capacity == 0);
}

// ── null guard sanity ─────────────────────────────────────────────────────────

void test_null_guards(void)
{
    assert(core_map_get_size(NULL) == 0);
    assert(core_map_get_capacity(NULL) == 0);

    assert(core_map_get_key_size(NULL) == 0);
    assert(core_map_get_value_size(NULL) == 0);

    assert(core_map_get_key_data(NULL) == NULL);
    assert(core_map_get_value_data(NULL) == NULL);

    assert(core_map_first_key(NULL) == NULL);
    assert(core_map_last_key(NULL) == NULL);

    assert(core_map_first_value(NULL) == NULL);
    assert(core_map_last_value(NULL) == NULL);

    assert(core_map_get_value(NULL, NULL) == NULL);

    assert(core_map_push_back(NULL, NULL, NULL) == MAP_POINTER_NULL_ERROR);
    assert(core_map_pop_back(NULL) == MAP_POINTER_NULL_ERROR);

    assert(core_map_set(NULL, NULL, NULL) == MAP_POINTER_NULL_ERROR);
    assert(core_map_remove(NULL, NULL) == MAP_POINTER_NULL_ERROR);
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(void)
{
    test_init();

    test_push_back_and_get();
    test_push_back_grows();
    test_push_back_string_keys_and_values();

    test_get_value_at();

    test_set();
    test_set_missing_key();
    test_set_string_value_destroys_old();

    test_pop_back();

    test_remove();
    test_remove_last_element();
    test_remove_string_pair();

    test_first_last();
    test_last_empty();

    test_foreach();

    test_destroy();
    test_null_guards();

    printf("All core_map tests passed.\n");

    return 0;
}