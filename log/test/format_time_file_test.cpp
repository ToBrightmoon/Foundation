#include "log.hpp"
#include "format.hpp"
#include "msg_buffer.hpp"
#include "format_time_file_appender.hpp"

int main()
{
    using namespace BaseTool::Log;

    Logger<Detail::FormatTimeFileAppender<Detail::LogMsgBuffer<1024> >, Detail::BaseFormat> logger(
        "../../logfile/log.log", "%Y_%m_%d-%H:%M:%S");

    logger.SetLevel(Common::Level::info);
    logger.Log(Common::Level::error, "{}", "test");
    logger.Log(Common::Level::error, "{}", "arga1", "arga2");
    for (int i = 0; i < 10000; ++i)
    {
        std::string msg = std::format("sss {}", i % 7);
        logger.Log(static_cast<Common::Level>(i % 7), msg.c_str());
        if (i > 0 && i % 100 == 0)
        {
            // logger.Flush();
            // std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
