# PhysX Atlas

Understand PhysX through native APIs, physics concepts and versioned source code.

[Sim Atlas home](https://github.com/huangkiki/sim-atlas) · [中文](README.md) · [Introductory guide](docs/guide.md) · [Curriculum](docs/curriculum.md) · [Source map](docs/source-map.md) · [Versions](docs/versions.md) · [Roadmap](docs/roadmap.md) · [Project tracker](https://github.com/users/huangkiki/projects/2)

Part of **Sim Atlas**, an independent community learning series with two complete planned tracks: applications (modeling, control, robotics, sensing and data) and principles/source (dynamics, contact, solvers, integration and extensions).

The initial guide and pinned source map are available in Chinese.

[E1: Modeling, frames, state and time](docs/modeling-state-time.md) now covers native object ownership, collision assets/cooking, units and quaternion conventions, actor/shape/COM frames, inertia, articulation cache indexing and reset, and the simulate/fetchResults boundary. The [native C++ reading example](examples/e1_state_boundary.cpp) and [validation record](docs/validation/e1.md) distinguish source/compile-only checks from runtime validation. E1 delivers A1/A2 and the relevant B0/B4 foundations; full solver/integration analysis remains in E3. The rest of the course is in development.

This phase prioritizes understanding engine subsystems. Minimal snippets support explanation and are explicitly marked when unexecuted. No new simulation campaigns, benchmarks, training or scoring are included; later experimental material will reuse [DexLab](https://github.com/huangkiki/Dexlab) with its original version and workload boundaries.

[Pinned upstream source](https://github.com/NVIDIA-Omniverse/PhysX/tree/da950a3537927784951853c66618036f332ca0ce) · [Attribution](THIRD_PARTY.md)
