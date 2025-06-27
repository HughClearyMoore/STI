#ifndef STI_STRING_27062025
#define STI_STRING_27062025

#include <stddef.h>

typedef struct STI_String
{
    size_t length;
    char* data;
} STI_String;


STI_String STIStringCreate(const char* string);
STI_String STIStringMoveCreate(char* string);
void STIStringDestroy(STI_String* string);

STI_String STIStringClone(const STI_String* string);

typedef struct STI_StringView
{
    size_t length;
    const char* data;
} STI_StringView;

STI_StringView STI_StringViewCreate(const char* string);

#endif