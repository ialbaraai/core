#ifndef CORE_H

#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// BOOL

#define bool _Bool
#define true 1
#define false 0

// STRING

typedef struct string
{
    char* data;
    size_t length;
    size_t capacity;
} string;

string string_init(size_t);
string string_initd(size_t, const char*);

int string_copy(string*, string*);

int string_compare(const string*, const string*);

void string_appendc(string*, const char);
void string_appendcstr(string*, const char*);
void string_appendstr(string*, const string*);
void string_pop(string*);
void string_input(string*);

int string_find(string*, const char, size_t*);
char string_at(string*, size_t);

size_t string_length(string*);
const char* string_get(string*);
char* string_getf(string*);
void string_set(string*, const char*);

void string_replacec(string*, const char, size_t);
void string_replace(string*, const char, const char);
void string_lower(string*);
void string_upper(string*);

void destroy_string(string*);

// GENERIC ARRAY

typedef struct array
{
    void* data;
    size_t length;
    size_t capacity;
    size_t element_size;
} array;

array array_init(size_t, size_t);
array array_initd(size_t, size_t, void*);

void ainsert(array*, void*);
void aremove(array*, size_t);
int afind(array*, void*, size_t*);
void* aget(array*, size_t);
void* afirst(array*);
void* alast(array*);

size_t alength(array*);
size_t acapacity(array*);
size_t aelement_size(array*);

void apop(array*);
void aset(array*, size_t, void*);

void aforeach(array*, void(size_t, void*));
void destroy_array(array*);

// GENERIC MAP

typedef struct map
{
    void* keys;
    void* values;
    size_t length;
    size_t capacity;
    size_t key_size;
    size_t value_size;
} map;

map map_init(size_t, size_t, size_t);
map map_initd(size_t, size_t, size_t, void*, void*);

void minsert(map*, void*, void*);
void mremove(map*, void*);
int mfindv(map*, void*, void*);
void* mgetv(map*, void*);

void* mfirstk(map*);
void* mlastk(map*);
void* mfirstv(map*);
void* mlastv(map*);

size_t mlength(map*);
size_t mcapacity(map*);
size_t mkey_size(map*);
size_t mvalue_size(map*);

void mpop(map*);
void mset(map*, void*, void*);

void mforeach(map*, void(void*, void*));
void destroy_map(map*);

#endif
