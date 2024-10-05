#ifndef FORMAT_TIME_FILE_APPENDER_HPP
#define FORMAT_TIME_FILE_APPENDER_HPP

#include "base_file_appender.hpp"
#include "file_helper.hpp"
#include "time_helper.hpp"

namespace BaseTool::Log::Detail
{
    template<Buffer LoggerBuffer>
    class FormatTimeFileAppender
    {
    public:
        explicit FormatTimeFileAppender(const std::string &filePath);

        FormatTimeFileAppender(const std::string_view &filePath, const std::string_view &timePatter);

        ~FormatTimeFileAppender() = default;

        void AddMsg(const std::string_view &);

        void Flush();

    private:
        BaseFileAppender<LoggerBuffer> fileAppender_;
    };

    template<Buffer LoggerBuffer>
    FormatTimeFileAppender<LoggerBuffer>::FormatTimeFileAppender(const std::string &filePath): fileAppender_(
        Tool::FileHelper::GetAbsoluteFileName(filePath) + "_" +
        Tool::TimeHelper::GetFormatTime("%Y_%m_%d_%H_%M_%S") +
        Tool::FileHelper::GetFileExtension(filePath)
    )
    {
    }

    template<Buffer LoggerBuffer>
    FormatTimeFileAppender<LoggerBuffer>::FormatTimeFileAppender(const std::string_view &filePath,
                                                                 const std::string_view &timePatter): fileAppender_(
        Tool::FileHelper::GetAbsoluteFileName(filePath.data()) + "_" +
        Tool::TimeHelper::GetFormatTime(timePatter) +
        Tool::FileHelper::GetFileExtension(filePath.data())
    )
    {
    }

    template<Buffer LoggerBuffer>
    void FormatTimeFileAppender<LoggerBuffer>::AddMsg(const std::string_view &msg)
    {
        fileAppender_.AddMsg(msg);
    }

    template<Buffer LoggerBuffer>
    void FormatTimeFileAppender<LoggerBuffer>::Flush()
    {
        fileAppender_.Flush();
    }
}

#endif //FORMAT_TIME_FILE_APPENDER_HPP
