#ifndef BASE_FILE_APPENDER_H
#define BASE_FILE_APPENDER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <condition_variable>

#include "buffer_concept.h"

namespace BaseTool::Log::Detail
{
    template<Buffer Buffer_T>
    class BaseFileAppender
    {
    public:
        explicit BaseFileAppender(const std::string &filePath);

        ~BaseFileAppender();

        void AddMsg(const std::string_view &);

        void Flush();

    private:
        void Work();

        std::thread work_;

        std::mutex writeMutex_;
        std::condition_variable writeCond_;
        std::condition_variable readCond_;

        Buffer_T writeBuffer_;
        Buffer_T readBuffer_;

        std::atomic_bool isFlush_ = false;
        std::atomic_bool isStop_ = false;
        std::ofstream file_;
    };

    template<Buffer Buffer_T>
    BaseFileAppender<Buffer_T>::BaseFileAppender(const std::string &path)
    {
        std::filesystem::path filePath(path);
        std::filesystem::path fileDir = filePath.parent_path();

        if (!exists(fileDir))
        {
            if (!std::filesystem::create_directory(fileDir))
            {
                throw std::runtime_error("Unable to create directory " + fileDir.string());
            }
        }

        file_.open(filePath.string(), std::ios::out | std::ios::app);
        if (!file_.is_open())
            throw std::runtime_error("Unable to open file " + filePath.string());
        work_ = std::thread(&BaseFileAppender<Buffer_T>::Work, this);
    }

    template<Buffer Buffer_T>
    BaseFileAppender<Buffer_T>::~BaseFileAppender()
    {
        isStop_.store(true);
        readCond_.notify_all();
        work_.join();
        file_.close();
    }

    template<Buffer Buffer_T>
    void BaseFileAppender<Buffer_T>::AddMsg(const std::string_view &msg)
    {
        std::unique_lock<std::mutex> lock(writeMutex_);
        writeCond_.wait(lock, [&] { return !writeBuffer_.IsFull(); });
        if (!writeBuffer_.TryAddMsg(msg))
        {
            throw std::runtime_error("Add a msg file");
        }
        readCond_.notify_one();
    }

    template<Buffer Buffer_T>
    void BaseFileAppender<Buffer_T>::Flush()
    {
        isFlush_.store(true);
        readCond_.notify_all();
    }

    template<Buffer Buffer_T>
    void BaseFileAppender<Buffer_T>::Work()
    {
        while (true)
        {
            int currentSize; {
                std::unique_lock<std::mutex> lock(writeMutex_);
                readCond_.wait(lock, [&] { return isStop_.load() || isFlush_.load() || writeBuffer_.IsFull(); });
                writeBuffer_.MoveDataToDesc(readBuffer_);
                currentSize = readBuffer_.Size();
            }

            if (!isStop_.load())
            {
                writeCond_.notify_all();
            }

            for (int i = 0; i < currentSize; ++i)
            {
                file_ << readBuffer_[i];
            }

            if (isFlush_.load())
            {
                file_.flush();
                isFlush_.store(false);
            }
            if (isStop_.load())
                return;
        }
    }
}

#endif
