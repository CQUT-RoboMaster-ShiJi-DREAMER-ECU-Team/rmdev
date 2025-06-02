/**
 * @file error_handler.cpp
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include "rmdev/attributes_and_useful_macros.h"

module rmdev.error_handler;

/**
 * 致命错误处理函数（此函数是弱链接的，用户可自己定义一个同名函数以覆盖此函数的定义）
 * @param message 错误消息
 */
RMDEV_WEAK RMDEV_NO_RETURN void rmdevFaultHandler(const char* message)
{
    RMDEV_UNUSED(message);

    while (true) {
    }
}

namespace rmdev {

}  // namespace rmdev
