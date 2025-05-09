/**
 * @file Serial.cppm
 * @module rmdev.connectivity.Serial
 * @author 杜以成
 * @date 2025-05-09
 * @brief 串口通信
 */

module;

#include <string_view>
#include <cstdint>
#include "rmdev/simplify_decl_macros.hpp"

import rmdev.connectivity.Connect;
import rmdev.connectivity.connect_method;

export module rmdev.connectivity.Serial;

export namespace rmdev {

class Serial : public Connect
{
public:
    Serial() = delete;

    DELETE_COPY_AND_MOVE(Serial);

public:
    Serial(const std::string_view name, const ConnectMethod tx, const ConnectMethod rx) : Connect(name, tx, rx) {}
};

}  // namespace rmdev
