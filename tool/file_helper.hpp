#ifndef FILE_HELPER_HPP
#define FILE_HELPER_HPP

#include <filesystem>

namespace BaseTool::Tool
{
    class FileHelper
    {
    public:
        static std::string GetFileName(const std::string_view &filePath)
        {
            std::filesystem::path path{filePath};
            return path.filename().string();
        }

        // 获取文件扩展名
        static std::string GetFileExtension(const std::string_view &filePath)
        {
            std::filesystem::path path{filePath};
            return path.extension().string();
        }

        // 获取文件绝对路径
        static std::string GetAbsolutePath(const std::string_view &filePath)
        {
            std::filesystem::path path{filePath};
            return path.string();
        }

        // 获取文件所在目录的绝对路径
        static std::string GetDirectory(const std::string_view &filePath)
        {
            std::filesystem::path path{filePath};
            return path.parent_path().string();
        }

        // 获取文件所在目录的绝对路径去掉拓展名
        static std::string GetAbsoluteFileName(const std::string_view &filePath)
        {
            std::filesystem::path path{filePath};
            return path.parent_path().string() + "/" + path.stem().string();
        }
    };
}

#endif //FILE_HELPER_HPP
