#include "../platform.h"
#include "../array.h"
#include <mutex>
#include "../logger.h"
#include "../memory.h"

#include <Windows.h>
#include <ShlObj.h>

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

#pragma comment(lib, "winmm.lib")

using namespace foundation::string_stream;

namespace core
{
    std::mutex g_format_trace_mutex;

    void get_stack_trace(StackTrace& _stackTraceOut, int _skipAmount /* = 1 */, int _maxAmount /* = cMaxStackTraceSize */)
    {
        foundation::array::set_capacity(_stackTraceOut, _maxAmount);
        ASSERT((_skipAmount >= 0) && (_skipAmount < _maxAmount), "Invalid skip count.");
        unsigned short frames = CaptureStackBackTrace(_skipAmount, _maxAmount, foundation::array::begin(_stackTraceOut), nullptr);
        _stackTraceOut._size = frames;
    }

    void format_stack_trace(const StackTrace& _stackTrace, Buffer& _bufferOut)
    {
        std::lock_guard<std::mutex> lock(g_format_trace_mutex);

#ifdef DBGHELP_TRANSLATE_TCHAR
#error Wide dbghelp is not supported.
#endif

        static char s_symbolBuffer[sizeof(SYMBOL_INFO) + 256];

        SYMBOL_INFO* symbol = (SYMBOL_INFO*)s_symbolBuffer;
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        IMAGEHLP_LINE64 line;
        DWORD displacement;

        HANDLE process = GetCurrentProcess();
        SymInitialize(process, nullptr, TRUE);

        const int numFrames = (int)_stackTrace._size;

        _bufferOut._allocator = &foundation::memory_globals::default_allocator();
        foundation::array::reserve(_bufferOut, 4096);
        _bufferOut << '\n';
        for (int i = 0; i < numFrames; ++i) {
            const mem_uint address = (mem_uint)_stackTrace[i];
            SymFromAddr(process, address, 0, symbol);
            const char* filename = "???";
            int lineNumber = -1;
            if (SymGetLineFromAddr64(process, address, &displacement, &line) == TRUE) {
                filename = line.FileName;
                lineNumber = line.LineNumber;
            }
            else {
                printf("SymGetLineFromAddr64 returned error %lu.\n", GetLastError());
            }

            char addressBuffer[17];
#if defined(CORE_IS_64_BIT)
            snprintf(addressBuffer, 16, "0x%016I64X", address);
#else
            snprintf(addressBuffer, 8, "0x%08X", address);
#endif
            _bufferOut << filename << "(" << lineNumber << "): " << symbol->Name << " - " << addressBuffer << " (" << numFrames - i - 1 << ")\n";

            if (strcmp(symbol->Name, "main") == 0) {
                if (numFrames - i - 2 >= 0) {
                    _bufferOut << "\"" << numFrames - i - 2 << ": Stack frames about \"main\" not displayed.\n";
                }
                break;
            }
        }
    }
}