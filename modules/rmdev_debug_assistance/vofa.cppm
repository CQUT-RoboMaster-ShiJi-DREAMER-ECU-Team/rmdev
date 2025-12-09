/**
 * @file vofa.cppm
 * @brief VOFA 上位机适配
 */

module;

#include <cstdint>

#include <initializer_list>
#include <span>
#include <algorithm>
#include <bit>

export module rmdev.debugAssistance.vofa;

export import emdevif.errorHandler;
import emdevif.endian;

export namespace rmdev::debug_assistance::vofa {

class JustFloat
{
public:
    static constexpr std::initializer_list<uint8_t> frame_tail{0x00, 0x00, 0x80, 0x7f};

    static std::span<float> toLittleEndian(const std::span<float> data) noexcept
    {
        static_assert(std::endian::native == std::endian::big || std::endian::native == std::endian::little,
                      "Not support mixed endian.");

        if constexpr (std::endian::native == std::endian::big) {
            for (auto& v : data) {
                v = emdevif::byteSwap(v);
            }
        }

        return data;
    }

    static std::span<uint8_t> appendFrameTail(const std::span<float> buffer, const std::size_t data_count) noexcept
    {
        const auto origin_buffer_length = data_count;
        const auto p_end = reinterpret_cast<uint8_t*>(&buffer[data_count]);

        std::ranges::copy(frame_tail.begin(), frame_tail.end(), p_end);

        return {reinterpret_cast<uint8_t*>(buffer.data()), origin_buffer_length * sizeof(float) + frame_tail.size()};
    }

    static std::span<uint8_t> processData(const std::span<float> buffer, const std::size_t data_count) noexcept
    {
        toLittleEndian(buffer);
        return appendFrameTail(buffer, data_count);
    }
};

}  // namespace rmdev::debug_assistance::vofa
