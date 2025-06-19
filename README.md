# rmdev

rmdev: RoboMaster Development

适用于 RoboMaster 电控的开发库。

## 特点

* 使用 C++20 的模块特性。
* 支持多种平台（但还在开发中）。
* 可以通过设置 CMake 宏来选择性关闭不需要的功能，被关闭的功能不会被编译进库中。

## 依赖

* C++20 以上以及模块的支持。
* CMake 3.28 以上版本。

## 使用方法

### 安装

使用 `git clone` 或 `git submodule`（推荐）将本库克隆到你的项目中，然后通过
`add_subdirectory` 将其添加到你的 CMake 项目中。

### 配置

建议新建一个名为 `rmdev_config.cmake` 的文件，并在其中设置需要的宏。以下是配置模板，
您可以将其复制到配置文件中并根据需要修改：

```cmake
# rmdev_config.cmake

# 需要启用 CMake 的 C++20 模块支持
if (NOT CMAKE_CXX_SCAN_FOR_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif ()

# 以下配置与平台相关，请根据实际情况选择（目前不支持修改，仍在开发）
set(RMDEV_USE_CMSIS_DSP ON)    # 是否使用 CMSIS DSP 库
set(RMDEV_USE_STM32CUBEMX ON)  # 是否使用 STM32CubeMX 库

# 以下配置与功能相关，可以视情况关闭一些功能
#set(RMDEV_DISABLE_MATH)                # 取消注释以禁用数学库
#set(RMDEV_DISABLE_MATRIX)              # 取消注释以禁用矩阵库
#set(RMDEV_DISABLE_THREAD)              # 取消注释以禁用线程库
#set(RMDEV_DISABLE_CONTROL_ALGORITHM)   # 取消注释以禁用控制算法库
#set(RMDEV_DISABLE_KINEMATIC_SOLUTION)  # 取消注释以禁用运动学解算库

# 注：目前连接通信库未开发完毕，必须禁用
set(RMDEV_DISABLE_CONNECTIVITY)         # 取消注释以禁用连接通信库

add_subdirectory(<path_to_rmdev>)  # 替换为实际的 rmdev 路径
```

然后在您的 CMakeLists.txt 中使用 `include` 把这个配置文件包含进来。

最后，通过 `target_link_libraries` 将 `rmdev` 库链接到您的目标中，即可开始使用。

## 测试

详见 [test/Test_Readme.md](./test/Test_Readme.md)。
