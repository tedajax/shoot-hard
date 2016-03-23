#include "debug.h"
#include <cstdio>

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
}
