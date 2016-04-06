#include "debug.h"
#include <cstdio>
#include <cstdarg>

namespace internal
{
    void print_assert(const char* expr, const char* file, int line, const char* msg)
    {
        if (msg) {
            fprintf(stderr, "\n-------------------\nASSERT(%s) -- %s\n%s:%d\n--------------------\n", expr, msg, file, line);
        }
        else {
            fprintf(stderr, "\n-------------------\nASSERT(%s)\n%s:%d\n--------------------\n", expr, file, line);
        }
    }

    void printf_assert(const char* expr, const char* file, int line, const char* fmt, ...)
    {
        if (fmt) {
            const size_t maxBufferSize = 4096;
            char buffer[maxBufferSize];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buffer, maxBufferSize, fmt, args);
            fprintf(stderr, "\n-------------------\nASSERT(%s) -- %s\n", expr, buffer);
            fprintf(stderr, "%s:%d\n--------------------\n", file, line);
            va_end(args);
        }
        else {
            fprintf(stderr, "\n-------------------\nASSERT(%s)\n%s:%d\n--------------------\n", expr, file, line);
        }
    }
}
