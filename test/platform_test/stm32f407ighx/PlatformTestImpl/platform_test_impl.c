/**
 * @file platform_test_impl.c
 * @author 杜以成
 * @date 2025-06-03
 * @brief 平台测试 - 函数的实现
 */

#include "platform_test_impl.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "main.h"
#include "usart.h"
#include "rmdev_test_framework.h"

extern void test_main(void);

static char printf_buffer[8000] = {0};

void rmdev_test_printf(const char* format, ...)
{
    printf_buffer[0] = '\0';

    va_list args;
    va_start(args, format);

    vsprintf(printf_buffer, format, args);

    va_end(args);

    const uint32_t len = strlen(printf_buffer);

    HAL_Delay(20 * len);

    while (HAL_UART_Transmit(&huart6, (const uint8_t*)printf_buffer, len, HAL_MAX_DELAY) != HAL_OK) {
        HAL_Delay(50);
    }

    while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY) {
        HAL_Delay(1);
    }
}

void rmdev_test_Delay(const unsigned int ms)
{
    HAL_Delay(ms);
}

void rmdev_test_testItem(void)
{
    test_main();
}

/**
 * stm32f407ighx 平台的测试函数入口
 * @note 由于 platform_test 库不是链接到 stm32cubemx 的，所以要在这里再包装一层
 */
void stm32f407ighx_test_main(void)
{
    rmdev_test_framework_main();
}
