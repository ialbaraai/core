#include "../include/core_map.h"

#include <stdlib.h>
#include <string.h>

map_t core_map_init(const size_t capacity, const size_t key_size, const size_t value_size, void (*key_copy_function)(void* destination, const void* source), void (*key_destroy_function)(void* object), void (*value_copy_function)(void* destination, const void* source), void (*value_destroy_function)(void* object), int (*compare_function)(const void* first, const void* second))
{
    map_t map = {._Map_Capacity = capacity, ._Map_Key_Element_Size = key_size, ._Map_Value_Element_Size = value_size, ._Map_Key_Data = malloc(capacity * key_size), ._Map_Value_Data = malloc(capacity * value_size)};

    if (!map._Map_Key_Data || !map._Map_Value_Data)
    {
        map._Map_Capacity = 0;
    }

    map._Map_Key_Copy_Function = key_copy_function;
    map._Map_Key_Destroy_Function = key_destroy_function;
    map._Map_Value_Copy_Function = value_copy_function;
    map._Map_Value_Destroy_Function = value_destroy_function;
    map._Map_Compare_Function = compare_function;

    return map;
}
map_t core_map_init_data(const size_t capacity, const size_t key_size, const size_t value_size, const size_t count, void (*key_copy_function)(void* destination, const void* source), void (*key_destroy_function)(void* object), void (*value_copy_function)(void* destination, const void* source), void (*value_destroy_function)(void* object), int (*compare_function)(const void* first, const void* second), const void* key_data, const void* value_data)
{
    map_t map = core_map_init(capacity, key_size, value_size, key_copy_function, key_destroy_function, value_copy_function, value_destroy_function, compare_function);

    if (map._Map_Key_Data && map._Map_Value_Data)
    {
        memcpy(map._Map_Key_Data, key_data, count * key_size);
        memcpy(map._Map_Value_Data, value_data, count * value_size);

        map._Map_Size = count;
    }

    return map;
}

int core_map_push_back(map_t* map, const void* key, const void* value)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;

    if (map->_Map_Size + 1 > map->_Map_Capacity)
    {
        size_t newcapacity = map->_Map_Capacity * 2 > map->_Map_Size + 1 ? map->_Map_Capacity * 2 : map->_Map_Size + 1;

        if (newcapacity > MAP_MAX_CAPACITY) return 0;

        void* keytemp = realloc(map->_Map_Key_Data, newcapacity * map->_Map_Key_Element_Size);
        void* valuetemp = realloc(map->_Map_Value_Data, newcapacity * map->_Map_Value_Element_Size);

        if (!keytemp || ! valuetemp) return 0;

        map->_Map_Key_Data = keytemp;
        map->_Map_Value_Data = valuetemp;
        map->_Map_Capacity = newcapacity;
    }

    if (map->_Map_Key_Copy_Function)
    {
        map->_Map_Key_Copy_Function((void*)((char*)map->_Map_Key_Data + (map->_Map_Size * map->_Map_Key_Element_Size)), key);
    }
    else
    {
        memcpy((void*)((char*)map->_Map_Key_Data + (map->_Map_Size * map->_Map_Key_Element_Size)), key, map->_Map_Key_Element_Size);
    }

    if (map->_Map_Value_Copy_Function)
    {
        map->_Map_Value_Copy_Function((void*)((char*)map->_Map_Value_Data + (map->_Map_Size * map->_Map_Value_Element_Size)), value);
    }
    else
    {
        memcpy((void*)((char*)map->_Map_Value_Data + (map->_Map_Size * map->_Map_Value_Element_Size)), value, map->_Map_Value_Element_Size);
    }

    map->_Map_Size++;
    return 1;
}
int core_map_pop_back(map_t* map)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;
    if (map->_Map_Size < 1) return 0;

    if (map->_Map_Key_Destroy_Function)
    {
        map->_Map_Key_Destroy_Function((void*)((char*)map->_Map_Key_Data + ((map->_Map_Size - 1) * map->_Map_Key_Element_Size)));
    }
    if (map->_Map_Value_Destroy_Function)
    {
        map->_Map_Value_Destroy_Function((void*)((char*)map->_Map_Value_Data + ((map->_Map_Size - 1) * map->_Map_Value_Element_Size)));
    }

    map->_Map_Size--;
    return 1;
}
int core_map_set(map_t* map, const void* key, const void* value)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;

    for (size_t i = 0; i < map->_Map_Size; ++i)
    {
        void* curkey = (void*)((char*)map->_Map_Key_Data + (i * map->_Map_Key_Element_Size));
        void* curvalue = (void*)((char*)map->_Map_Value_Data + (i * map->_Map_Value_Element_Size));

        if (map->_Map_Compare_Function(curkey, key) == 0)
        {
            if (map->_Map_Value_Destroy_Function)
            {
                map->_Map_Value_Destroy_Function(curvalue);
            }

            if (map->_Map_Value_Copy_Function)
            {
                map->_Map_Value_Copy_Function(curvalue, value);
            }
            else
            {
                memcpy(curvalue, value, map->_Map_Value_Element_Size);
            }

            return 1;
        }
    }

    return 0;
}
int core_map_remove(map_t* map, const void* key)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;

    int index = -1;

    for (size_t i = 0; i < map->_Map_Size; i++)
    {
        void* curkey = (void*)((char*)map->_Map_Key_Data + (i * map->_Map_Key_Element_Size));

        if (map->_Map_Compare_Function(key, curkey) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return 0;
    }

    if (map->_Map_Key_Destroy_Function)
    {
        map->_Map_Key_Destroy_Function((void*)((char*)map->_Map_Key_Data + (index * map->_Map_Key_Element_Size)));
    }
    if (map->_Map_Value_Destroy_Function)
    {
        map->_Map_Value_Destroy_Function((void*)((char*)map->_Map_Value_Data + (index * map->_Map_Value_Element_Size)));
    }

    if (index != map->_Map_Size - 1)
    {
        if (map->_Map_Key_Copy_Function)
        {
            map->_Map_Key_Copy_Function((void*)((char*)map->_Map_Key_Data + (index * map->_Map_Key_Element_Size)), (void*)((char*)map->_Map_Key_Data + ((map->_Map_Size - 1) * map->_Map_Key_Element_Size)));
        }
        else
        {
            memcpy((void*)((char*)map->_Map_Key_Data + (index * map->_Map_Key_Element_Size)), (void*)((char*)map->_Map_Key_Data + ((map->_Map_Size - 1) * map->_Map_Key_Element_Size)), map->_Map_Key_Element_Size);
        }

        if (map->_Map_Value_Copy_Function)
        {
            map->_Map_Value_Copy_Function((void*)((char*)map->_Map_Value_Data + (index * map->_Map_Value_Element_Size)), (void*)((char*)map->_Map_Value_Data + ((map->_Map_Size - 1) * map->_Map_Value_Element_Size)));
        }
        else
        {
            memcpy((void*)((char*)map->_Map_Value_Data + (index * map->_Map_Value_Element_Size)), (void*)((char*)map->_Map_Value_Data + ((map->_Map_Size - 1) * map->_Map_Value_Element_Size)), map->_Map_Value_Element_Size);
        }
    }

    map->_Map_Size--;
    return 1;
}

void core_map_foreach(map_t* map, void (*function)(void* key, void* value))
{
    if (!map) return;
    if (!map->_Map_Key_Data) return;
    if (!map->_Map_Value_Data) return;
    if (!function) return;

    for (size_t i = 0; i < map->_Map_Size; ++i)
    {
        function((void*)((char*)map->_Map_Key_Data + (i * map->_Map_Key_Element_Size)), (void*)((char*)map->_Map_Value_Data + (i * map->_Map_Value_Element_Size)));
    }
}

size_t core_map_get_key_size(const map_t* map)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;

    return map->_Map_Key_Element_Size;
}
size_t core_map_get_value_size(const map_t* map)
{
    if (!map) return 0;
    if (!map->_Map_Value_Data) return 0;

    return map->_Map_Value_Element_Size;
}
size_t core_map_get_size(const map_t* map)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;

    return map->_Map_Size;
}
size_t core_map_get_capacity(const map_t* map)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;

    return map->_Map_Capacity;
}
const void* core_map_get_key_data(const map_t* map)
{
    if (!map) return NULL;
    if (!map->_Map_Key_Data) return NULL;

    return map->_Map_Key_Data;
}
const void* core_map_get_value_data(const map_t* map)
{
    if (!map) return NULL;
    if (!map->_Map_Value_Data) return NULL;

    return map->_Map_Value_Data;
}

void* core_map_first_key(const map_t* map)
{
    if (!map) return NULL;
    if (!map->_Map_Key_Data) return NULL;

    return map->_Map_Key_Data;
}
void* core_map_last_key(const map_t* map)
{
    if (!map) return NULL;
    if (!map->_Map_Key_Data) return NULL;
    if (map->_Map_Size == 0) return NULL;

    return (void*)((char*)map->_Map_Key_Data + ((map->_Map_Size - 1) * map->_Map_Key_Element_Size));
}

void* core_map_first_value(const map_t* map)
{
    if (!map) return NULL;
    if (!map->_Map_Value_Data) return NULL;

    return map->_Map_Value_Data;
}
void* core_map_last_value(const map_t* map)
{
    if (!map) return NULL;
    if (!map->_Map_Value_Data) return NULL;
    if (map->_Map_Size == 0) return NULL;

    return (void*)((char*)map->_Map_Value_Data + ((map->_Map_Size - 1) * map->_Map_Value_Element_Size));
}

int core_map_get_value_at(const map_t* map, const void* key, void* value)
{
    if (!map) return 0;
    if (!map->_Map_Key_Data) return 0;
    if (!map->_Map_Value_Data) return 0;

    for (size_t i = 0; i < map->_Map_Size; ++i)
    {
        void* curkey = (void*)((char*)map->_Map_Key_Data + (i * map->_Map_Key_Element_Size));
        void* curvalue = (void*)((char*)map->_Map_Value_Data + (i * map->_Map_Value_Element_Size));

        if (map->_Map_Compare_Function(curkey, key) == 0)
        {
            if (map->_Map_Value_Copy_Function)
            {
                map->_Map_Value_Copy_Function(value, curvalue);
            }
            else
            {
                memcpy(value, curvalue, map->_Map_Value_Element_Size);
            }

            return 1;
        }
    }

    return 0;
}
void* core_map_get_value(const map_t* map, const void* key)
{
    if (!map) return NULL;
    if (!map->_Map_Key_Data) return NULL;
    if (!map->_Map_Value_Data) return NULL;

    for (size_t i = 0; i < map->_Map_Size; ++i)
    {
        void* curkey = (void*)((char*)map->_Map_Key_Data + (i * map->_Map_Key_Element_Size));
        void* curvalue = (void*)((char*)map->_Map_Value_Data + (i * map->_Map_Value_Element_Size));

        if (map->_Map_Compare_Function(curkey, key) == 0)
        {
            return curvalue;
        }
    }

    return NULL;
}

void core_map_destroy(map_t* map)
{
    if (!map) return;

    if (map->_Map_Key_Data)
    {
        if (map->_Map_Key_Destroy_Function)
        {
            for (size_t i = 0; i < map->_Map_Size; ++i)
            {
                map->_Map_Key_Destroy_Function((void*)((char*)map->_Map_Key_Data + (i * map->_Map_Key_Element_Size)));
            }
        }

        free(map->_Map_Key_Data);
    }

    if (map->_Map_Value_Data)
    {
        if (map->_Map_Value_Destroy_Function)
        {
            for (size_t i = 0; i < map->_Map_Size; ++i)
            {
                map->_Map_Value_Destroy_Function((void*)((char*)map->_Map_Value_Data + (i * map->_Map_Value_Element_Size)));
            }
        }

        free(map->_Map_Value_Data);
    }

    map->_Map_Key_Data = NULL;
    map->_Map_Value_Data = NULL;

    map->_Map_Key_Element_Size = 0;
    map->_Map_Value_Element_Size = 0;

    map->_Map_Capacity = 0;
    map->_Map_Size = 0;

    map->_Map_Key_Copy_Function = NULL;
    map->_Map_Key_Destroy_Function = NULL;

    map->_Map_Value_Copy_Function = NULL;
    map->_Map_Value_Destroy_Function = NULL;
}