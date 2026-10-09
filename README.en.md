# PhysX Atlas

Understand PhysX through native APIs, physics concepts and versioned source code.

[Sim Atlas home](https://github.com/huangkiki/sim-atlas) · [中文](README.md) · [Introductory guide](docs/guide.md) · [Curriculum](docs/curriculum.md) · [Source map](docs/source-map.md) · [Versions](docs/versions.md) · [Roadmap](docs/roadmap.md) · [Project tracker](https://github.com/users/huangkiki/projects/2)

Part of **[Sim Atlas](https://github.com/huangkiki/sim-atlas)**, an independent community learning series with two complete planned tracks: applications (modeling, control, robotics, sensing and data) and principles/source (dynamics, contact, solvers, integration and extensions).

The initial guide and pinned source map are available in Chinese.

[E1: Modeling, frames, state and time](docs/modeling-state-time.md) now covers native object ownership, collision assets/cooking, units and quaternion conventions, actor/shape/COM frames, inertia, articulation cache indexing and reset, and the simulate/fetchResults boundary. The [native C++ reading example](examples/e1_state_boundary.cpp) and [validation record](docs/validation/e1.md) distinguish source/compile-only checks from runtime validation. E1 delivers A1/A2 and the relevant B0/B4 foundations; the CPU contact solver/integration analysis is now covered by E3.

[E2: Control, robotics and task interfaces](docs/control-robotics.md) covers force/impulse modes, implicit articulation drives and motor envelopes, D6 distinctions, robot asset/DOF mapping, FK and tool-point Jacobians, application-owned IK, and task/control scheduling. Its [native C++ example](examples/e2_control_robotics.cpp) is compile-only checked; [E2 validation](docs/validation/e2.md) lists the source and execution boundaries. A3/A5/A7 source lessons are delivered; host importer execution and robot runtime behavior remain unvalidated. The rest of the course is in development.

[E3: Contact, solvers and force observations](docs/contact-solvers.md) traces native filtering and offsets, rigid/compliant material combination, constraint preparation, CPU PGS/TGS iteration and integration, and normal versus friction-anchor impulse readback. It explains the missing CCD friction stream and pure torsional-row contribution, so reported point impulses are not presented as a complete contact wrench. The [C++ fragment](examples/e3_contact_readback.cpp) is syntax/type checked only; [E3 validation](docs/validation/e3.md) records scope and limitations. A4 and B1–B5 source lessons are delivered, with GPU implementation details and runtime evidence still outside this slice.

[E4: Sensing, scene queries and debug visualization](docs/sensors-rendering.md) covers native raycast/sweep/overlap filtering, hit validity and overflow, query updates and sampling boundaries, incoming joint forces and acceleration validity, and the distinct responsibilities of debug primitives, PVD/OmniPVD and host RGB/depth/segmentation pipelines. The [C++ ray-query fragment](examples/e4_scene_query.cpp) is syntax/type checked only; [E4 validation](docs/validation/e4.md) records the fixed source and unexecuted scope. A6 is delivered as a source lesson; sensor, renderer, GPU and headless runtime behavior remain unvalidated.

This phase prioritizes understanding engine subsystems. Minimal snippets support explanation and are explicitly marked when unexecuted. No new simulation campaigns, benchmarks, training or scoring are included; later experimental material will reuse [DexLab](https://github.com/huangkiki/Dexlab) with its original version and workload boundaries.

[Pinned upstream source](https://github.com/NVIDIA-Omniverse/PhysX/tree/da950a3537927784951853c66618036f332ca0ce) · [Attribution](THIRD_PARTY.md)
