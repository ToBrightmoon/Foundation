#ifndef MSG_BUFFER_HPP
#define MSG_BUFFER_HPP

#include <array>

namespace BaseTool::Log::Detail
{
    template<size_t Capacity>
    class LogMsgBuffer
    {
        using Iterator = typename std::array<std::string, Capacity>::iterator;

    public:
        LogMsgBuffer() = default;

        ~LogMsgBuffer() = default;

        [[nodiscard]] bool IsFull() const;

        bool TryAddMsg(const std::string_view &msg) noexcept;

        [[nodiscard]] size_t Size() const noexcept;

        void MoveDataToDesc(LogMsgBuffer<Capacity> &desc) noexcept;

        std::string &operator[](size_t idx);

        const std::string &operator[](size_t idx) const;

        std::string &at(size_t idx);

        [[nodiscard]] const std::string &at(size_t idx) const;

    private:
        Iterator Begin() noexcept;

        Iterator End() noexcept;

        void Clear() noexcept;

        std::atomic<size_t> curr_size_ = 0;
        std::array<std::string, Capacity> buffer_;
    };

    template<size_t Capacity>
    bool LogMsgBuffer<Capacity>::IsFull() const
    {
        return curr_size_.load() == Capacity;
    }

    template<size_t Capacity>
    bool LogMsgBuffer<Capacity>::TryAddMsg(const std::string_view &msg) noexcept
    {
        if (IsFull()) return false;

        buffer_[curr_size_.load()] = msg;
        curr_size_.store(curr_size_.load() + 1);

        return true;
    }

    template<size_t Capacity>
    size_t LogMsgBuffer<Capacity>::Size() const noexcept
    {
        return curr_size_.load();
    }

    template<size_t Capacity>
    void LogMsgBuffer<Capacity>::MoveDataToDesc(LogMsgBuffer<Capacity> &desc) noexcept
    {
        desc.curr_size_.store(curr_size_.load());
        std::move(Begin(), End(), desc.Begin());
        Clear();
    }

    template<size_t Capacity>
    const std::string &LogMsgBuffer<Capacity>::at(size_t idx) const
    {
        return buffer_.at(idx);
    }

    template<size_t Capacity>
    std::string &LogMsgBuffer<Capacity>::at(size_t idx)
    {
        return buffer_.at(idx);
    }

    template<size_t Capacity>
    std::string &LogMsgBuffer<Capacity>::operator[](size_t idx)
    {
        return buffer_.at(idx);
    }

    template<size_t Capacity>
    const std::string &LogMsgBuffer<Capacity>::operator[](size_t idx) const
    {
        return buffer_.at(idx);
    }

    template<size_t Capacity>
    typename LogMsgBuffer<Capacity>::Iterator LogMsgBuffer<Capacity>::Begin() noexcept
    {
        return buffer_.begin();
    }

    template<size_t Capacity>
    typename LogMsgBuffer<Capacity>::Iterator LogMsgBuffer<Capacity>::End() noexcept
    {
        return buffer_.end();
    }

    template<size_t Capacity>
    void LogMsgBuffer<Capacity>::Clear() noexcept
    {
        buffer_.fill("");
        curr_size_.store(0);
    }
}

#endif //MSG_BUFFER_HPP
