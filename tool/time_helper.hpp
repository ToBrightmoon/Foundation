#ifndef TIME_HELPER_HPP
#define TIME_HELPER_HPP

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

namespace BaseTool::Tool
{
    class TimeHelper
    {
    public:
        static std::string GetFormatTime(const std::string_view &timeFormat)
        {
            const std::time_t time{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};

            std::ostringstream formatStream;

            auto local_time = std::localtime(&time);

            formatStream << std::put_time(local_time, timeFormat.data());

            return formatStream.str();
        }
    };
} // namespace BaseTool::Tool


#endif // TIME_HELPER_HPP
