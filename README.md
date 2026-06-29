# rmdev

rmdev: RoboMaster Development， RoboMaster 电控开发库。

## 特点

- C++20 模块 + 头文件两种导入方式
- 可按模块/驱动启用，减少无用编译
- 与 `emdevif` 深度协同，适配嵌入式开发场景

## 依赖

- CMake >= 3.28
- 支持 C++20 的编译器
- `emdevif`（必须先加入工程）

## 编码规范

本项目遵循统一编码规范（覆盖 emdevif 与 rmdev 全部子模块）：
- 集成环境内：[`../docs/coding-style.md`](../docs/coding-style.md)（相对本仓库根目录）
- 独立查看：https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/coding-style.md

## 模块组成

子模块

* [rmdev_math](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_math.git) 数学库
* [rmdev_control_algorithm](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_control_algorithm.git) 控制算法
* [rmdev_kinematic_solution](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_kinematic_solution.git)
  运动学解算
* [rmdev_device_model](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_device_model.git) 设备模型
* [rmdev_ins](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_ins.git) 姿态解算
* [rmdev_message_manager](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_message_manager.git) 发布/订阅消息管理
* [rmdev_debug_assistance](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_debug_assistance.git) 调试辅助

驱动

* [rmdev_driver_BMI088](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_driver_BMI088.git) BMI088 驱动
* [rmdev_driver_DJIMotor](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_driver_DJIMotor) 大疆电机驱动

## 使用方法

首先，请参考 [emdevif](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/emdevif.git) 的文档，配置 emdevif 库。

完成后，文件结构将会变成：

```
project_root
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
├── emdevif_collection/
│   └── emdevif/
│       └── ...
└── ...
```

rmdev 的配置方式与 emdevif 的配置方法稍有不同。emdevif 的主要模块都在一个仓库内，但 rmdev 根据功能，将子模块拆分到了不同仓库内。
以下有两种方式配置：

### 1、手动拉取需要的子模块

我们推荐您先需要先确定需要使用哪些子模块，然后根据需要使用 `git clone` 或 `git submodule add` 将子模块加入您的工程中。

假如需要 `rmdev_math`、`rmdev_control_algorithm` 库，使用示例如下：

```Shell
mkdir rmdev

git submodule add https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_math.git rmdev/rmdev_math
git submodule add https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev_control_algorithm.git rmdev/rmdev_control_algorithm

touch rmdev/CMakeLists.txt  # 需要手动创建 CMakeLists.txt
```

这样，文件结构将会变成：

```
project_root
├── inc/
│   └── ...
├── src/
│   └── ...
├── CMakeLists.txt
├── emdevif_collection/
│   └── emdevif/
│       └── ...
├── rmdev/
│   ├── CMakeLists.txt
│   ├── rmdev_math/
│   └── rmdev_control_algorithm/
└── ...
```

手动创建的 CMakeLists.txt 示例：

```CMake
cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

if (NOT (TARGET emdevif))
    message(FATAL_ERROR "[${PROJECT_NAME}]: emdevif target not found! Please add emdevif as a subdirectory before adding rmdev.")
endif ()

add_subdirectory(rmdev_math)
add_subdirectory(rmdev_control_algorithm)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    emdevif

    # 将需要的子模块添加在此处
    rmdev_math
    rmdev_control_algorithm
)
```

### 2、直接拉取整个仓库

您也可以直接拉取本仓库，然后将它添加到 CMake 的子路径中。只是有一些 CMake 缓存变量需要您配置：

| CMake 缓存变量                | 类型     | 默认值  | 说明                    |
|---------------------------|--------|------|-----------------------|
| RMDEV_ENABLE_INS_MODULE   | Bool   | OFF  | 是否使用姿态解算模块            |
| RMDEV_ENABLED_DRIVER_LIST | String | `""` | 要使用的驱动列表。驱动名称之间使用分号分隔 |

说明：由于姿态解算库直接依赖 CMSISDSP，而驱动库往往依赖项较多，因此这两个模块设置变量用于开关。

## 测试

单元测试已迁移至模拟测试环境仓库 `rmdev-dev` 的 `tests/mock/` 目录，使用 GoogleTest 框架。详见 `rmdev-dev` 根目录 `AGENTS.md` 的模拟单元测试章节。
