# AGENTS.md

## 模块定位
`rmdev` 是 RoboMaster 电控开发库聚合层，通过 git submodule 统一组织算法、模型与驱动模块，并依赖 `emdevif` 提供底层抽象。

## 仓库结构

```
rmdev/
├── modules/                    # 功能模块（各为独立 submodule 仓库）
│   ├── rmdev_math/             #   数学库
│   ├── rmdev_control_algorithm/ #   控制算法
│   ├── rmdev_kinematic_solution/ # 运动学解算
│   ├── rmdev_device_model/     #   设备模型
│   ├── rmdev_ins/              #   姿态解算（需 CMSIS-DSP）
│   ├── rmdev_message_manager/  #   发布订阅消息管理
│   └── rmdev_debug_assistance/ #   调试辅助
├── drivers/                    # 驱动模块（各为独立 submodule 仓库）
│   ├── rmdev_driver_BMI088/    #   BMI088 IMU 驱动
│   └── rmdev_driver_DJIMotor/  #   大疆电机驱动
└── OpenOCD-BoardConfigFileTemplates/  # OpenOCD 板级配置模板
```

### 元仓库性质
`rmdev` 本身是一个聚合仓库，`modules/` 和 `drivers/` 下的每个目录都是独立的 git submodule（见 `.gitmodules`），各有独立的版本历史、Issues 和 CI。修改子模块代码时应当在其独立仓库中进行，本仓库只做聚合层调整。

### 构建参与方式
并非所有子模块都默认参与编译：
- **基础模块**（math / control_algorithm / kinematic_solution / device_model / debug_assistance / message_manager）：总是编译并链接
- **INS 模块**：通过 `RMDEV_ENABLE_INS_MODULE=ON` 启用，依赖 CMSIS-DSP
- **驱动模块**：通过 `RMDEV_ENABLED_DRIVER_LIST` 按需启用

## 编码规范

本项目开发需遵循统一编码规范，详见：

[https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/emdevif-coding-style.md](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/emdevif-coding-style.md)

## 构建系统

### 关键 CMake 变量

| 变量 | 默认 | 说明 |
|---|---|---|
| `RMDEV_ENABLE_INS_MODULE` | `OFF` | INS 姿态解算模块开关，需 CMSIS-DSP |
| `RMDEV_ENABLED_DRIVER_LIST` | `""` | 启用的驱动列表（分号分隔），如 `"BMI088;DJIMotor"` |

### 依赖链
- `rmdev` → `emdevif`（必须先 `add_subdirectory`）
- `rmdev_ins` → CMSIS-DSP（由 CubeMX 或用户提供）

### 测试

单元测试已迁移至宿主环境，测试文件位于开发环境仓库 [rmdev-dev](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev) 根目录下的 `tests/mock/emdevif/`，使用 GoogleTest 框架。详见 `rmdev-dev` 根 `AGENTS.md` 的宿主单元测试章节。

## 参考资料
- 编码规范：[https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/emdevif-coding-style.md](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/emdevif-coding-style.md)
- 开发环境（集成与测试）：[https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev)
- 完整使用指南：`README.md`
- 子模块清单：`.gitmodules`
