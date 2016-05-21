#include "logger.h"
#include <cstdio>
#include <cstdarg>
#include <experimental/filesystem>

namespace logger
{
    namespace internal
    {
        struct Logger
        { 
            Logger()
                : next(nullptr), level(LogLevel::cInfo)
            { }
            
            virtual ~Logger()
            {
                if (next) {
                    delete next;
                }
            }
            
            void log(LogLevel _level, const char* _message)
            {
                if (will_log(_level)) {
                    internal_log(_message);
                }
                if (next) {
                    next->log(_level, _message);
                }
            }

            virtual void internal_log(const char* _message) = 0;

            bool will_log(LogLevel _level)
            {
                return (int)_level >= (int)level;
            }

            Logger* next;
            LogLevel level;
        };

        struct PrintLogger : public Logger
        {
            void internal_log(const char* _message) override
            {
                ::fprintf(stdout, "%s", _message);
            }
        };

        struct FileLogger : public Logger
        {
            FileLogger()
            {
                std::experimental::filesystem::create_directory("logs/");
                _file = fopen("logs/log.txt", "w");
            }

            ~FileLogger()
            {
                fclose(_file);
            }

            void internal_log(const char* _message) override
            {
                ::fprintf(_file, "%s", _message);
            }

            FILE* _file;
        };
    }

    internal::Logger* _globalLogger;

    void init()
    {
        _globalLogger = new internal::PrintLogger();
        _globalLogger->next = new internal::FileLogger();
    }

    void shutdown()
    {
        delete _globalLogger;
    }

    namespace internal {
        const size_t cMaxLogBufferSize = 512;
        const char* cRawLog = "RAW";
        const char* cInfoLog = "INFO";
        const char* cWarnLog = "WARNING";
        const char* cErrorLog = "ERROR";

        void format_log_msg(LogLevel _level, const char* _context, const char* _message, char* _out)
        {
            const char* logStr;
            switch (_level) {
            case LogLevel::cRaw:
                logStr = cRawLog;
                break;

            default:
            case LogLevel::cInfo:
                logStr = cInfoLog;
                break;

            case LogLevel::cWarn:
                logStr = cWarnLog;
                break;

            case LogLevel::cError:
                logStr = cErrorLog;
                break;
            }

            snprintf(_out, cMaxLogBufferSize, "[%s] %s: %s\n", _context, logStr, _message);
        }

        void raw_log(internal::Logger& _logger, LogLevel _level, const char* _context, const char* _message)
        {
            char buffer[cMaxLogBufferSize];
            format_log_msg(_level, _context, _message, buffer);
            _logger.log(_level, buffer);
        }
    }

    void log(LogLevel _level, const char* _context, const char* _message)
    {
        internal::raw_log(*_globalLogger, _level, _context, _message);
    }

    void _logv(LogLevel _level, const char* _context, const char* _format, va_list _args)
    {
        char buffer[internal::cMaxLogBufferSize];
        vsnprintf(buffer, internal::cMaxLogBufferSize, _format, _args);
        log(_level, _context, buffer);
    }

    void logf(LogLevel _level, const char* _context, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        _logv(_level, _context, _format, args);
        va_end(args);
    }

    void raw(const char* _context, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        _logv(LogLevel::cRaw, _context, _format, args);
        va_end(args);
    }

    void info(const char* _context, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        _logv(LogLevel::cInfo, _context, _format, args);
        va_end(args);
    }
    
    void warn(const char* _context, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        _logv(LogLevel::cWarn, _context, _format, args);
        va_end(args);
    }
    
    void error(const char* _context, const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        _logv(LogLevel::cError, _context, _format, args);
        va_end(args);
    }
}