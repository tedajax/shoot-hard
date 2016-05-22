#pragma once

#include "platform_defines.h"

namespace internal
{
    void print_assert(const char* expr, const char* file, int line, const char* msg);
    void printf_assert(const char* exp, const char* file, int line, const char* fmt, ...);
}

#define MULTILINE_MACRO_BEGIN() do {
#define MULTILINE_MACRO_END() } while (0);

#define ASSERT_ALWAYS(expr, msg) MULTILINE_MACRO_BEGIN()        \
    static bool s_skipAssert = false;                                \
    if (!s_skipAssert && !(expr)) {                                  \
        ::internal::print_assert(#expr, __FILE__, __LINE__, msg);     \
        CORE_BREAKPOINT(); \
    }                                                               \
    MULTILINE_MACRO_END();


#define ASSERTF_ALWAYS(expr, msg, ...) MULTILINE_MACRO_BEGIN()        \
    static bool s_skipAssert = false;                                \
    if (!s_skipAssert && !(expr)) {                                  \
        ::internal::printf_assert(#expr, __FILE__, __LINE__, msg, __VA_ARGS__);     \
        CORE_BREAKPOINT(); \
    }                                                               \
    MULTILINE_MACRO_END();

#if defined(DEBUG) || defined(_DEBUG)
#define IF_DEBUG(x) x
#define ASSERT ASSERT_ALWAYS
#define ASSERTF ASSERTF_ALWAYS
#define IS_DEBUG 1
#else
#define IF_DEBUG(x)
#define ASSERT(expr, msg) ((void)0)
#define ASSERTF(expr, msg, ...) ((void)0)
#define IS_DEBUG 0
#endif
