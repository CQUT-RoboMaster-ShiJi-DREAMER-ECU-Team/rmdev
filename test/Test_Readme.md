# rmdev 测试

## 单元测试

使用 [rmdev_test_framework](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_test_framework) 进行单元测试。

要运行单元测试，请在 CMake 的配置中，增加宏 `RMDEV_ENABLE_TESTS`，然后通过宏
`RMDEV_UNIT_TEST_PLATFORM` 指定运行测试的平台。支持的值有：
```
"stm32f407ighx"
```
