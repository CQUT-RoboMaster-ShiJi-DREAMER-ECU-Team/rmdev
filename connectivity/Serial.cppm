/**
 * @file Serial.cppm
 * @module rmdev.connectivity.Serial
 * @author 杜以成
 * @date 2025-05-09
 * @brief 串口通信
 */

module;

#include <string_view>
#include "rmdev/simplify_decl_macros.hpp"

import rmdev.lib.InitOnce;

export module rmdev.connectivity.Serial;

export namespace rmdev {

class Serial
{
public:
    Serial() = delete;

    DELETE_COPY_AND_MOVE(Serial);

public:
    explicit Serial(const std::string_view name) : port_name(name) {}

private:
    const std::string_view port_name;  ///< 串口名称

    InitOnce<void*> handle{};          ///< 串口句柄
};

}  // namespace rmdev
