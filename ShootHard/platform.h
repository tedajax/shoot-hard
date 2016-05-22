#pragma once

#include "memory.h"
#include "string_stream.h"

namespace core
{
    typedef foundation::Array<void*> StackTrace;
    const static int cMaxStackTraceSize = 64;

    namespace internal
    {
        void init_platform();
        void shutdown_platform();
    }

#ifdef IS_DEBUG
#define CORE_MEMORY_TRACKING_DEFAULT true
#else
#define CORE_MEMORY_TRACKING_DEFAULT false
#endif

    void init(bool _enableMemTrack = CORE_MEMORY_TRACKING_DEFAULT);

#undef CORE_MEMORY_TRACKING_DEFAULT

    void shutdown();

    void get_stack_trace(StackTrace& _stackTraceOut, int _skipAmount = 1, int _maxAmount = cMaxStackTraceSize);
    void format_stack_trace(const StackTrace& _stackTrace, foundation::string_stream::Buffer& _bufferOut);
}