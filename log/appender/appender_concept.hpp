#ifndef BASETOOL_APPENDER_CONCEPT_HPP
#define BASETOOL_APPENDER_CONCEPT_HPP

namespace BaseTool::Log::Detail
{
    template<class T>
    concept Appender = requires(T appender,const std::string_view msg)
    {
        appender.AddMsg(msg);
        appender.Flush();
    };
}

#endif //BASETOOL_APPENDER_CONCEPT_HPP
