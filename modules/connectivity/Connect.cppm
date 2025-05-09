/**
 * @file Connect.cppm
 * @module rmdev.connectivity.Connect
 * @author 杜以成
 * @date 2025-05-09
 * @brief 通信基类
 */

module;

#include <string_view>

import rmdev.connectivity.connect_method;
import rmdev.lib.InitOnce;

export module rmdev.connectivity.Connect;

export namespace rmdev {

/**
 * 通信基类
 */
class Connect
{
public:
    Connect() = delete;

    /**
     * 通信类构造函数
     * @param name 端口名称
     * @param tx 发送方式
     * @param rx 接收方式
     */
    Connect(const std::string_view name, const ConnectMethod tx, const ConnectMethod rx)
        : port_name(name), tx_method(tx), rx_method(rx)
    {
    }

protected:
    const std::string_view port_name;  ///< 端口名称
    const ConnectMethod tx_method{};   ///< 发送方式
    const ConnectMethod rx_method{};   ///< 接收方式
    InitOnce<void*> handle{};          ///< 句柄
};

}  // namespace rmdev
