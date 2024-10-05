#ifndef BASETOOL_LOG_BASE_HPP
#define BASETOOL_LOG_BASE_HPP

#include <chrono>
#include <string>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
#else
#include <unistd.h>
#endif

namespace BaseTool::Log::Common
{
    // 获取线程 ID
    inline std::string getThreadId()
    {
#ifdef _WIN32
        // Windows 平台：使用 GetCurrentThreadId() 获取线程 ID
        DWORD threadId = GetCurrentThreadId();
        return std::to_string(threadId);
#else
        // POSIX 平台（如 Linux、macOS）：使用 syscall 获取线程 ID
        return std::to_string(static_cast<size_t>(::syscall(SYS_gettid)));
#endif
    }

    enum class Level
    {
        trace = 0,
        debug = 1,
        info = 2,
        warn = 3,
        error = 4,
        fatal = 5,
        off = 6
    };

    struct LogMsg
    {
        explicit LogMsg(Level level = Level::debug, const std::string_view &msg = "");

        Level level{Level::debug};
        const std::string_view msg;
        const std::string thread_id = getThreadId();
        const std::time_t time{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
    };

    inline LogMsg::LogMsg(Level logLevel, const std::string_view &logMsg): level(logLevel), msg(logMsg)
    {
    }

    static std::string_view level_to_str(Level level)
    {
        static std::string_view level_str_table[] = {"trace", "debug", "info", "warn", "error", "fatal", "off"};
        int size = static_cast<int>(level);
        return size <= 6 ? level_str_table[size] : "";
    }
}

#endif //BASETOOL_LOG_BASE_HPP
