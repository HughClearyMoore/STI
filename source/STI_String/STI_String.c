#include "STI_String.h"

#include <string.h>

STI_String STIStringCreate(const char* string)
{
    STI_String str = {0};

    if(string)
    {
        str.data = strdup(string);
        str.length = strlen(str.data);
    }

    return str;
}
void STIStringDestroy(STI_String* string)
{
    if(string->data)
    {
        free(string->data);
    }

    *string = (STI_String){0};
}

STI_String STIStringMoveCreate(char* string)
{
    STI_String str = {0};
    str.data = string;
    str.length = strlen(string);
    return str;
}

STI_String STIStringClone(const STI_String* string)
{
    STI_String str = {0};
    str.data = strdup(string->data);
    str.length = string->length;
    return str;
}

STI_StringView STI_StringViewCreate(const char* string)
{
    STI_StringView str = {0};
    str.data = string;
    str.length = strlen(string);
    return str;
}

STI_StringView STI_StringViewCreate(const STI_String* string)
{
    STI_StringView str = {0};
    str.data = string->data;
    str.length = string->length;

    return str;
}