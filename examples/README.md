# C++ 阅读片段

[E1：状态边界](e1_state_boundary.cpp) 对应[建模、坐标、状态与时间](../docs/modeling-state-time.md)。原生 C++，没有 Python 包装器，没有 `main`；调用方负责初始化与资源生命周期，前置条件写在文件中。

本阶段不运行 SDK 或新仿真实验。语法/类型检查结果与未覆盖范围见[验证记录](../docs/validation/e1.md)。

[E2：驱动与机器人](e2_control_robotics.cpp) 对应[控制专题](../docs/control-robotics.md)：平移关节配置、target/持久 effort、工具 FK 与 Jacobian 行映射。候选参数未作运行验证；源码/编译检查见 [E2 记录](../docs/validation/e2.md)。
