#pragma once

#include "memory.h"
#include "string_stream.h"

namespace core
{
    typedef foundation::Array<void*> StackTrace;
    const static int cMaxStackTraceSize = 64;

    void get_stack_trace(StackTrace& _stackTraceOut, int _skipAmount = 1, int _maxAmount = cMaxStackTraceSize);
    void format_stack_trace(const StackTrace& _stackTrace, foundation::string_stream::Buffer& _bufferOut);
}