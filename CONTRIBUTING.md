# 贡献指南

`rmdev` 是一个**独立维护**的聚合库（RoboMaster 电控开发库），但它的开发 / 测试集成环境是 [`rmdev-dev`](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev)。

## 在哪里开发

`rmdev` 本身是聚合仓库，`modules/` 和 `drivers/` 下的每个目录都是独立的 git submodule，各有独立仓库、版本历史与 CI：

- **子模块（算法/模型/驱动）逻辑改动**：优先在对应独立仓库（如 `rmdev_math.git`、`rmdev_driver_BMI088.git`）提交并验证。
- **聚合层调整**：仅当涉及 `rmdev` 自身的 `CMakeLists.txt`、`.gitmodules` 或子模块组织结构时，才在 `rmdev` 仓库提交。
- **集成验证**：改动合并后，同步到 `rmdev-dev` 并运行宿主单元测试，确保集成路径无回归。

## 多仓库协作流程

完整流程见 `rmdev-dev` 根目录 [`AGENTS.md`](https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/AGENTS.md) 的「多仓库协作流程」章节。

## 编码规范

统一编码规范位于 `rmdev-dev` 仓库的 `docs/coding-style.md`：

- 集成环境内：`../docs/coding-style.md`（相对本仓库根目录）
- 独立查看：https://github.com/CQUT-RoboMaster-ShiJi-DREAMER-ECU-Team/rmdev-dev/blob/main/docs/coding-style.md

裁剪、接口与验证要求见 [`AGENTS.md`](AGENTS.md)。
