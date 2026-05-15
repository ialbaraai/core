#include "../include/core_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

string_t core_string_init(const size_t capacity)
{
    string_t string = {._String_Capacity = capacity, ._String_Size = 0, ._String_Data = malloc(capacity * sizeof(char))};

    if (!string._String_Data)
    {
        string._String_Capacity = 0;
    }
    
    if (string._String_Data) string._String_Data[0] = '\0';

    return string;
}
string_t core_string_init_data(const size_t capacity, const char* data)
{
    string_t string = core_string_init(capacity);

    if (string._String_Data && data)
    {
        size_t datalen = strlen(data);

        size_t ocucapacity = capacity > datalen + 1 ? datalen + 1 : capacity;
        memcpy(string._String_Data, data, ocucapacity);
        string._String_Size = ocucapacity - 1;

        string._String_Data[string._String_Size] = '\0';
    }

    return string;
}

int core_string_set(string_t* string, const char* data)
{
    if (!data || !string) return 0;
    if (!string->_String_Data) return 0;

    size_t datalen = strlen(data);

    if (datalen + 1 > string->_String_Capacity)
    {
        size_t newcapacity = string->_String_Capacity * 2 > datalen + 1 ? string->_String_Capacity * 2 : datalen + 1;

        if (newcapacity > STRING_MAX_CAPACITY) return 0;

        char* temp = realloc(string->_String_Data, newcapacity * sizeof(char));

        if (!temp)
        {
            return 0;
        }

        string->_String_Data = temp;
        string->_String_Capacity = newcapacity;
    }

    memcpy(string->_String_Data, data, datalen + 1);
    string->_String_Size = datalen;
    string->_String_Data[string->_String_Size] = '\0';

    return 1;
}
int core_string_copy(string_t* destination, const string_t* source)
{
    if (!destination || !source) return 0;
    if (!destination->_String_Data || !source->_String_Data) return 0;

    size_t srclen = core_string_get_size(source);

    if (srclen + 1 > destination->_String_Capacity)
    {
        size_t newcapacity = destination->_String_Capacity * 2 > srclen + 1 ? destination->_String_Capacity * 2 : srclen + 1;

        if (newcapacity > STRING_MAX_CAPACITY) return 0;

        char* temp = realloc(destination->_String_Data, newcapacity * sizeof(char));

        if (!temp)
        {
            return 0;
        }

        destination->_String_Data = temp;
        destination->_String_Capacity = newcapacity;
    }

    memcpy(destination->_String_Data, source->_String_Data, srclen);
    destination->_String_Size = srclen;
    destination->_String_Data[destination->_String_Size] = '\0';

    return 1;
}
int core_string_compare(const string_t* first, const string_t* second)
{
    if (!first || !second) return 0;
    if (!first->_String_Data || !second->_String_Data) return 0;

    if (strcmp(first->_String_Data, second->_String_Data) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int core_string_pop_back(string_t* string)
{
    if (!string) return 0;
    if (!string->_String_Data) return 0;
    if (string->_String_Size < 1) return 0;

    string->_String_Size--;
    string->_String_Data[string->_String_Size] = '\0';
    return 1;
}

int core_string_append_char(string_t* string, const char character)
{
    if (!string) return 0;
    if (!string->_String_Data) return 0;
    if (character == '\0') return 0;

    if (string->_String_Size + sizeof(character) + 1 > string->_String_Capacity)
    {
        size_t newcapacity = string->_String_Capacity * 2 > string->_String_Size + sizeof(character) + 1 ? string->_String_Capacity * 2 : string->_String_Size + sizeof(character) + 1;

        if (newcapacity > STRING_MAX_CAPACITY) return 0;

        char* temp = realloc(string->_String_Data, newcapacity * sizeof(char));

        if (!temp)
        {
            return 0;
        }

        string->_String_Data = temp;
        string->_String_Capacity = newcapacity;
    }

    string->_String_Data[string->_String_Size] = character;
    string->_String_Size++;
    string->_String_Data[string->_String_Size] = '\0';

    return 1;
}
int core_string_append_cstr(string_t* string, const char* source)
{
    if (!string) return 0;
    if (!string->_String_Data) return 0;
    if (!source) return 0;

    size_t sourcelen = strlen(source);

    if (string->_String_Size + sourcelen + 1 > string->_String_Capacity)
    {
        size_t newcapacity = string->_String_Capacity * 2 > string->_String_Size + sourcelen + 1 ? string->_String_Capacity * 2 : string->_String_Size + sourcelen + 1;

        if (newcapacity > STRING_MAX_CAPACITY) return 0;

        char* temp = realloc(string->_String_Data, newcapacity * sizeof(char));

        if (!temp)
        {
            return 0;
        }

        string->_String_Data = temp;
        string->_String_Capacity = newcapacity;
    }

    memcpy(string->_String_Data + string->_String_Size, source, sourcelen);
    string->_String_Size += sourcelen;
    string->_String_Data[string->_String_Size] = '\0';

    return 1;
}
int core_string_append_string(string_t* string, const string_t* source)
{
    if (!string) return 0;
    if (!source) return 0;
    if (!string->_String_Data) return 0;
    if (!source->_String_Data) return 0;

    size_t sourcelen = core_string_get_size(source);

    if (string->_String_Size + sourcelen + 1 > string->_String_Capacity)
    {
        size_t newcapacity = string->_String_Capacity * 2 > string->_String_Size + sourcelen + 1 ? string->_String_Capacity * 2 : string->_String_Size + sourcelen + 1;

        if (newcapacity > STRING_MAX_CAPACITY) return 0;

        char* temp = realloc(string->_String_Data, newcapacity * sizeof(char));

        if (!temp)
        {
            return 0;
        }

        string->_String_Data = temp;
        string->_String_Capacity = newcapacity;
    }

    memcpy(string->_String_Data + string->_String_Size, source->_String_Data, sourcelen);
    string->_String_Size += sourcelen;
    string->_String_Data[string->_String_Size] = '\0';

    return 1;
}

void core_string_input(string_t* string)
{
    core_string_set(string, "");

    int curchar = getchar();

    while (curchar != '\n' && curchar != EOF)
    {
        core_string_append_char(string, (char)curchar);
        curchar = getchar();
    }
}

void core_string_lower(string_t* string)
{
    if (!string) return;
    if (!string->_String_Data) return;

    for (size_t i = 0; i < string->_String_Size; ++i)
    {
        string->_String_Data[i] = tolower((unsigned char)string->_String_Data[i]);
    }
}
void core_string_upper(string_t* string)
{
    if (!string) return;
    if (!string->_String_Data) return;
    
    for (size_t i = 0; i < string->_String_Size; ++i)
    {
        string->_String_Data[i] = toupper((unsigned char)string->_String_Data[i]);
    }
}

size_t core_string_get_size(const string_t* string)
{
    if (!string) return 0;

    return string->_String_Size;
}
size_t core_string_get_capacity(const string_t* string)
{
    if (!string) return 0;

    return string->_String_Capacity;
}
const char* core_string_get_data(const string_t* string)
{
    if (!string) return NULL;
    if (!string->_String_Data) return NULL;

    return string->_String_Data;
}

char core_string_at(const string_t* string, size_t index)
{
    if (!string) return '\0';
    if (!string->_String_Data) return '\0';
    if (index >= string->_String_Size) return '\0';

    return string->_String_Data[index];
}
int core_string_find(const string_t* string, const char character, size_t* index)
{
    if (!string) return 0;
    if (!string->_String_Data) return 0;
    
    int retval = 0;

    for (size_t i = 0; i < string->_String_Size; ++i)
    {
        if (string->_String_Data[i] == character)
        {
            if (index)
            {
                *index = i;
            }

            retval = 1;
            break;
        }
    }

    return retval;
}
int core_string_replace_at(string_t* string, const size_t index, const char character)
{
    if (!string) return 0;
    if (!string->_String_Data) return 0;
    if (index >= string->_String_Size) return 0;

    string->_String_Data[index] = character;
    return 1;
}
void core_string_replace(string_t* string, const char oldc, const char newc)
{
    if (!string) return;
    if (!string->_String_Data) return;
    if (oldc == '\0') return;
    if (newc == '\0') return;

    for(size_t i = 0; i < string->_String_Size; ++i)
    {
        if (string->_String_Data[i] == oldc)
        {
            string->_String_Data[i] = newc;
        }
    }
}

void core_string_destroy(string_t* string)
{
    if (!string) return;

    if (string->_String_Data)
    {
        free(string->_String_Data);
    }

    string->_String_Data = NULL;
    string->_String_Capacity = 0;
    string->_String_Size = 0;
}