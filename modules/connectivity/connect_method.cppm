/**
 * @file connect_method.cppm
 * @module rmdev.connectivity.connect_method
 * @author 杜以成
 * @date 2025-05-09
 * @brief 通信方式
 */

module;

#include <cstdint>

export module rmdev.connectivity.connect_method;

export namespace rmdev {

/**
 * 通信方式
 */
enum class ConnectMethod : std::uint8_t {
    BLOCK = 0U,  ///< 阻塞式
    INTERRUPT,   ///< 中断式
    DMA          ///< DMA 模式
};

}  // namespace rmdev
