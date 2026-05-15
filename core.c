#include "core.h"

// DYNAMIC STRING \\

string string_init(size_t size)
{
    string str = {.data = malloc(size * sizeof(char)),
                  .capacity = size > 0 ? size : 1,
                  .length = 0};

    str.data[0] = '\0';

    return str;
}
string string_initd(size_t size, const char* cstr)
{
    string str = string_init(size);
    string_set(&str, cstr);

    return str;
}

int string_copy(string* destination, string* source)
{
    if (destination->capacity >= source->length)
    {
        strcpy(destination->data, source->data);
        destination->length = source->length;

        return 0;
    }
    else
    {
        size_t new_size = destination->capacity * 2 > strlen(source->data) ? destination->capacity * 2 : strlen(source->data) * 2;

        char* temp = realloc(destination->data, new_size * sizeof(char));

        if (temp == NULL)
        {
            printf("Failed to copy string!\n");

            return -1;
        }

        destination->data = temp;

        strcpy(destination->data, source->data);
        destination->length = source->length;

        return 0;
    }
}

int string_compare(const string* first, const string* second)
{
    if (first->length == second->length)
    {
        size_t length = first->length;

        for (size_t i = 0; i < length; i++)
        {
            if (*(first->data+i) != *(second->data+i))
            {
                return 0;
            }
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

void string_appendc(string* str, const char character)
{
    if (str->length + 1 > str->capacity)
    {
        size_t new_size = str->capacity * 2 > 0 ? str->capacity * 2 : 1;
        char* temp = realloc(str->data, new_size);

        if (temp == NULL)
        {
            printf("Failed to append to string!\n");
            return;
        }

        str->data = temp;
        *(str->data + str->length) = character;
        *(str->data + str->length+1) = '\0';
        str->length = strlen(str->data);
        str->capacity = new_size;
    }
    else
    {
        *(str->data + str->length) = character;
        str->length++;
        *(str->data + str->length) = '\0';
    }
}
void string_appendcstr(string* str, const char* new)
{
    if (str->capacity < str->length + strlen(new))
    {
        char* temp = realloc(str->data, sizeof(char) * (str->length + strlen(new)) * 2);

        if (temp == NULL)
        {
            printf("Failed to append string!\n");
            return;
        }

        str->data = temp;
        str->capacity = (str->length + strlen(new)) * 2;
    }

    for (size_t i = 0; i < strlen(new); i++)
    {
        if (*(new+i) == '\0')
        {
            break;
        }

        *(str->data + (str->length) + i) = *(new + i);
    }

    *(str->data + str->length + strlen(new)) = '\0';

    str->length = strlen(str->data);
}
void string_appendstr(string* str, const string* new)
{
    if (str->capacity < str->length + new->length)
    {
        char* temp = realloc(str->data, sizeof(char) * (str->length + new->length) * 2);

        if (temp == NULL)
        {
            printf("Failed to append string!\n");
            return;
        }

        str->data = temp;
        str->capacity = (str->length + new->length) * 2;
    }

    for (size_t i = 0; i < new->length; i++)
    {
        if (*(new->data+i) == '\0')
        {
            break;
        }

        *(str->data + (str->length) + i) = *(new->data + i);
    }

    *(str->data + str->length + new->length) = '\0';

    str->length = strlen(str->data);
}
void string_pop(string* str)
{
    if (str->length > 0)
    {
        str->length--;
        str->data[str->length] = '\0';
    }
}
void string_input(string* str)
{
    string_set(str, "");

    char c = getchar();

    while (c != '\n')
    {
        string_appendc(str, c);
        c = getchar();
    }
}

int string_find(string* str, const char character, size_t* index)
{
    for(size_t i = 0; i < str->length; i++)
    {
        if (*(str->data+i) == character)
        {
            *index = i;
            return 1;
        }
    }

    return 0;
}
char string_at(string* str, size_t index)
{
    if (index >= 0 && index < str->length)
    {
        return *(str->data+index);
    }

    return '\0';
}

size_t string_length(string* str)
{
    return str->length;
}
const char* string_get(string* str)
{
    return str->data;
}
char* string_getf(string* str)
{
    return str->data;
}
void string_set(string* str, const char* new)
{
    if (strlen(new) > str->capacity)
    {
        size_t new_size = str->capacity * 2 > strlen(new) ? str->capacity * 2 : strlen(new) * 2;
        char* temp = realloc(str->data, new_size);

        if (temp == NULL)
        {
            printf("Failed to set string!\n");

            return;
        }

        str->data = temp;
        str->capacity = new_size;
    }
    
    strcpy(str->data, new);
    str->length = strlen(new);
}

void string_replacec(string* str, const char character, size_t index)
{
    if (index >= 0 && index < str->length)
    {
        *(str->data+index) = character;
    }
}
void string_replace(string* str, const char tchar, const char nchar)
{
    for (size_t i = 0; i < str->length; i++)
    {
        if (*(str->data+i) == tchar)
        {
            *(str->data+i) = nchar;
        }
    }
}
void string_lower(string* str)
{
    for (size_t i = 0; i < str->length; i++)
    {
        *(str->data+i) = tolower((unsigned char)str->data[i]);
    }
}
void string_upper(string* str)
{
    for (size_t i = 0; i < str->length; i++)
    {
        *(str->data+i) = toupper((unsigned char)str->data[i]);
    }
}

void destroy_string(string* str)
{
    if (str == NULL) return;

    if (str->data != NULL)
    {
        free(str->data);
    }

    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

// DYNAMIC ARRAY \\

array array_init(size_t length, size_t size)
{
    array arr = {.data = malloc(length * size),
        .capacity = length,
        .length = 0,
        .element_size = size};

    return arr;
}
array array_initd(size_t length, size_t size, void* data)
{
    array arr = array_init(length, size);

    if (length > 0 && data != NULL)
    {
        memcpy(arr.data, data, size * length);
        arr.length = length;
    }

    return arr;
}
void ainsert(array* arr, void* value)
{
    if (arr->capacity < arr->length + 1)
    {
        size_t new_size = (arr->capacity == 0) ? 1 : arr->capacity * 2;
        void* temp = realloc(arr->data, new_size * arr->element_size);

        if (temp == NULL)
        {
            printf("Failed to insert to array!\n");

            return;
        }

        arr->data = temp;
        arr->capacity = new_size;
    }

    memcpy(((char*)(arr->data) + (arr->length * arr->element_size)), value, arr->element_size);
    arr->length++;
}
void aremove(array* arr, size_t index)
{
    if (index < 0 || index >= alength(arr))
    {
        return;
    }

    void* temp = malloc(arr->element_size);

    memcpy(temp, (void*)((char*)arr->data + (index * arr->element_size)), aelement_size(arr));
    memcpy((void*)((char*)arr->data + (index * arr->element_size)), (void*)((char*)arr->data + ((alength(arr) - 1) * arr->element_size)), aelement_size(arr));
    memcpy((void*)((char*)arr->data + ((alength(arr) - 1) * arr->element_size)), temp, aelement_size(arr));
    
    arr->length--;
    free(temp);
}
int afind(array* arr, void* value, size_t* target)
{
    for (size_t i = 0; i < arr->length; i++)
    {
        if (memcmp((void*)((char*)arr->data + (arr->element_size * i)), value, arr->element_size) == 0)
        {
            if (target != NULL)
                *target = i;

            return 1;
        }
    }

    return 0;
}
void* aget(array* arr, size_t index)
{
    if (index >= 0 && index < arr->length)
    {
        return (void*)((char*)arr->data + (index * arr->element_size));
    }
    else
    {
        return NULL;
    }
}
void* afirst(array* arr)
{
    return arr->data;
}
void* alast(array* arr)
{
    if (arr->length == 0) return NULL;

    return (void*)((char*)arr->data + ((arr->length - 1) * arr->element_size));
}
size_t alength(array* arr)
{
    return arr->length;
}
size_t acapacity(array* arr)
{
    return arr->capacity;
}
size_t aelement_size(array* arr)
{
    return arr->element_size;
}
void apop(array* arr)
{
    if (arr->length > 0)
    {
        arr->length--;
    }
}
void aset(array* arr, size_t index, void* value)
{
    if (index >= 0 && index < arr->length)
    {
        memcpy((void*)((char*)arr->data + (index * arr->element_size)), value, arr->element_size);
    }
}
void aforeach(array* arr, void (*function)(size_t, void*))
{
    for (size_t i = 0; i < arr->length; i++)
    {
        function(i, (void*)((char*)arr->data + (arr->element_size * i)));
    }
}
void destroy_array(array* arr)
{
    if (arr == NULL) return;

    if (arr->data != NULL)
    {
        free(arr->data);
    }

    arr->data = NULL;
    arr->length = 0;
    arr->element_size = 0;
    arr->capacity = 0;
}

// DYNAMIC MAP \\

map map_init(size_t length, size_t key_size, size_t value_size)
{
    map m = {.keys = malloc(key_size * length),
        .values = malloc(value_size * length),
        .capacity = length,
        .length = 0,
        .key_size = key_size,
        .value_size = value_size};

    return m;
}
map map_initd(size_t length, size_t key_size, size_t value_size, void* keys, void* values)
{
    map m = map_init(length, key_size, value_size);

    if (length > 0 && keys != NULL && values != NULL)
    {
        memcpy(m.keys, keys, length * key_size);
        memcpy(m.values, values, length * value_size);

        m.length = length;
    }

    return m;
}

void minsert(map* m, void* key, void* value)
{
    if (m->capacity < m->length + 1)
    {
        size_t new_size = (m->capacity == 0) ? 1 : m->capacity * 2;
        void* ktemp = realloc(m->keys, new_size * m->key_size);
        
        if (ktemp == NULL)
        {
            printf("Failed to insert new key to map!\n");

            return;
        }
        
        m->keys = ktemp;
        
        void* vtemp = realloc(m->values, new_size * m->value_size);

        if (vtemp == NULL)
        {
            printf("Failed to insert new value to map!\n");

            free(m->keys);
            m->keys = NULL;

            return;
        }

        m->values = vtemp;
        m->capacity = new_size;
    }

    memcpy(((char*)m->keys + (m->length * m->key_size)), key, m->key_size);
    memcpy(((char*)m->values + (m->length * m->value_size)), value, m->value_size);
    m->length++;
}
void mremove(map* m, void* key)
{
    // TO BE IMPLEMENTED
}
int mfindv(map* m, void* key, void* target)
{
    for (size_t i = 0; i < m->length; i++)
    {
        if (memcmp((void*)((char*)m->keys + (i * m->key_size)), key, m->key_size) == 0)
        {
            if (target != NULL)
            {
                memcpy(target, (char*)m->values + (i * m->value_size), m->value_size);
            }

            return 1;
        }
    }

    return 0;
}
void* mgetv(map* m, void* key)
{
    for (size_t i = 0; i < m->length; i++)
    {
        if (memcmp((void*)((char*)m->keys + (i * m->key_size)), key, m->key_size) == 0)
        {
            return (void*)((char*)m->values + (i * m->value_size));
        }
    }

    return NULL;
}

void* mfirstk(map* m)
{
    return m->keys;
}
void* mlastk(map* m)
{
    if (m->length == 0) return NULL;

    return (void*)((char*)m->keys + ((m->length - 1) * m->key_size));
}
void* mfirstv(map* m)
{
    return m->values;
}
void* mlastv(map* m)
{
    if (m->length == 0) return NULL;

    return (void*)((char*)m->values + ((m->length - 1) * m->value_size));
}

size_t mlength(map* m)
{
    return m->length;
}
size_t mcapacity(map* m)
{
    return m->capacity;
}
size_t mkey_size(map* m)
{
    return m->key_size;
}
size_t mvalue_size(map* m)
{
    return m->value_size;
}

void mpop(map* m)
{
    if (m->length > 0)
    {
        m->length--;
    }
}
void mset(map* m, void* key, void* value)
{
    for(size_t i = 0; i < m->length; i++)
    {
        if (memcmp((void*)((char*)m->keys + (i * m->key_size)), key, m->key_size) == 0)
        {
            memcpy((void*)((char*)m->values + (i * m->value_size)), value, m->value_size);
        }
    }
}

void mforeach(map* m, void(*function)(void*, void*))
{
    for (size_t i = 0; i < m->length; i++)
    {
        function((void*)((char*)m->keys + (i * m->key_size)), (void*)((char*)m->values + (i * m->value_size)));
    }
}
void destroy_map(map* m)
{
    if (m == NULL) return;

    if (m->keys != NULL)
    {
        free(m->keys);
    }

    if (m->values != NULL)
    {
        free(m->values);
    }

    m->keys = NULL;
    m->values = NULL;
    m->length = 0;
    m->capacity = 0;
    m->key_size = 0;
    m->value_size = 0;
}
