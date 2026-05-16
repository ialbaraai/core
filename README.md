# core
A lightweight, generic data structures library written in pure C. Supports custom copy and destroy functions for managing heap-owning types.
## Structures
- `string_t` — dynamic string with a full character and string manipulation API
- `vector_t` — generic dynamic array with optional deep copy and destroy hooks
- `map_t` — generic key-value store with type-erased keys and values
- `file_t` — file reader and writer built on `string_t` and `vector_t`
## Building
```bash
cmake -S . -B build
cmake --build build
```
This produces `libcore.a` which you can link against your project.
## Usage
### string_t
```c
string_t s = core_string_init(64);
string_t s2 = core_string_init_data(64, "Hello, World!");
core_string_append_cstr(&s, " How are you?");
core_string_upper(&s2);
printf("%s\n", core_string_get_data(&s));
core_string_destroy(&s);
core_string_destroy(&s2);
```
### vector_t
```c
// primitive types — no copy/destroy needed
vector_t v = core_vector_init(8, sizeof(int), NULL, NULL);
int x = 42;
core_vector_push_back(&v, &x);
int* val = (int*)core_vector_get(&v, 0);
core_vector_destroy(&v);
```
```c
// heap-owning types — provide copy and destroy hooks
vector_t v = core_vector_init(8, sizeof(string_t), copy_str, destroy_str);
string_t s = core_string_init_data(32, "hello");
core_vector_push_back(&v, &s);
core_string_destroy(&s); // safe — vector holds its own deep copy
core_vector_destroy(&v);
```
### map_t
```c
// primitive types
map_t m = core_map_init(8, sizeof(int), sizeof(int),
                        NULL, NULL, NULL, NULL, compare_int);
int k = 1, v = 100;
core_map_push_back(&m, &k, &v);
int* result = (int*)core_map_get_value(&m, &k);
core_map_destroy(&m);
```
```c
// heap-owning types
map_t m = core_map_init(8, sizeof(string_t), sizeof(string_t),
                        copy_str, destroy_str,
                        copy_str, destroy_str,
                        compare_str);
string_t key = core_string_init_data(32, "name");
string_t val = core_string_init_data(32, "Albaraa");
core_map_push_back(&m, &key, &val);
core_string_destroy(&key);
core_string_destroy(&val);
core_map_destroy(&m);
```
### file_t
```c
// read a file
file_t f = core_file_init("README.md", 128, 128);
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
file_t f = core_file_init("notes.txt", 64, 128);
core_file_read(&f);

string_t* line = (string_t*)core_vector_get(core_file_get_lines(&f), 0);
core_string_set(line, "updated first line");
core_file_write(&f);

core_file_destroy(&f);
```
```c
// reuse the same struct for a different file
file_t f = core_file_init("input.txt", 64, 128);
core_file_read(&f);

core_string_set(core_file_get_filepath(&f), "output.txt");
core_file_write(&f);

core_file_destroy(&f);
```
## License
MIT © albaraa ialbaraai
