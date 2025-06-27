#ifndef STI_UTILITY_26062025
#define STI_UTILITY_26062025

#include <stddef.h>

#define STI_SWAP(x, y, type) \
    do { \
    type _sti_temp_26062025 = x; \
    (x) = (y); \
    (y) = _sti_temp_26062025; \
    } while(0)


typedef unsigned char STI_BOOL;

#define STI_TRUE 1
#define STI_FALSE 0

typedef struct STI_Finder
{
    STI_BOOL is_found;
    union
    {
        size_t index;
        void* ptr;
    } result;
    
} STI_Finder;

#endif