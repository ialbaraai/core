# core

A lightweight, generic data structures and OOP library written in pure C. Supports custom copy and destroy functions for managing heap-owning types.

## Structures

- `string_t` — dynamic string with a full character and string manipulation API
- `vector_t` — generic dynamic array with optional deep copy and destroy hooks
- `map_t` — generic key-value store with type-erased keys and values
- `file_t` — file reader and writer built on `string_t` and `vector_t`
- `class_t` & `object_t` — basic object-oriented programming in C

## Building

```bash
cmake -S . -B build
cmake --build build
```

This produces `libcore.a` which you can link against your project.

## Usage

### string_t

```c
string_t s = {0}
core_string_init(&s, 64);
string_t s2 = {0};
core_string_init_data(&s2, 64, "Hello, World!");
core_string_append_cstr(&s, " How are you?");
core_string_upper(&s2);
printf("%s\n", core_string_get_data(&s));
core_string_destroy(&s);
core_string_destroy(&s2);
```

```bash
# Expected output:
 How are you?
```

### vector_t

```c
// primitive types — no copy/destroy needed
vector_t v = {0};
core_vector_init(&v, 8, sizeof(int), NULL, NULL);
int x = 42;
core_vector_push_back(&v, &x); // copies value of `x`
int* val = (int*)core_vector_get(&v, 0);
core_vector_destroy(&v);
```

```c
// heap-owning types — provide copy and destroy hooks
vector_t v = {0};
core_vector_init(&v, 8, sizeof(string_t), core_string_copy_callback /*built-in core string copy callback*/, core_string_destroy_callback /*built-in core string destroy callback*/);
string_t s = {0};
core_string_init_data(&s, 32, "hello");
core_vector_push_back(&v, &s);
core_string_destroy(&s); // safe — vector holds its own deep copy
core_vector_destroy(&v);
```

### map_t

```c
// primitive types
map_t m = {0};
core_map_init(&m, 8, sizeof(int), sizeof(int),
                        NULL, NULL, NULL, NULL, compare_int);
int k = 1, v = 100;
core_map_push_back(&m, &k, &v);
int* result = (int*)core_map_get_value(&m, &k);
core_map_destroy(&m);
```

```c
// heap-owning types
map_t m = {0};
core_map_init(&m, 8, sizeof(string_t), sizeof(string_t),
                        core_string_copy_callback, core_string_destroy_callback,
                        core_string_copy_callback, core_string_destroy_callback,
                        core_string_compare_callback /*built-in core string compare callback*/);
string_t key = {0};
core_string_init_data(&key, 32, "name");
string_t val = {0};
core_string_init_data(&val, 32, "Albaraa");
core_map_push_back(&m, &key, &val);
core_string_destroy(&key);
core_string_destroy(&val);
core_map_destroy(&m);
```

### file_t

```c
// read a file
file_t f = {0};
core_file_init(&f, "README.md", 128, 128);
core_file_read(&f);

// iterate lines
for (size_t i = 0; i < core_vector_get_size(core_file_get_lines(&f)); ++i)
{
    string_t* line = (string_t*)core_vector_get(core_file_get_lines(&f), i);
    printf("%s\n", core_string_get_data(line));
}

core_file_destroy(&f);
```

```c
// mutate a line and write back
file_t f = {0};
core_file_init(&f, "notes.txt", 64, 128);
core_file_read(&f);

string_t* line = (string_t*)core_vector_get(core_file_get_lines(&f), 0);
core_string_set(line, "updated first line");
core_file_write(&f);

core_file_destroy(&f);
```

```c
// reuse the same struct for a different file
file_t f = {0};
core_file_init(&f, "input.txt", 64, 128);
core_file_read(&f);

core_string_set(core_file_get_filepath(&f), "output.txt");
core_file_write(&f);

core_file_destroy(&f);
```

### OOP (Basic)

```c
// 1. Define a method function
void speak(object_t* this, int argc, void** argv) {
    printf("%s says: Woof!\n", core_string_get_data(core_object_get_name(this)));
}

// 2. Create a method pointing to your method function
method_t m = core_method_init("Speak", speak, PUBLIC);

// 3. Create a Class
// class_t Class_Name = core_class_init(Class name, Base class pointer, Number of static members, Pointer to static members, Number of static methods, Pointer to static methods, Constructor function pointer, Destructor function pointer);
class_t c = core_class_init("Dog", NULL, 0, NULL, 0, NULL, NULL, NULL); // Initialize a class with STATIC members & methods

// 4. Create an Object (Instance)
// object_t Object_Name = core_object_init(Class pointer, Object name, Number of instance members, Pointer to instance members, Number of instance methods, Pointer to instance methods); (If no class constructor method)
// Object_t Object_Name = Class.constructor(Number of arguments, Constructor arguments); (If `Class` has constructor method, must call `Class.destructor(&Object_Name)` for destructor)
object_t o = core_object_init(&c, "Fido", 0, NULL, 1, &m); // Initialize an object with INSTANCE members & methods

// 5. Call Method
core_object_call(&o, "Speak", 0, NULL);

// 6. Cleanup
core_object_destroy(&o);
core_class_destroy(&c);
core_method_destroy(&m);
```

```bash
# Expected output:
Fido says: Woof!
```

---

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.

---

## Author

**albaraa** — [@ialbaraai](https://github.com/ialbaraai)
