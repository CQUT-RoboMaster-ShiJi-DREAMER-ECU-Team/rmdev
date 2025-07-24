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

static void serialPrint(const char* buffer, const uint16_t size)
{
    for (uint16_t i = 0; i < size; ++i) {
        while (HAL_UART_GetState(&huart6) != HAL_UART_STATE_READY) {
        }

        while (HAL_UART_Transmit(&huart6, (const uint8_t*)&buffer[i], sizeof(char), HAL_MAX_DELAY) != HAL_OK) {
        }
    }
}

static void testPrintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char printf_buffer[1000] = {'\0'};

    vsprintf(printf_buffer, format, args);

    serialPrint(printf_buffer, strlen(printf_buffer));

    va_end(args);
}

static void testEntry(void)
{
    test_main();
}

/**
 * stm32f407ighx 平台的测试函数入口
 * @note 由于 platform_test 库不是链接到 stm32cubemx 的，所以要在这里再包装一层
 */
void stm32f407ighx_test_main(void)
{
    const rmdev_test_Callbacks callbacks = {.printfCallback = testPrintf, .testEntryCallback = testEntry};
    rmdev_test_framework_main("\r\n", &callbacks, NULL);
}
