#include <string>
#include <fstream>
#include <mutex>

#include "json.hpp"


namespace BaseTool::Param
{
    using json = nlohmann::json;

    enum class ErrorCode
    {
        NO_ERROR = 0,
        FILE_OPEN_FAIL = 1,
        FILE_TO_JSON_FAIL = 2,
        NO_INIT = 3,
        NOT_CONTAIN_KEY = 4,
        PARSE_FAIL = 5
    };

    class ParamParse
    {
    public:
        ParamParse() = default;

        ~ParamParse();

        ParamParse(const ParamParse &) = delete;

        ParamParse(ParamParse &&) = delete;

        ParamParse &operator=(const ParamParse &) = delete;

        ParamParse &operator=(ParamParse &&) = delete;

        ErrorCode Init(const std::string &);

        ErrorCode UnInit();

        template<class T>
        ErrorCode GetValue(const std::string &, T &value);

        template<class T>
        ErrorCode GetValueByPath(const std::string &, T &value);

    private:
        bool inited_ = false;
        std::recursive_mutex mutex_;
        json jsonParse_;
    };

    ParamParse::~ParamParse()
    {
        UnInit();
    }

    ErrorCode ParamParse::Init(const std::string &configFilePath)
    {
        std::lock_guard<std::recursive_mutex> guard(mutex_);

        std::ifstream configStream(configFilePath);

        if (!configStream.is_open())
        {
            return ErrorCode::FILE_OPEN_FAIL;
        }

        try
        {
            configStream >> jsonParse_;
        } catch (const json::parse_error &e)
        {
            configStream.close();
            return ErrorCode::FILE_TO_JSON_FAIL;
        }

        inited_ = true;
        return ErrorCode::NO_ERROR;
    };

    ErrorCode ParamParse::UnInit()
    {
        std::lock_guard<std::recursive_mutex> guard(mutex_);

        if (!inited_) return ErrorCode::NO_ERROR;

        inited_ = false;
        return ErrorCode::NO_ERROR;
    }

    template<class T>
    ErrorCode ParamParse::GetValue(const std::string &key, T &value)
    {
        std::lock_guard<std::recursive_mutex> guard(mutex_);

        if (!inited_)
            return ErrorCode::NO_INIT;

        if (!jsonParse_.contains(key))
        {
            return ErrorCode::NOT_CONTAIN_KEY;
        }

        try
        {
            value = jsonParse_.at(key).get<T>();
        } catch (const std::exception &e)
        {
            return ErrorCode::PARSE_FAIL;
        }

        return ErrorCode::NO_ERROR;
    }

    template<class T>
    ErrorCode ParamParse::GetValueByPath(const std::string &key, T &value)
    {
        std::lock_guard<std::recursive_mutex> guard(mutex_);

        if (!inited_)
            return ErrorCode::NO_INIT;

        if (!jsonParse_.contains(json::json_pointer(key)))
        {
            return ErrorCode::NOT_CONTAIN_KEY;
        }

        try
        {
            value = jsonParse_.at(json::json_pointer(key)).get<T>();
        } catch (const std::exception &e)
        {
            return ErrorCode::PARSE_FAIL;
        }

        return ErrorCode::NO_ERROR;
    }
}
