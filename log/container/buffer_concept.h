#ifndef BUFFER_CONCEPT_H
#define BUFFER_CONCEPT_H

namespace BaseTool::Log::Detail
{
    template<typename T>
    concept Buffer = requires(T buffer, T desc, const std::string_view &str, size_t idx){
        { buffer.IsFull() } -> std::same_as<bool>;
        { buffer.TryAddMsg(str) } -> std::same_as<bool>;
        { buffer.Size() } -> std::same_as<size_t>;

        buffer.MoveDataToDesc(desc);
        buffer[idx];
        buffer.at(idx);
    };
}

#endif //BUFFER_CONCEPT_H
