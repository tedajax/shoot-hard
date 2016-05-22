#include "debug.h"
#include <cstdarg>
#include <cstdio>
#include "logger.h"
#include "platform.h"

namespace internal
{
    void print_assert(const char* expr, const char* file, int line, const char* msg)
    {
        if (msg) {
            logger::debug(nullptr, "\n-------------------\nASSERT(%s) -- %s\n%s:%d\n--------------------\n", expr, msg, file, line);
        }
        else {
            logger::debug(nullptr, "\n-------------------\nASSERT(%s)\n%s:%d\n--------------------\n", expr, file, line);
        }

        core::StackTrace stackTraceAddresses(foundation::memory_globals::default_allocator());
        core::get_stack_trace(stackTraceAddresses, 2);

        foundation::string_stream::Buffer formattedTrace;
        core::format_stack_trace(stackTraceAddresses, formattedTrace);
        logger::debug(nullptr, foundation::string_stream::c_str(formattedTrace));

        logger::debug(nullptr, "---------------");
    }

    void printf_assert(const char* expr, const char* file, int line, const char* fmt, ...)
    {
        const size_t maxBufferSize = 4096;
        char buffer[maxBufferSize];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, maxBufferSize, fmt, args);
        va_end(args);
        print_assert(expr, file, line, buffer);
    }
}
