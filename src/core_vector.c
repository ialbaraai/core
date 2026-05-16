#include "../include/core_vector.h"

#include <stdlib.h>
#include <string.h>

vector_t core_vector_init(const size_t capacity, const size_t element_size, void (*copy_function)(void* destination, const void* source), void (*destroy_function)(void* object))
{
    vector_t vector = {._Vector_Capacity = capacity, ._Vector_Element_Size = element_size, ._Vector_Size = 0, ._Vector_Data = malloc(capacity * element_size)};

    if (!vector._Vector_Data)
    {
        vector._Vector_Capacity = 0;
    }

    vector._Vector_Copy_Function = copy_function;
    vector._Vector_Destroy_Function = destroy_function;

    return vector;
}

vector_t core_vector_init_data(const size_t capacity, const size_t element_size, const size_t count, void (*copy_function)(void* destination, const void* source), void (*destroy_function)(void* object), const void* data)
{
    vector_t vector = core_vector_init(capacity, element_size, copy_function, destroy_function);

    if (vector._Vector_Data)
    {
        memcpy(vector._Vector_Data, data, count * element_size);
        vector._Vector_Size = count;
    }

    return vector;
}

int core_vector_push_back(vector_t* vector, const void* value)
{
    if (!vector) return 0;
    if (!vector->_Vector_Data) return 0;
    if (!value) return 0;

    if (vector->_Vector_Size + 1 > vector->_Vector_Capacity)
    {
        size_t newcapacity = vector->_Vector_Capacity * 2 > vector->_Vector_Size + 1 ? vector->_Vector_Capacity * 2 : vector->_Vector_Size + 1;

        if (newcapacity > VECTOR_MAX_CAPACITY) return 0;

        void* temp = realloc(vector->_Vector_Data, newcapacity * vector->_Vector_Element_Size);

        if (!temp)
        {
            return 0;
        }

        vector->_Vector_Data = temp;
        vector->_Vector_Capacity = newcapacity;
    }

    if (vector->_Vector_Copy_Function)
    {
        vector->_Vector_Copy_Function((void*)((char*)vector->_Vector_Data + (vector->_Vector_Size * vector->_Vector_Element_Size)), value);
    }
    else
    {
        memcpy((void*)((char*)vector->_Vector_Data + (vector->_Vector_Size * vector->_Vector_Element_Size)), value, vector->_Vector_Element_Size);
    }

    vector->_Vector_Size++;
    return 1;
}
int core_vector_pop_back(vector_t* vector)
{
    if (!vector) return 0;
    if (!vector->_Vector_Data) return 0;
    if (vector->_Vector_Size < 1) return 0;

    if (vector->_Vector_Destroy_Function)
    {
        vector->_Vector_Destroy_Function((void*)((char*)vector->_Vector_Data + ((vector->_Vector_Size - 1) * vector->_Vector_Element_Size)));
    }

    vector->_Vector_Size--;
    return 1;
}
int core_vector_set(vector_t* vector, size_t index, const void* value)
{
    if (!vector) return 0;
    if (!vector->_Vector_Data) return 0;
    if (index >= vector->_Vector_Size) return 0;

    if (vector->_Vector_Destroy_Function)
    {
        vector->_Vector_Destroy_Function((void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size)));
    }

    if (vector->_Vector_Copy_Function)
    {
        vector->_Vector_Copy_Function((void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size)), value);
    }
    else
    {
        memcpy((void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size)), value, vector->_Vector_Element_Size);
    }

    return 1;
}
int core_vector_remove(vector_t* vector, const size_t index)
{
    if (!vector) return 0;
    if (!vector->_Vector_Data) return 0;
    if (vector->_Vector_Size < 1) return 0;
    if (index >= vector->_Vector_Size) return 0;

    if (vector->_Vector_Destroy_Function)
    {
        vector->_Vector_Destroy_Function((void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size)));
    }

    if (index != vector->_Vector_Size - 1)
        memcpy((void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size)), (void*)((char*)vector->_Vector_Data + ((vector->_Vector_Size - 1) * vector->_Vector_Element_Size)), vector->_Vector_Element_Size);

    vector->_Vector_Size--;
    return 1;
}
int core_vector_clear(vector_t* vector)
{
    if (!vector) return 0;
    if (!vector->_Vector_Data) return 0;

    if (vector->_Vector_Destroy_Function)
    {
        for (size_t i = 0; i < vector->_Vector_Size; ++i)
        {
            vector->_Vector_Destroy_Function((void*)((char*)vector->_Vector_Data + (i * vector->_Vector_Element_Size)));
        }
    }

    vector->_Vector_Size = 0;
    return 1;
}

void core_vector_foreach(vector_t* vector, void (*function)(const size_t index, void* data))
{
    if (!vector) return;
    if (!vector->_Vector_Data) return;
    if (!function) return;

    for(size_t i = 0; i < vector->_Vector_Size; ++i)
    {
        function(i, (void*)((char*)vector->_Vector_Data + (i * vector->_Vector_Element_Size)));
    }
}

size_t core_vector_get_size(const vector_t* vector)
{
    if (!vector) return 0;
    
    return vector->_Vector_Size;
}
size_t core_vector_get_element_size(const vector_t* vector)
{
    if (!vector) return 0;

    return vector->_Vector_Element_Size;
}
size_t core_vector_get_capacity(const vector_t* vector)
{
    if (!vector) return 0;

    return vector->_Vector_Capacity;
}
const void* core_vector_get_data(const vector_t* vector)
{
    if (!vector) return NULL;
    if (!vector->_Vector_Data) return NULL;

    return vector->_Vector_Data;
}
void* core_vector_first(const vector_t* vector)
{
    if (!vector) return NULL;
    if (!vector->_Vector_Data) return NULL;

    return vector->_Vector_Data;
}
void* core_vector_last(const vector_t* vector)
{
    if (!vector) return NULL;
    if (!vector->_Vector_Data) return NULL;
    if (vector->_Vector_Size == 0)
    {
        return NULL;
    }

    return (void*)((char*)vector->_Vector_Data + ((vector->_Vector_Size - 1) * vector->_Vector_Element_Size));
}
int core_vector_at(const vector_t* vector, const size_t index, void* value)
{
    if (!vector) return 0;
    if (!vector->_Vector_Data) return 0;
    if (index >= vector->_Vector_Size) return 0;

    memcpy(value, (void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size)), vector->_Vector_Element_Size);
    return 1;
}
void* core_vector_get(const vector_t* vector, const size_t index)
{
    if (!vector) return NULL;
    if (!vector->_Vector_Data) return NULL;
    if (index >= vector->_Vector_Size) return NULL;

    return (void*)((char*)vector->_Vector_Data + (index * vector->_Vector_Element_Size));
}

void core_vector_destroy(vector_t* vector)
{
    if (!vector) return;

    if (vector->_Vector_Data)
    {
        if (vector->_Vector_Destroy_Function)
        {
            for (size_t i = 0; i < vector->_Vector_Size; ++i)
            {
                vector->_Vector_Destroy_Function((void*)((char*)vector->_Vector_Data + (i * vector->_Vector_Element_Size)));
            }
        }

        free(vector->_Vector_Data);
    }

    vector->_Vector_Data = NULL;
    vector->_Vector_Element_Size = 0;

    vector->_Vector_Capacity = 0;
    vector->_Vector_Size = 0;

    vector->_Vector_Copy_Function = NULL;
    vector->_Vector_Destroy_Function = NULL;
}