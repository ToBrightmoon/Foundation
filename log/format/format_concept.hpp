#ifndef BASETOOL_FORMAT_CONCEPT_HPP
#define BASETOOL_FORMAT_CONCEPT_HPP

#include <concepts>
#include <string>

#include "log_base.hpp"

namespace BaseTool::Log::Detail
{
    template<typename T>
    concept Format = requires(T format, Common::LogMsg msg)
    {
        { format.Format(msg) }-> std::same_as<std::string>;
    };
}


#endif //BASETOOL_FORMAT_CONCEPT_HPP
