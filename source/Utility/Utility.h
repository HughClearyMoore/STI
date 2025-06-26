#ifndef STI_UTILITY_26062025
#define STI_UTILITY_26062025

#define STI_SWAP(x, y, type) \
    do { \
    type _sti_temp_26062025 = x; \
    (x) = (y); \
    (y) = _sti_temp_26062025; \
    } while(0) \

#endif