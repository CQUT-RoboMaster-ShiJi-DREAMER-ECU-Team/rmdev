/**
 * @file error_handler.cpp
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include "rmdev/attributes_and_useful_macros.h"

module rmdev.error_handler;

namespace rmdev {

RMDEV_NO_RETURN void faultHandler(const char* message)
{
    while (true) {
    }
}

}  // namespace rmdev
