#ifndef BASETOOL_LOG_HPP
#define BASETOOL_LOG_HPP

#include "log_base.hpp"
#include "log_detail_concepts.hpp"

namespace BaseTool::Log
{
    template<Detail::Appender Appender,Detail::Format Format>
    class Logger
    {
    public:
        Logger() = default;

        explicit Logger(const std::string &filename);

        template<typename... Args>
        explicit Logger(Args &&...);

        ~Logger() = default;

        Logger(const Logger &) = delete;

        Logger &operator=(const Logger &) = delete;

        Logger(Logger &&) = delete;

        Logger &operator=(Logger &&) = delete;

        void Log(Common::Level, const std::string_view &);

        template<typename... Args>
        void Log(Common::Level, std::string_view, Args &&...);

        void Flush();

        void SetLevel(Common::Level);

    private:
        std::atomic<Common::Level> level_ = Common::Level::debug;

        Format format_;

        Appender appender_;
    };

    template<Detail::Appender Appender, Detail::Format Format>
    Logger<Appender, Format>::Logger(const std::string &filename): appender_(filename)
    {
    }

    template<Detail::Appender Appender, Detail::Format Format>
    template<typename... Args>
    Logger<Appender, Format>::Logger(Args &&... args): appender_(std::forward<Args>(args)...)
    {
    }

    template<Detail::Appender Appender, Detail::Format Format>
    void Logger<Appender, Format>::Log(BaseTool::Log::Common::Level level, const std::string_view &msg)
    {
        if (level < level_.load()) return;

        appender_.AddMsg(format_.Format(Common::LogMsg(level, msg)));
    }

    template<Detail::Appender Appender, Detail::Format Format>
    template<typename... Args>
    void Logger<Appender, Format>::Log(Common::Level level, std::string_view fmt, Args &&... args)
    {
        try
        {
            auto msg = std::vformat(fmt, std::make_format_args(args...));
            Log(level, msg.c_str());
        } catch (const std::exception &e)
        {
            Log(Common::Level::error, std::string_view(e.what()));
        }
    }

    template<Detail::Appender Appender, Detail::Format Format>
    void Logger<Appender, Format>::SetLevel(BaseTool::Log::Common::Level level)
    {
        level_.store(level);
    }

    template<Detail::Appender Appender, Detail::Format Format>
    void Logger<Appender, Format>::Flush()
    {
        appender_.Flush();
    }
}


#endif //BASETOOL_LOG_HPP
