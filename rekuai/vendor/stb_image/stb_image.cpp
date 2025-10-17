#include "kpch.h"

#define STB_IMAGE_IMPLEMENTATION

#if defined(__GNUC__) && !defined(NDEBUG) && defined(__OPTIMIZE__)
    #warning "Undefing __OPTIMIZE__ because of fmt"
    #undef __OPTIMIZE__
#endif

#include "stb_image.h"
