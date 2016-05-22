#pragma once

namespace logger
{
    enum class LogLevel
    {
        cRaw,
        cDebug,
        cInfo,
        cWarn,
        cError,
    };

    void init();
    void shutdown();

    void log(LogLevel _level, const char* _context, const char* _message);
    void logf(LogLevel _level, const char* _context, const char* _format, ...);

    void raw(const char* _context, const char* _format, ...);
    void debug(const char* _context, const char* _format, ...);
    void info(const char* _context, const char* _format, ...);
    void warn(const char* _context, const char* _format, ...);
    void error(const char* _context, const char* _format, ...);
}