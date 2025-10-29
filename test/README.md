# rmdev 测试

见 [emdevif](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif.git)
中的测试文档说明。此项目的测试与 emdevif 的类似，并且依赖 emdevif 的测试内容（会自动调用 emdevif 的测试入口函数）。
区别是 `此处 test_main.cpp` 中声明的函数是 `extern "C" void rmdev_testEntry(void)`，同样不提供外部头文件，需要用户手动在需要处声明。
