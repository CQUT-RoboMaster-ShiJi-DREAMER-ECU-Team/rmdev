/**
 * @file vofa.cppm
 * @module rmdev.debugAssistance.vofa
 * @author DuYicheng
 * @date 2025-10-17
 * @brief VOFA 上位机适配
 */

module;

#include <cstdint>

#include <initializer_list>
#include <span>
#include <algorithm>

export module rmdev.debugAssistance.vofa;

export import emdevif.errorHandler;

export namespace rmdev::debug_assistance::vofa {

class JustFloat
{
public:
    static constexpr std::initializer_list<uint8_t> frame_tail{0x00, 0x00, 0x80, 0x7f};

    static std::span<uint8_t> appendFrameTail(std::span<float> buffer) noexcept
    {
        const auto p_end = reinterpret_cast<uint8_t*>(buffer.data() + buffer.size());
        std::ranges::copy(frame_tail, p_end - frame_tail.size());

        return {reinterpret_cast<uint8_t*>(buffer.data()), buffer.size()};
    }
};

}  // namespace rmdev::debug_assistance::vofa
