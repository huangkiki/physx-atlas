// Original native API reading fragments; syntax/type checked, never linked/run.
// Source contracts and limitations: ../docs/batch-learning-data.md
#include "PxFiltering.h"
#include "PxDirectGPUAPI.h"

#include <cstddef>
#include <limits>
#include <stdexcept>

namespace physx_atlas {
using namespace physx;

// Application convention, not a built-in PhysX env_id:
// simulationFilterData.word0 = environment ID;
// word1 bit 0 = shared world, honored only for RIGID_STATIC objects.
// Assign every shape before insertion; this does not configure query filtering.
constexpr PxU32 kSharedStaticWorld = 1u;

PxFilterFlags environmentFilter(
    PxFilterObjectAttributes attributes0, PxFilterData data0,
    PxFilterObjectAttributes attributes1, PxFilterData data1,
    PxPairFlags& pairFlags, const void*, PxU32) {
    const bool shared0 = (data0.word1 & kSharedStaticWorld) != 0 &&
        PxGetFilterObjectType(attributes0) == PxFilterObjectType::eRIGID_STATIC;
    const bool shared1 = (data1.word1 & kSharedStaticWorld) != 0 &&
        PxGetFilterObjectType(attributes1) == PxFilterObjectType::eRIGID_STATIC;
    pairFlags = PxPairFlags();
    if (data0.word0 != data1.word0 && !shared0 && !shared1) {
        return PxFilterFlag::eSUPPRESS;
    }
    pairFlags = PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1)
                    ? PxPairFlag::eTRIGGER_DEFAULT : PxPairFlag::eCONTACT_DEFAULT;
    return PxFilterFlag::eDEFAULT;
}

// Use as PxSceneDesc::filterShader = environmentFilter before scene creation.
// This minimal policy does not enable contact reports or CCD. Extend pairFlags
// deliberately for those uses; retain E3's filtering/notification distinctions.

// Caller preconditions:
// - Direct GPU scene initialization is complete, no simulation is in flight,
//   and scene/topology cannot change throughout this operation.
// - Both pointers refer to live GPU allocations in the correct context/device.
// - indicesDevice contains count valid, current articulation GPU indices;
//   it is ordered by the requested output rows, not by numerical GPU index.
// - Capacity describes the actual allocation. Padding is not an observation;
//   the caller separately preserves each articulation's valid DOF count/map.
// - Producer/consumer event dependencies and buffer lifetime are managed by
//   the caller. A non-null finished event must complete before consumption.
// - A true return need not include asynchronous CUDA errors, and is NOT a CPU
//   readback. A null finished event waits for this operation, per native API.
bool copyJointPositions(
    const PxDirectGPUAPI& api, PxReal* positionsDevice,
    std::size_t capacityBytes, const PxArticulationGPUIndex* indicesDevice,
    PxU32 count, CUevent ready, CUevent finished) {
    if (count == 0) {
        return true; // No SDK operation.
    }
    if (!positionsDevice || !indicesDevice) {
        throw std::invalid_argument("GPU data and index buffers are required");
    }
    const std::size_t maxDofs = api.getArticulationGPUAPIMaxCounts().maxDofs;
    // Scene-wide maximum, even when requesting only a subset of articulations.
    if (maxDofs == 0 ||
        maxDofs > std::numeric_limits<std::size_t>::max() / sizeof(PxReal)) {
        throw std::invalid_argument("Expected a scene with joint DOFs and a valid stride");
    }
    const std::size_t strideBytes = maxDofs * sizeof(PxReal);
    if (count > std::numeric_limits<std::size_t>::max() / strideBytes ||
        capacityBytes < static_cast<std::size_t>(count) * strideBytes) {
        throw std::length_error("Device output is smaller than scene-wide padded layout");
    }
    return api.getArticulationData(
        positionsDevice, indicesDevice, PxArticulationGPUAPIReadType::eJOINT_POSITION,
        count, ready, finished);
}
} // namespace physx_atlas
