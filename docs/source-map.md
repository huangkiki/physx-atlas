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
