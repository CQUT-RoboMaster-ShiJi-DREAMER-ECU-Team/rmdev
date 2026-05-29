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
├── test/                       # 测试入口与测试套件
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
本模块沿用 `emdevif` 的编码风格（`emdevif/docs/emdevif-coding-style.md`），核心约定一致：

- 类型名 `PascalCase`，函数名 `camelCase`，变量/命名空间 `snake_case`
- 宏 `UPPER_SNAKE_CASE`，外部可见宏统一 `RMDEV_*` 前缀
- 控制语句强制大括号，行宽 120，4 空格缩进
- `#include` 最小化，禁止循环依赖

## 构建系统

### 关键 CMake 变量

| 变量 | 默认 | 说明 |
|---|---|---|
| `RMDEV_ENABLE_TESTS` | `OFF` | 测试开关，开启后构建 `rmdev_unit_test` 库 |
| `RMDEV_ENABLE_INS_MODULE` | `OFF` | INS 姿态解算模块开关，需 CMSIS-DSP |
| `RMDEV_ENABLED_DRIVER_LIST` | `""` | 启用的驱动列表（分号分隔），如 `"BMI088;DJIMotor"` |
| `RMDEV_TEST_IGNORE_NAME_LIST` | `""` | 测试忽略列表（分号分隔） |

### 依赖链
- `rmdev` → `emdevif`（必须先 `add_subdirectory`）
- `rmdev_ins` → CMSIS-DSP（由 CubeMX 或用户提供）
- `rmdev` 测试入口自动调用 `emdevif` 测试入口（`rmdev_testEntry` 内部先执行 emdevif 测试）

## 总体开发约束
- 保持可裁剪特性：基础模块默认启用，INS/驱动按需启用。
- 新能力优先模块化拆分，避免在聚合层堆积板级细节。
- 新驱动接入遵循 `RMDEV_ENABLED_DRIVER_LIST` 机制。

## 子模块约束（合并说明）

### rmdev_control_algorithm
- 算法保持平台无关与可复用。
- 数值稳定性优先，避免隐式单位/范围错误。

### rmdev_debug_assistance
- 调试能力不得侵入核心控制路径。
- 输出格式变更需考虑既有上位机工具兼容。

### rmdev_device_model
- 模型字段语义稳定（名称、单位、方向约定）。
- 保持"驱动写模型、算法读模型"的职责边界。

### rmdev_ins
- 时间步长、坐标系、姿态表示约定必须保持一致。
- 依赖 CMSIS-DSP 的改动需评估精度与性能。

### rmdev_kinematic_solution
- 输入输出坐标系、符号约定必须清晰。
- 参数（轮距、半径、排布）需显式可配置。

### rmdev_math
- 兼容 `RMDEV_MATH_USE_CMSIS_DSP` 开关两条路径。
- 浮点误差阈值策略改动要评估下游影响。

### rmdev_message_manager
- 保持发布订阅接口简洁可追踪。
- 线程安全策略明确，避免隐式竞态。

### rmdev_driver_BMI088
- 保持 SPI/I2C 双路径配置一致性。
- 通过 `emdevif_peripheral/timeline` 访问底层能力。

### rmdev_driver_DJIMotor
- 保持单位语义正确（`mp-units`）。
- 不绕过 `emdevif_peripheral` 直接耦合板级实现。

### rmdev/test
- 测试命名应兼容 `RMDEV_TEST_IGNORE_NAME_LIST`。
- 优先覆盖公共接口与跨模块集成路径。
- 详见 `test/README.md`。

## 扩展指引

### 新增功能模块
1. 在独立仓库中开发模块，遵循 `rmdev_<name>` 命名
2. 在本仓库 `modules/` 下以 git submodule 引入
3. 在根 `CMakeLists.txt` 中添加 `add_subdirectory` 和 `target_link_libraries`
4. 在 AGENTS.md 中新增子模块约束小节
5. 如果需按条件启用，选择合适的机制：布尔开关（类似 INS）或列表（类似驱动）

### 新增驱动
1. 在独立仓库中开发驱动，遵循 `rmdev_driver_<Name>` 命名
2. 在本仓库 `drivers/` 下以 git submodule 引入
3. 用户通过 `RMDEV_ENABLED_DRIVER_LIST` 启用
4. 驱动通过 `emdevif_peripheral` 访问硬件，不直接耦合板级

## 验证要求
- 检查 `RMDEV_ENABLE_INS_MODULE` 与驱动列表的条件构建路径。
- 关键模块改动后，至少验证对应测试或最小集成编译。
- 与 emdevif 接口交互的改动，需验证 `EMDEVIF_USE_CPP_MODULES=ON/OFF` 两条路径。
- 变更涉及子模块子仓库时，还需在该子仓库独立验证。

## 参考资料
- 编码规范（上游）：`emdevif/docs/emdevif-coding-style.md`
- 测试说明：`test/README.md`
- 完整使用指南：`README.md`
- 子模块清单：`.gitmodules`
