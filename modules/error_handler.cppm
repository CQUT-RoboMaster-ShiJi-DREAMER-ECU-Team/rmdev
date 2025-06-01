/**
 * @file error_handler.cppm
 * @module rmdev.error_handler
 * @author 杜以成
 * @date 2025-06-01
 * @brief 错误处理
 */

module;

#include <cstdint>
#include "rmdev/attributes_and_useful_macros.h"

export module rmdev.error_handler;

export namespace rmdev {

/**
 * 错误码
 */
enum class ErrorCode : std::int8_t {
    Success = 0,            ///< 成功
    UnknownError = -1,      ///< 未知错误
    InvalidArgument = -2,   ///< 无效参数
    OutOfMemory = -3,       ///< 内存不足
    NotFound = -4,          ///< 未找到
    PermissionDenied = -5,  ///< 权限被拒绝
    Timeout = -6,           ///< 超时
    AlreadyExists = -7,     ///< 已存在
    NotImplemented = -8,    ///< 未实现
    InternalError = -9      ///< 内部错误
};

RMDEV_NO_RETURN void faultHandler(const char* message);

}  // namespace rmdev
