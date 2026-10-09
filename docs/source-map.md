# physx-atlas 固定版本源码入口

阅读基线：`da950a3537927784951853c66618036f332ca0ce`。以下入口已核对官方 Git 树与文件内容身份；不是全仓审查或运行验收记录。

本阶段先理解引擎架构、建模、步进、控制、接触/求解、传感器/渲染、性能与扩展。独立实验、基准、训练和评分暂不开展，后续复用 DexLab。

| 源码文件 | 阅读目的 |
|---|---|
| [README.md](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/README.md) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/include/foundation/PxPhysicsVersion.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/foundation/PxPhysicsVersion.h) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/include/PxSceneDesc.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxSceneDesc.h) | 追踪构建、步进、数据更新与生命周期 |
| [physx/include/PxScene.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxScene.h) | 追踪构建、步进、数据更新与生命周期 |
| [physx/include/PxRigidBody.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxRigidBody.h) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/include/PxRigidDynamic.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxRigidDynamic.h) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/include/PxArticulationReducedCoordinate.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxArticulationReducedCoordinate.h) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/source/physx/src/NpScene.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpScene.cpp) | 追踪构建、步进、数据更新与生命周期 |
| [physx/source/lowleveldynamics/src/DyDynamics.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/src/DyDynamics.cpp) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/source/lowleveldynamics/src/DyTGSDynamics.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/src/DyTGSDynamics.cpp) | 核对对象职责、数据布局、参数与版本约定 |
| [physx/snippets/snippethelloworld/SnippetHelloWorld.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/snippets/snippethelloworld/SnippetHelloWorld.cpp) | 理解官方最小使用顺序；本轮不执行 |
| [physx/documentation/fetchcontent/FetchContent.md](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/documentation/fetchcontent/FetchContent.md) | 核对对象职责、数据布局、参数与版本约定 |

## E1 增补阅读入口

[课程](modeling-state-time.md)给出对应结论与行号；文件身份清单记录于 [sources.json](sources.json)。头文件依赖的编译检查不等于这些文件的全部功能已审查。

| 源码文件 | 阅读目的 |
|---|---|
| [physx/include/PxPhysics.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxPhysics.h) | 对象工厂、Shape 创建引用、资源生命周期 |
| [physx/include/PxShape.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxShape.h) | simulation/query/trigger 标志、局部位姿与质量更新边界 |
| [physx/include/PxRigidActor.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxRigidActor.h) | Actor 世界位姿、附着引用计数、动态形状限制 |
| [physx/include/PxArticulationJointReducedCoordinate.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxArticulationJointReducedCoordinate.h) | parent/child joint frame、关节类型与 motion |
| [physx/include/PxArticulationLink.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxArticulationLink.h) | 低层 link 索引与非 cache 更新后的速度查询 |
| [physx/include/PxArticulationFlag.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxArticulationFlag.h) | cache mask，尤其 eALL 不包含的控制和外力字段 |
| [physx/include/common/PxTolerancesScale.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/common/PxTolerancesScale.h) | 尺度参考值与单位转换的区别 |
| [physx/include/foundation/PxTransform.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/foundation/PxTransform.h) | 初始化、字段布局、旋转平移与复合顺序 |
| [physx/include/foundation/PxQuat.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/foundation/PxQuat.h) | xyzw 构造顺序、单位轴与弧度 |
| [physx/include/extensions/PxRigidBodyExt.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/extensions/PxRigidBodyExt.h) | 质量/惯量扩展接口与参数契约 |
| [physx/include/extensions/PxSerialization.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/extensions/PxSerialization.h) | 对象依赖闭包、格式兼容与序列化阶段限制 |
| [physx/include/cooking/PxCooking.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/cooking/PxCooking.h) | triangle/convex descriptor 到 collision mesh 的 cooking 路径 |
| [physx/include/geometry/PxBoxGeometry.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/geometry/PxBoxGeometry.h) | 半边长与合法性 |
| [physx/include/geometry/PxTriangleMeshGeometry.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/geometry/PxTriangleMeshGeometry.h) | 三角网格几何与缩放引用 |
| [physx/include/geometry/PxConvexMeshGeometry.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/geometry/PxConvexMeshGeometry.h) | 凸网格几何与缩放引用 |
| [physx/source/physxextensions/src/ExtRigidBodyExt.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physxextensions/src/ExtRigidBodyExt.cpp) | 计算质心/对角化惯量与点速度的实现 |
| [physx/source/physx/src/NpRigidDynamic.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpRigidDynamic.cpp) | setGlobalPose/velocity 的运行阶段与 Direct GPU 限制 |
| [physx/source/physx/src/NpArticulationReducedCoordinate.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpArticulationReducedCoordinate.cpp) | root global pose 返回链与注释方向核对 |
| [physx/include/geometry/PxCapsuleGeometry.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/geometry/PxCapsuleGeometry.h) | X 轴和 halfHeight 的原生定义 |
| [physx/include/foundation/PxSimpleTypes.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/foundation/PxSimpleTypes.h) | PxReal 的实际 float 定义 |
| [physx/include/geometry/PxMeshScale.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/geometry/PxMeshScale.h) | mesh scaling 与刚性 Transform 的区别 |
| [physx/source/physx/src/NpSceneFetchResults.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpSceneFetchResults.cpp) | 完成事件、fetch 前后处理与阶段切换 |
| [physx/include/solver/PxSolverDefs.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/solver/PxSolverDefs.h) | articulation 轴、motion、joint type 枚举 |
| [physx/source/physx/src/NpRigidActorTemplate.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpRigidActorTemplate.h) | attachShape 的实际检查与 ShapeManager 调用 |
| [physx/source/physx/src/NpShape.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpShape.cpp) | Shape 标志修改限制与 SDF 材料分支 |
| [physx/source/physx/src/NpShapeManager.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpShapeManager.cpp) | 实际附着与重复 Shape 检查 |
| [physx/snippets/snippetsdf/SnippetSDF.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/snippets/snippetsdf/SnippetSDF.cpp) | SDF triangle mesh 动态 Actor、cooking 与 CPU/GPU 示例分支（未运行） |

## E2 增补阅读入口

[控制、机器人与任务接口](control-robotics.md)按 setter、数据映射、驱动参数准备与冲量限幅追踪。下列固定文件已做身份核对与对应段落阅读，不代表整个文件或引擎已全面审查。

| 源码文件 | 阅读目的 |
|---|---|
| [physx/include/extensions/PxD6Joint.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/extensions/PxD6Joint.h) | D6 隐式 drive、angular model、相对目标与 forceLimit |
| [physx/include/extensions/PxRevoluteJoint.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/extensions/PxRevoluteJoint.h) | extension revolute velocity motor 与 drive flag |
| [physx/include/extensions/PxJoint.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/extensions/PxJoint.h) | extension joint/constraint 的公共接口边界 |
| [physx/include/PxConstraint.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxConstraint.h) | constraint drive force/impulse flag 与版本迁移标记 |
| [physx/include/PxSimulationEventCallback.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxSimulationEventCallback.h) | onContact/onAdvance 的阶段、写入与线程契约 |
| [physx/source/physx/src/NpArticulationJointReducedCoordinate.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpArticulationJointReducedCoordinate.cpp) | drive target/velocity/params 的实际入口检查与转发 |
| [physx/source/simulationcontroller/src/ScArticulationJointCore.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/simulationcontroller/src/ScArticulationJointCore.cpp) | 关节轴到有效 DOF target 数组映射、dirty 更新 |
| [physx/source/lowleveldynamics/include/DyArticulationJointCore.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/include/DyArticulationJointCore.h) | drive、target、joint frame 在低层 core 的存储 |
| [physx/source/lowleveldynamics/src/DyFeatherstoneArticulation.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/src/DyFeatherstoneArticulation.cpp) | 构造 drive、上限缩放/限幅与 dense Jacobian 布局 |
| [physx/source/lowleveldynamics/src/DyFeatherstoneForwardDynamic.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/src/DyFeatherstoneForwardDynamic.cpp) | articulation 动力学接口衔接；完整步进待 E3 |
| [physx/source/lowleveldynamics/include/DyFeatherstoneArticulationUtils.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/include/DyFeatherstoneArticulationUtils.h) | 低层空间量/关节工具入口；完整数值展开待 E3 |
| [physx/source/physxextensions/src/ExtD6Joint.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physxextensions/src/ExtD6Joint.cpp) | D6 参数保存、相对 joint frame 误差与 drive row |
| [physx/source/physx/src/NpRigidBodyTemplate.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpRigidBodyTemplate.h) | 四种 force mode 的逆质量/逆惯量和累加器分支 |
| [physx/source/simulationcontroller/src/ScBodyCore.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/simulationcontroller/src/ScBodyCore.cpp) | Body core 与 simulation 状态接口的衔接 |
| [physx/snippets/snippetarticulationrc/SnippetArticulation.cpp](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/snippets/snippetarticulationrc/SnippetArticulation.cpp) | 原生 robot link/joint frame、额外 D6 约束与目标提交（未运行） |
| [physx/source/lowleveldynamics/include/DyFeatherstoneArticulation.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/include/DyFeatherstoneArticulation.h) | articulation 与 CPU/GPU 共享驱动数据入口 |
| [physx/source/physx/src/NpArticulationJointReducedCoordinate.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/physx/src/NpArticulationJointReducedCoordinate.h) | scSetDrive/Target/Velocity 向 core 转发 |
| [physx/include/PxArticulationMimicJoint.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/include/PxArticulationMimicJoint.h) | mimic ratio/offset/compliance 的公开入口 |
| [physx/source/lowleveldynamics/shared/DyCpuGpuArticulation.h](https://github.com/NVIDIA-Omniverse/PhysX/blob/da950a3537927784951853c66618036f332ca0ce/physx/source/lowleveldynamics/shared/DyCpuGpuArticulation.h) | force/acceleration 隐式 drive 公式、冲量计算与 envelope 限幅 |
