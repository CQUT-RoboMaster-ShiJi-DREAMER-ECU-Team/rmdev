/**
 * @file msg_queue_implements.cppm
 * @module rmdev.thread.message_queue:implements
 * @author 杜以成
 * @date 2025-06-18
 * @brief 消息队列的实现
 */

module;

#include <cstddef>

#include "FreeRTOS.h"
#include "queue.h"

#include "rmdev/byte_type.h"

#include "rmdev/fault_handler.hpp"

export module rmdev.thread.message_queue:implements;
import :interface;

export namespace rmdev {

template<typename Type, std::size_t length>
class MessageQueue : public MessageQueueInterface<MessageQueue<Type, length>>
{
public:
    using DataType = Type;

    MessageQueue() : handle(xQueueCreateStatic(length, sizeof(DataType), storge_memory, &queue_buffer))
    {
        if (handle == nullptr) {
            RMDEV_FAULT_HANDLER("Failed to create message queue!");
        }
    }

private:
    ErrorCode push_impl(const bool in_isr, const Type& data, const std::size_t timeout)
    {
        if (in_isr) {
            BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
            const auto ret = xQueueSendFromISR(handle, &data, &xHigherPriorityTaskWokenByPost);

            ErrorCode final_ret;

            if (ret == pdTRUE) {
                final_ret = ErrorCode::Success;
            }
            else {
                final_ret = ErrorCode::Full;
            }

            if (xHigherPriorityTaskWokenByPost) {
                portYIELD_FROM_ISR(xHigherPriorityTaskWokenByPost);
            }

            return final_ret;
        }
        else {
            const auto ret = xQueueSend(handle, &data, timeout);
            if (ret == pdTRUE) {
                return ErrorCode::Success;
            }
            else {
                return ErrorCode::Full;
            }
        }
    }

    ErrorCode pop_impl(const bool in_isr, Type& data, const std::size_t timeout)
    {
        if (in_isr) {
            BaseType_t xHigherPriorityTaskWokenByPost = pdFALSE;
            const auto ret = xQueueReceiveFromISR(handle, &data, &xHigherPriorityTaskWokenByPost);

            ErrorCode final_ret;

            if (ret == pdTRUE) {
                final_ret = ErrorCode::Success;
            }
            else {
                final_ret = ErrorCode::Empty;
            }

            if (xHigherPriorityTaskWokenByPost) {
                portYIELD_FROM_ISR(xHigherPriorityTaskWokenByPost);
            }

            return final_ret;
        }
        else {
            const auto ret = xQueueReceive(handle, &data, timeout);
            if (ret == pdTRUE) {
                return ErrorCode::Success;
            }
            else {
                return ErrorCode::Empty;
            }
        }
    }

    ErrorCode pop_impl(const bool in_isr)
    {
        Type data;
        return pop_impl(in_isr, &data, 0U);
    }

    ErrorCode peek_impl(bool in_isr, Type& data, std::size_t timeout)
    {
        if (in_isr) {
            const auto ret = xQueuePeekFromISR(handle, &data);

            ErrorCode final_ret;

            if (ret == pdTRUE) {
                final_ret = ErrorCode::Success;
            }
            else {
                final_ret = ErrorCode::Empty;
            }

            return final_ret;
        }
        else {
            const auto ret = xQueuePeek(handle, &data, timeout);
            if (ret == pdTRUE) {
                return ErrorCode::Success;
            }
            else {
                return ErrorCode::Empty;
            }
        }
    }

    [[nodiscard]] std::size_t size_impl() const
    {
        return uxQueueMessagesWaiting(handle);
    }

    [[nodiscard]] std::size_t remainSize_impl() const
    {
        return uxQueueSpacesAvailable(handle);
    }

private:
    QueueHandle_t handle;
    StaticQueue_t queue_buffer{};
    ubyte_t storge_memory[length]{};
};

}  // namespace rmdev
