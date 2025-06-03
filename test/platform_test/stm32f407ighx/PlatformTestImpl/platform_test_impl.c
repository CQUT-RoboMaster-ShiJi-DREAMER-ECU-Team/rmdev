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

void rmdev_test_printf(const char* format, ...)
{
    static char buffer[256] = {0};

    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);

    va_end(args);

    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
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
