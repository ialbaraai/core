#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/oop/core_oop.h"

// ── helpers ───────────────────────────────────────────────────────────────────

static void dummy_method(object_t* this, int argc, void** argv) { (void)this; (void)argc; (void)argv; }
static int method_called = 0;
static void tracking_method(object_t* this, int argc, void** argv) { (void)this; (void)argc; (void)argv; method_called = 1; }
static int base_method_called = 0;
static void base_tracking_method(object_t* this, int argc, void** argv) { (void)this; (void)argc; (void)argv; base_method_called = 1; }

// ── member: init ──────────────────────────────────────────────────────────────

void test_member_init(void)
{
    member_t m = core_member_init("score", NULL, sizeof(int), PUBLIC, NULL, NULL);
    assert(core_member_get_name(&m) != NULL);
    assert(strcmp(core_string_get_data(core_member_get_name(&m)), "score") == 0);
    assert(core_member_get_value_size(&m) == sizeof(int));
    assert(core_member_get_value(&m) != NULL);
    assert(*core_member_get_visibility(&m) == PUBLIC);
    core_member_destroy(&m);
}

void test_member_init_data(void)
{
    int val = 42;
    member_t m = core_member_init_data("score", NULL, &val, sizeof(int), PUBLIC, NULL, NULL);
    assert(*(int*)core_member_get_value(&m) == 42);
    core_member_destroy(&m);
}

void test_member_init_private(void)
{
    int val = 0;
    member_t m = core_member_init_data("hidden", NULL, &val, sizeof(int), PRIVATE, NULL, NULL);
    assert(*core_member_get_visibility(&m) == PRIVATE);
    core_member_destroy(&m);
}

// ── member: set_value ─────────────────────────────────────────────────────────

void test_member_set_value(void)
{
    int val = 10;
    member_t m = core_member_init_data("score", NULL, &val, sizeof(int), PUBLIC, NULL, NULL);
    int new_val = 99;
    assert(core_member_set_value(&m, &new_val) == 1);
    assert(*(int*)core_member_get_value(&m) == 99);
    core_member_destroy(&m);
}

void test_member_set_value_null_member(void)
{
    int val = 1;
    assert(core_member_set_value(NULL, &val) == 0);
}

void test_member_set_value_null_value(void)
{
    int val = 1;
    member_t m = core_member_init_data("score", NULL, &val, sizeof(int), PUBLIC, NULL, NULL);
    assert(core_member_set_value(&m, NULL) == 0);
    core_member_destroy(&m);
}

// ── member: getters ───────────────────────────────────────────────────────────

void test_member_get_name(void)
{
    int val = 0;
    member_t m = core_member_init_data("hp", NULL, &val, sizeof(int), PUBLIC, NULL, NULL);
    assert(strcmp(core_string_get_data(core_member_get_name(&m)), "hp") == 0);
    core_member_destroy(&m);
}

void test_member_get_holder(void)
{
    int val = 0;
    int holder = 0;
    member_t m = core_member_init_data("hp", &holder, &val, sizeof(int), PUBLIC, NULL, NULL);
    assert(core_member_get_holder(&m) == &holder);
    core_member_destroy(&m);
}

void test_member_get_null_guards(void)
{
    assert(core_member_get_name(NULL) == NULL);
    assert(core_member_get_value(NULL) == NULL);
    assert(core_member_get_holder(NULL) == NULL);
    assert(core_member_get_visibility(NULL) == NULL);
    assert(core_member_get_value_size(NULL) == 0);
}

// ── member: destroy ───────────────────────────────────────────────────────────

void test_member_destroy(void)
{
    int val = 5;
    member_t m = core_member_init_data("score", NULL, &val, sizeof(int), PUBLIC, NULL, NULL);
    core_member_destroy(&m);
    assert(m._Member_Value == NULL);
    assert(m._Member_Holder == NULL);
    assert(m._Member_Value_Size == 0);
}

void test_member_destroy_null(void)
{
    core_member_destroy(NULL); // must not crash
}

// ── method: init ──────────────────────────────────────────────────────────────

void test_method_init(void)
{
    method_t m = core_method_init("attack", dummy_method, PUBLIC);
    assert(core_method_get_name(&m) != NULL);
    assert(strcmp(core_string_get_data(core_method_get_name(&m)), "attack") == 0);
    assert(m._Method_Function == dummy_method);
    assert(*core_method_get_visibility(&m) == PUBLIC);
    core_method_destroy(&m);
}

void test_method_init_private(void)
{
    method_t m = core_method_init("internal", dummy_method, PRIVATE);
    assert(*core_method_get_visibility(&m) == PRIVATE);
    core_method_destroy(&m);
}

// ── method: getters ───────────────────────────────────────────────────────────

void test_method_get_null_guards(void)
{
    assert(core_method_get_name(NULL) == NULL);
    assert(core_method_get_visibility(NULL) == NULL);
}

// ── method: destroy ───────────────────────────────────────────────────────────

void test_method_destroy(void)
{
    method_t m = core_method_init("attack", dummy_method, PUBLIC);
    core_method_destroy(&m);
    assert(m._Method_Name._String_Data == NULL);
    assert(m._Method_Function == NULL);
}

void test_method_destroy_null(void)
{
    core_method_destroy(NULL); // must not crash
}

// ── object: init ──────────────────────────────────────────────────────────────

void test_object_init_no_members_no_methods(void)
{
    object_t obj = core_object_init(NULL, "hero", 0, NULL, 0, NULL);
    assert(strcmp(core_string_get_data(core_object_get_name(&obj)), "hero") == 0);
    assert(core_object_get_members_size(&obj) == 0);
    assert(core_object_get_methods_size(&obj) == 0);
    core_object_destroy(&obj);
}

void test_object_init_with_member(void)
{
    int val = 100;
    member_t members[] = { core_member_init_data("hp", NULL, &val, sizeof(int), PUBLIC, NULL, NULL) };
    object_t obj = core_object_init(NULL, "hero", 1, members, 0, NULL);
    assert(core_object_get_members_size(&obj) == 1);
    member_t* m = core_object_member(&obj, "hp");
    assert(m != NULL);
    assert(*(int*)core_member_get_value(m) == 100);
    core_member_destroy(&members[0]);
    core_object_destroy(&obj);
}

void test_object_init_with_method(void)
{
    method_t methods[] = { core_method_init("attack", dummy_method, PUBLIC) };
    object_t obj = core_object_init(NULL, "hero", 0, NULL, 1, methods);
    assert(core_object_get_methods_size(&obj) == 1);
    method_t* m = core_object_method(&obj, "attack");
    assert(m != NULL);
    core_method_destroy(&methods[0]);
    core_object_destroy(&obj);
}

void test_object_init_deep_copies_member(void)
{
    int val = 7;
    member_t members[] = { core_member_init_data("x", NULL, &val, sizeof(int), PUBLIC, NULL, NULL) };
    object_t obj = core_object_init(NULL, "obj", 1, members, 0, NULL);
    core_member_destroy(&members[0]);
    // object's copy must still be valid after source is destroyed
    member_t* m = core_object_member(&obj, "x");
    assert(m != NULL);
    assert(*(int*)core_member_get_value(m) == 7);
    core_object_destroy(&obj);
}

void test_object_init_deep_copies_method(void)
{
    method_t methods[] = { core_method_init("run", dummy_method, PUBLIC) };
    object_t obj = core_object_init(NULL, "obj", 0, NULL, 1, methods);
    core_method_destroy(&methods[0]);
    method_t* m = core_object_method(&obj, "run");
    assert(m != NULL);
    assert(m->_Method_Function == dummy_method);
    core_object_destroy(&obj);
}

// ── object: call ──────────────────────────────────────────────────────────────

void test_object_call(void)
{
    method_t methods[] = { core_method_init("fire", tracking_method, PUBLIC) };
    object_t obj = core_object_init(NULL, "obj", 0, NULL, 1, methods);
    method_called = 0;
    assert(core_object_call(&obj, "fire", 0, NULL) == 1);
    assert(method_called == 1);
    core_method_destroy(&methods[0]);
    core_object_destroy(&obj);
}

void test_object_call_not_found(void)
{
    object_t obj = core_object_init(NULL, "obj", 0, NULL, 0, NULL);
    assert(core_object_call(&obj, "missing", 0, NULL) == 0);
    core_object_destroy(&obj);
}

void test_object_call_null_guards(void)
{
    object_t obj = core_object_init(NULL, "obj", 0, NULL, 0, NULL);
    assert(core_object_call(NULL, "fire", 0, NULL) == 0);
    assert(core_object_call(&obj, NULL, 0, NULL) == 0);
    core_object_destroy(&obj);
}

// ── object: getters ───────────────────────────────────────────────────────────

void test_object_get_class(void)
{
    class_t cls = core_class_init("Entity", NULL, NULL, 0, NULL, 0, NULL, NULL);
    object_t obj = core_object_init(&cls, "obj", 0, NULL, 0, NULL);
    assert(core_object_get_class(&obj) == &cls);
    core_object_destroy(&obj);
    core_class_destroy(&cls);
}

void test_object_member_not_found(void)
{
    object_t obj = core_object_init(NULL, "obj", 0, NULL, 0, NULL);
    assert(core_object_member(&obj, "missing") == NULL);
    core_object_destroy(&obj);
}

void test_object_method_not_found(void)
{
    object_t obj = core_object_init(NULL, "obj", 0, NULL, 0, NULL);
    assert(core_object_method(&obj, "missing") == NULL);
    core_object_destroy(&obj);
}

void test_object_get_null_guards(void)
{
    assert(core_object_get_name(NULL) == NULL);
    assert(core_object_get_class(NULL) == NULL);
    assert(core_object_get_members(NULL) == NULL);
    assert(core_object_get_methods(NULL) == NULL);
    assert(core_object_get_members_size(NULL) == 0);
    assert(core_object_get_methods_size(NULL) == 0);
    assert(core_object_member(NULL, "x") == NULL);
    assert(core_object_method(NULL, "x") == NULL);
}

// ── object: destroy ───────────────────────────────────────────────────────────

void test_object_destroy(void)
{
    int val = 1;
    member_t members[] = { core_member_init_data("hp", NULL, &val, sizeof(int), PUBLIC, NULL, NULL) };
    method_t methods[] = { core_method_init("run", dummy_method, PUBLIC) };
    object_t obj = core_object_init(NULL, "hero", 1, members, 1, methods);
    core_member_destroy(&members[0]);
    core_method_destroy(&methods[0]);
    core_object_destroy(&obj);
    assert(obj._Object_Members == NULL);
    assert(obj._Object_Methods == NULL);
    assert(obj._Object_Members_Size == 0);
    assert(obj._Object_Methods_Size == 0);
    assert(obj._Object_Class == NULL);
}

void test_object_destroy_null(void)
{
    core_object_destroy(NULL); // must not crash
}

// ── class: init ───────────────────────────────────────────────────────────────

void test_class_init_empty(void)
{
    class_t cls = core_class_init("Player", NULL, NULL, 0, NULL, 0, NULL, NULL);
    assert(strcmp(core_string_get_data(core_class_get_name(&cls)), "Player") == 0);
    assert(core_class_get_base(&cls) == NULL);
    assert(core_class_get_static_members_size(&cls) == 0);
    assert(core_class_get_static_methods_size(&cls) == 0);
    core_class_destroy(&cls);
}

void test_class_init_with_static_member(void)
{
    int val = 10;
    member_t members[] = { core_member_init_data("count", NULL, &val, sizeof(int), PUBLIC, NULL, NULL) };
    class_t cls = core_class_init("Player", NULL, members, 1, NULL, 0, NULL, NULL);
    assert(core_class_get_static_members_size(&cls) == 1);
    member_t* m = core_class_static_member(&cls, "count");
    assert(m != NULL);
    assert(*(int*)core_member_get_value(m) == 10);
    core_member_destroy(&members[0]);
    core_class_destroy(&cls);
}

void test_class_init_with_static_method(void)
{
    method_t methods[] = { core_method_init("spawn", dummy_method, PUBLIC) };
    class_t cls = core_class_init("Player", NULL, NULL, 0, methods, 1, NULL, NULL);
    assert(core_class_get_static_methods_size(&cls) == 1);
    method_t* m = core_class_static_method(&cls, "spawn");
    assert(m != NULL);
    core_method_destroy(&methods[0]);
    core_class_destroy(&cls);
}

void test_class_init_deep_copies_static_member(void)
{
    int val = 5;
    member_t members[] = { core_member_init_data("count", NULL, &val, sizeof(int), PUBLIC, NULL, NULL) };
    class_t cls = core_class_init("Player", NULL, members, 1, NULL, 0, NULL, NULL);
    core_member_destroy(&members[0]);
    member_t* m = core_class_static_member(&cls, "count");
    assert(m != NULL);
    assert(*(int*)core_member_get_value(m) == 5);
    core_class_destroy(&cls);
}

void test_class_init_deep_copies_static_method(void)
{
    method_t methods[] = { core_method_init("spawn", dummy_method, PUBLIC) };
    class_t cls = core_class_init("Player", NULL, NULL, 0, methods, 1, NULL, NULL);
    core_method_destroy(&methods[0]);
    method_t* m = core_class_static_method(&cls, "spawn");
    assert(m != NULL);
    assert(m->_Method_Function == dummy_method);
    core_class_destroy(&cls);
}

// ── class: call ───────────────────────────────────────────────────────────────

void test_class_call(void)
{
    method_t methods[] = { core_method_init("spawn", tracking_method, PUBLIC) };
    class_t cls = core_class_init("Player", NULL, NULL, 0, methods, 1, NULL, NULL);
    method_called = 0;
    assert(core_class_call(&cls, "spawn", 0, NULL) == 1);
    assert(method_called == 1);
    core_method_destroy(&methods[0]);
    core_class_destroy(&cls);
}

void test_class_call_not_found(void)
{
    class_t cls = core_class_init("Player", NULL, NULL, 0, NULL, 0, NULL, NULL);
    assert(core_class_call(&cls, "missing", 0, NULL) == 0);
    core_class_destroy(&cls);
}

void test_class_call_inherits_base_method(void)
{
    method_t base_methods[] = { core_method_init("base_action", base_tracking_method, PUBLIC) };
    class_t base = core_class_init("Entity", NULL, NULL, 0, base_methods, 1, NULL, NULL);
    class_t derived = core_class_init("Player", &base, NULL, 0, NULL, 0, NULL, NULL);
    base_method_called = 0;
    assert(core_class_call(&derived, "base_action", 0, NULL) == 1);
    assert(base_method_called == 1);
    core_method_destroy(&base_methods[0]);
    core_class_destroy(&derived);
    core_class_destroy(&base);
}

void test_class_call_own_method_takes_priority(void)
{
    method_t base_methods[]    = { core_method_init("action", base_tracking_method, PUBLIC) };
    method_t derived_methods[] = { core_method_init("action", tracking_method,      PUBLIC) };
    class_t base    = core_class_init("Entity", NULL,  NULL, 0, base_methods,    1, NULL, NULL);
    class_t derived = core_class_init("Player", &base, NULL, 0, derived_methods, 1, NULL, NULL);
    method_called = 0;
    base_method_called = 0;
    assert(core_class_call(&derived, "action", 0, NULL) == 1);
    assert(method_called == 1);
    assert(base_method_called == 0);
    core_method_destroy(&base_methods[0]);
    core_method_destroy(&derived_methods[0]);
    core_class_destroy(&derived);
    core_class_destroy(&base);
}

void test_class_call_null_guards(void)
{
    class_t cls = core_class_init("Player", NULL, NULL, 0, NULL, 0, NULL, NULL);
    assert(core_class_call(NULL, "spawn", 0, NULL) == 0);
    assert(core_class_call(&cls, NULL, 0, NULL) == 0);
    core_class_destroy(&cls);
}

// ── class: getters ────────────────────────────────────────────────────────────

void test_class_static_member_not_found(void)
{
    class_t cls = core_class_init("Player", NULL, NULL, 0, NULL, 0, NULL, NULL);
    assert(core_class_static_member(&cls, "missing") == NULL);
    core_class_destroy(&cls);
}

void test_class_static_method_not_found(void)
{
    class_t cls = core_class_init("Player", NULL, NULL, 0, NULL, 0, NULL, NULL);
    assert(core_class_static_method(&cls, "missing") == NULL);
    core_class_destroy(&cls);
}

void test_class_get_null_guards(void)
{
    assert(core_class_get_name(NULL) == NULL);
    assert(core_class_get_base(NULL) == NULL);
    assert(core_class_get_static_members(NULL) == NULL);
    assert(core_class_get_static_methods(NULL) == NULL);
    assert(core_class_get_static_members_size(NULL) == 0);
    assert(core_class_get_static_methods_size(NULL) == 0);
    assert(core_class_static_member(NULL, "x") == NULL);
    assert(core_class_static_method(NULL, "x") == NULL);
}

// ── class: destroy ────────────────────────────────────────────────────────────

void test_class_destroy(void)
{
    int val = 1;
    member_t members[] = { core_member_init_data("count", NULL, &val, sizeof(int), PUBLIC, NULL, NULL) };
    method_t methods[] = { core_method_init("spawn", dummy_method, PUBLIC) };
    class_t cls = core_class_init("Player", NULL, members, 1, methods, 1, NULL, NULL);
    core_member_destroy(&members[0]);
    core_method_destroy(&methods[0]);
    core_class_destroy(&cls);
    assert(cls._Class_Static_Members == NULL);
    assert(cls._Class_Static_Methods == NULL);
    assert(cls._Class_Static_Members_Size == 0);
    assert(cls._Class_Static_Methods_Size == 0);
    assert(cls._Class_Base_Class == NULL);
    assert(cls._Class_Name._String_Data == NULL);
}

void test_class_destroy_null(void)
{
    core_class_destroy(NULL); // must not crash
}

// ── main ──────────────────────────────────────────────────────────────────────

int main(void)
{
    // member
    test_member_init();
    test_member_init_data();
    test_member_init_private();
    test_member_set_value();
    test_member_set_value_null_member();
    test_member_set_value_null_value();
    test_member_get_name();
    test_member_get_holder();
    test_member_get_null_guards();
    test_member_destroy();
    test_member_destroy_null();

    // method
    test_method_init();
    test_method_init_private();
    test_method_get_null_guards();
    test_method_destroy();
    test_method_destroy_null();

    // object
    test_object_init_no_members_no_methods();
    test_object_init_with_member();
    test_object_init_with_method();
    test_object_init_deep_copies_member();
    test_object_init_deep_copies_method();
    test_object_call();
    test_object_call_not_found();
    test_object_call_null_guards();
    test_object_get_class();
    test_object_member_not_found();
    test_object_method_not_found();
    test_object_get_null_guards();
    test_object_destroy();
    test_object_destroy_null();

    // class
    test_class_init_empty();
    test_class_init_with_static_member();
    test_class_init_with_static_method();
    test_class_init_deep_copies_static_member();
    test_class_init_deep_copies_static_method();
    test_class_call();
    test_class_call_not_found();
    test_class_call_inherits_base_method();
    test_class_call_own_method_takes_priority();
    test_class_call_null_guards();
    test_class_static_member_not_found();
    test_class_static_method_not_found();
    test_class_get_null_guards();
    test_class_destroy();
    test_class_destroy_null();

    printf("All core_oop tests passed.\n");
    return 0;
}