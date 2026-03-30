# AGENTS.md

## 模块定位
`rmdev` 是 RoboMaster 电控开发库聚合层，统一组织算法、模型与驱动模块，并依赖 `emdevif` 提供底层抽象。

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
- 保持“驱动写模型、算法读模型”的职责边界。

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

## 验证要求
- 检查 `RMDEV_ENABLE_INS_MODULE` 与驱动列表的条件构建路径。
- 关键模块改动后，至少验证对应测试或最小集成编译。
