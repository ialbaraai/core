#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/oop/core_oop.h"

// ── helpers ───────────────────────────────────────────────────────────────────

static void dummy_method(object_t* this, int argc, void** argv)
{
    (void)this;
    (void)argc;
    (void)argv;
}

static int method_called = 0;

static void tracking_method(object_t* this, int argc, void** argv)
{
    (void)this;
    (void)argc;
    (void)argv;

    method_called = 1;
}

static int base_method_called = 0;

static void base_tracking_method(object_t* this, int argc, void** argv)
{
    (void)this;
    (void)argc;
    (void)argv;

    base_method_called = 1;
}

// ── member: init ──────────────────────────────────────────────────────────────

void test_member_init(void)
{
    member_t member = {0};

    assert(
        core_member_init(
            &member,
            "score",
            NULL,
            sizeof(int),
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(core_member_get_name(&member) != NULL);
    assert(strcmp(core_string_get_data(core_member_get_name(&member)), "score") == 0);
    assert(core_member_get_value_size(&member) == sizeof(int));
    assert(core_member_get_value(&member) != NULL);
    assert(*core_member_get_visibility(&member) == PUBLIC);

    core_member_destroy(&member);
}

void test_member_init_data(void)
{
    int value = 42;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "score",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(*(int*)core_member_get_value(&member) == 42);

    core_member_destroy(&member);
}

void test_member_init_private(void)
{
    int value = 0;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "hidden",
            NULL,
            sizeof(int),
            &value,
            PRIVATE,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(*core_member_get_visibility(&member) == PRIVATE);

    core_member_destroy(&member);
}

// ── member: set_value ─────────────────────────────────────────────────────────

void test_member_set_value(void)
{
    int value = 10;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "score",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    int new_value = 99;

    assert(core_member_set_value(&member, &new_value) == SUCCESS);
    assert(*(int*)core_member_get_value(&member) == 99);

    core_member_destroy(&member);
}

void test_member_set_value_null_member(void)
{
    int value = 1;

    assert(core_member_set_value(NULL, &value) == MEMBER_POINTER_NULL_ERROR);
}

void test_member_set_value_null_value(void)
{
    int value = 1;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "score",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(core_member_set_value(&member, NULL) == MEMBER_NEW_VALUE_POINTER_NULL_ERROR);

    core_member_destroy(&member);
}

// ── member: getters ───────────────────────────────────────────────────────────

void test_member_get_name(void)
{
    int value = 0;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "hp",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(strcmp(core_string_get_data(core_member_get_name(&member)), "hp") == 0);

    core_member_destroy(&member);
}

void test_member_get_holder(void)
{
    int value = 0;
    int holder = 0;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "hp",
            &holder,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(core_member_get_holder(&member) == &holder);

    core_member_destroy(&member);
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
    int value = 5;

    member_t member = {0};

    assert(
        core_member_init_data(
            &member,
            "score",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    core_member_destroy(&member);

    assert(member._Member_Value == NULL);
    assert(member._Member_Holder == NULL);
    assert(member._Member_Value_Size == 0);
}

void test_member_destroy_null(void)
{
    core_member_destroy(NULL);
}

// ── method: init ──────────────────────────────────────────────────────────────

void test_method_init(void)
{
    method_t method = {0};

    assert(
        core_method_init(
            &method,
            "attack",
            dummy_method,
            PUBLIC
        ) == SUCCESS
    );

    assert(core_method_get_name(&method) != NULL);
    assert(strcmp(core_string_get_data(core_method_get_name(&method)), "attack") == 0);
    assert(method._Method_Function == dummy_method);
    assert(*core_method_get_visibility(&method) == PUBLIC);

    core_method_destroy(&method);
}

void test_method_init_private(void)
{
    method_t method = {0};

    assert(
        core_method_init(
            &method,
            "internal",
            dummy_method,
            PRIVATE
        ) == SUCCESS
    );

    assert(*core_method_get_visibility(&method) == PRIVATE);

    core_method_destroy(&method);
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
    method_t method = {0};

    assert(
        core_method_init(
            &method,
            "attack",
            dummy_method,
            PUBLIC
        ) == SUCCESS
    );

    core_method_destroy(&method);

    assert(method._Method_Name._String_Data == NULL);
    assert(method._Method_Function == NULL);
}

void test_method_destroy_null(void)
{
    core_method_destroy(NULL);
}

// ── object: init ──────────────────────────────────────────────────────────────

void test_object_init_no_members_no_methods(void)
{
    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "hero",
            0,
            NULL,
            0,
            NULL
        ) == SUCCESS
    );

    assert(strcmp(core_string_get_data(core_object_get_name(&object)), "hero") == 0);
    assert(core_object_get_members_size(&object) == 0);
    assert(core_object_get_methods_size(&object) == 0);

    core_object_destroy(&object);
}

void test_object_init_with_member(void)
{
    int value = 100;

    member_t members[1] = {0};

    assert(
        core_member_init_data(
            &members[0],
            "hp",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "hero",
            1,
            members,
            0,
            NULL
        ) == SUCCESS
    );

    assert(core_object_get_members_size(&object) == 1);

    member_t* member = core_object_member(&object, "hp");

    assert(member != NULL);
    assert(*(int*)core_member_get_value(member) == 100);

    core_member_destroy(&members[0]);
    core_object_destroy(&object);
}

void test_object_init_with_method(void)
{
    method_t methods[1] = {0};

    assert(
        core_method_init(
            &methods[0],
            "attack",
            dummy_method,
            PUBLIC
        ) == SUCCESS
    );

    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "hero",
            0,
            NULL,
            1,
            methods
        ) == SUCCESS
    );

    assert(core_object_get_methods_size(&object) == 1);

    method_t* method = core_object_method(&object, "attack");

    assert(method != NULL);

    core_method_destroy(&methods[0]);
    core_object_destroy(&object);
}

void test_object_init_deep_copies_member(void)
{
    int value = 7;

    member_t members[1] = {0};

    assert(
        core_member_init_data(
            &members[0],
            "x",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "obj",
            1,
            members,
            0,
            NULL
        ) == SUCCESS
    );

    core_member_destroy(&members[0]);

    member_t* member = core_object_member(&object, "x");

    assert(member != NULL);
    assert(*(int*)core_member_get_value(member) == 7);

    core_object_destroy(&object);
}

void test_object_init_deep_copies_method(void)
{
    method_t methods[1] = {0};

    assert(
        core_method_init(
            &methods[0],
            "run",
            dummy_method,
            PUBLIC
        ) == SUCCESS
    );

    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "obj",
            0,
            NULL,
            1,
            methods
        ) == SUCCESS
    );

    core_method_destroy(&methods[0]);

    method_t* method = core_object_method(&object, "run");

    assert(method != NULL);
    assert(method->_Method_Function == dummy_method);

    core_object_destroy(&object);
}

// ── object: call ──────────────────────────────────────────────────────────────

void test_object_call(void)
{
    method_t methods[1] = {0};

    assert(
        core_method_init(
            &methods[0],
            "fire",
            tracking_method,
            PUBLIC
        ) == SUCCESS
    );

    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "obj",
            0,
            NULL,
            1,
            methods
        ) == SUCCESS
    );

    method_called = 0;

    assert(core_object_call(&object, "fire", 0, NULL) == SUCCESS);
    assert(method_called == 1);

    core_method_destroy(&methods[0]);
    core_object_destroy(&object);
}

void test_object_call_not_found(void)
{
    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "obj",
            0,
            NULL,
            0,
            NULL
        ) == SUCCESS
    );

    assert(core_object_call(&object, "missing", 0, NULL) == 0);

    core_object_destroy(&object);
}

void test_object_call_null_guards(void)
{
    object_t object = {0};

    assert(
        core_object_init(
            &object,
            NULL,
            "obj",
            0,
            NULL,
            0,
            NULL
        ) == SUCCESS
    );

    assert(core_object_call(NULL, "fire", 0, NULL) == 0);
    assert(core_object_call(&object, NULL, 0, NULL) == 0);

    core_object_destroy(&object);
}

// ── class: init ───────────────────────────────────────────────────────────────

void test_class_init_empty(void)
{
    class_t class_instance = {0};

    assert(
        core_class_init(
            &class_instance,
            "Player",
            NULL,
            0,
            NULL,
            0,
            NULL,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(strcmp(core_string_get_data(core_class_get_name(&class_instance)), "Player") == 0);
    assert(core_class_get_base(&class_instance) == NULL);
    assert(core_class_get_static_members_size(&class_instance) == 0);
    assert(core_class_get_static_methods_size(&class_instance) == 0);

    core_class_destroy(&class_instance);
}

void test_class_init_with_static_member(void)
{
    int value = 10;

    member_t members[1] = {0};

    assert(
        core_member_init_data(
            &members[0],
            "count",
            NULL,
            sizeof(int),
            &value,
            PUBLIC,
            NULL,
            NULL
        ) == SUCCESS
    );

    class_t class_instance = {0};

    assert(
        core_class_init(
            &class_instance,
            "Player",
            NULL,
            1,
            members,
            0,
            NULL,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(core_class_get_static_members_size(&class_instance) == 1);

    member_t* member = core_class_static_member(&class_instance, "count");

    assert(member != NULL);
    assert(*(int*)core_member_get_value(member) == 10);

    core_member_destroy(&members[0]);
    core_class_destroy(&class_instance);
}

void test_class_init_with_static_method(void)
{
    method_t methods[1] = {0};

    assert(
        core_method_init(
            &methods[0],
            "spawn",
            dummy_method,
            PUBLIC
        ) == SUCCESS
    );

    class_t class_instance = {0};

    assert(
        core_class_init(
            &class_instance,
            "Player",
            NULL,
            0,
            NULL,
            1,
            methods,
            NULL,
            NULL
        ) == SUCCESS
    );

    assert(core_class_get_static_methods_size(&class_instance) == 1);

    method_t* method = core_class_static_method(&class_instance, "spawn");

    assert(method != NULL);

    core_method_destroy(&methods[0]);
    core_class_destroy(&class_instance);
}

// ── class: call ───────────────────────────────────────────────────────────────

void test_class_call(void)
{
    method_t methods[1] = {0};

    assert(
        core_method_init(
            &methods[0],
            "spawn",
            tracking_method,
            PUBLIC
        ) == SUCCESS
    );

    class_t class_instance = {0};

    assert(
        core_class_init(
            &class_instance,
            "Player",
            NULL,
            0,
            NULL,
            1,
            methods,
            NULL,
            NULL
        ) == SUCCESS
    );

    method_called = 0;

    assert(core_class_call(&class_instance, "spawn", 0, NULL) == SUCCESS);
    assert(method_called == 1);

    core_method_destroy(&methods[0]);
    core_class_destroy(&class_instance);
}

void test_class_call_inherits_base_method(void)
{
    method_t base_methods[1] = {0};

    assert(
        core_method_init(
            &base_methods[0],
            "base_action",
            base_tracking_method,
            PUBLIC
        ) == SUCCESS
    );

    class_t base_class = {0};

    assert(
        core_class_init(
            &base_class,
            "Entity",
            NULL,
            0,
            NULL,
            1,
            base_methods,
            NULL,
            NULL
        ) == SUCCESS
    );

    class_t derived_class  ={0};

    assert(
        core_class_init(
            &derived_class,
            "Player",
            &base_class,
            0,
            NULL,
            0,
            NULL,
            NULL,
            NULL
        ) == SUCCESS
    );

    base_method_called = 0;

    assert(core_class_call(&derived_class, "base_action", 0, NULL) == SUCCESS);
    assert(base_method_called == 1);

    core_method_destroy(&base_methods[0]);
    core_class_destroy(&derived_class);
    core_class_destroy(&base_class);
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

    // class
    test_class_init_empty();
    test_class_init_with_static_member();
    test_class_init_with_static_method();
    test_class_call();
    test_class_call_inherits_base_method();

    printf("All core_oop tests passed.\n");

    return 0;
}