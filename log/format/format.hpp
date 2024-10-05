#ifndef BASETOOL_FORMAT_HPP
#define BASETOOL_FORMAT_HPP

#include <format>
#include <iostream>
#include <sstream>

#include "log_base.hpp"

namespace BaseTool::Log::Detail
{
    class BaseFormat
    {
    public:
        BaseFormat() = default;

        ~BaseFormat() = default;

        std::string Format(const Common::LogMsg &msg)
        {
            auto local_time = std::localtime(&msg.time);

            format_time_stream_ << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

            auto res = std::format("[{}][{}][{}][{}]\n", format_time_stream_.str(), Common::level_to_str(msg.level),
                                   msg.thread_id, msg.msg);

            format_time_stream_.str("");
            format_time_stream_.clear();

            return res;
        }

    private:
        std::ostringstream format_time_stream_;
    };
}

#endif //BASETOOL_FORMAT_HPP
