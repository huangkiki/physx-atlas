// Original native API reading fragment. Syntax/type checked only; not linked/run.
// Fixed SDK identity and source references: ../docs/sensors-rendering.md
#include "PxScene.h"
#include "PxSceneLock.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <stdexcept>
#include <vector>

namespace physx_atlas {
using namespace physx;

// Only copied numeric values leave the query. No borrowed actor/shape pointers.
struct RayObservation {
    PxReal range;
    std::optional<PxVec3> positionWorld;
    std::optional<PxVec3> normalWorld;
};

// Implements the SDK callback, with per-call, read-only self-filter state.
class ExcludeActors final : public PxQueryFilterCallback {
public:
    explicit ExcludeActors(const std::vector<const PxRigidActor*>& actors)
        : actors_(actors) {}

    PxQueryHitType::Enum preFilter(const PxFilterData&, const PxShape*,
                                  const PxRigidActor* actor, PxHitFlags&) override {
        return std::find(actors_.begin(), actors_.end(), actor) == actors_.end()
                   ? PxQueryHitType::eBLOCK : PxQueryHitType::eNONE;
    }

    PxQueryHitType::Enum postFilter(const PxFilterData&, const PxQueryHit&,
                                   const PxShape*, const PxRigidActor*) override {
        return PxQueryHitType::eBLOCK; // Required override; POSTFILTER is not set.
    }

private:
    const std::vector<const PxRigidActor*>& actors_;
};

// Preconditions owned by caller:
// - SDK/scene/actors are alive; the latest simulate has completed fetchResults.
// - Manual scene-query update work (if selected) is complete.
// - worldFromSensor and query observe the SAME state: prevent another thread
//   advancing/mutating the scene between pose capture and this function.
// - Scene writers obey the lock protocol; excludedActors is immutable here and
//   includes every robot actor to ignore, not only the sensor's mounting link.
// - The caller records step/time, units, pose and visibility policy with result.
// This inner lock protects the query; it cannot repair an already stale pose.
// A nullopt means no accepted hit, not an invalid input or a zero range.
std::optional<RayObservation> castSensorRay(
    PxScene& scene, const PxTransform& worldFromSensor,
    const PxVec3& unitDirectionSensor, PxReal maxRange,
    const std::vector<const PxRigidActor*>& excludedActors,
    PxU32 visibilityMask) {
    if (!worldFromSensor.isValid() || !unitDirectionSensor.isFinite() ||
        !unitDirectionSensor.isNormalized() ||
        !std::isfinite(maxRange) || maxRange <= 0.0f) {
        throw std::invalid_argument("Expected valid pose, unit direction and positive finite range");
    }

    PxSceneReadLock lock(scene);
    ExcludeActors filter(excludedActors);
    // Mask zero bypasses hardcoded filtering. For nonzero masks the caller must
    // set the corresponding word0 bits on each shape's queryFilterData.
    const PxQueryFilterData queryFilter(
        PxFilterData(visibilityMask, 0, 0, 0),
        PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER);
    PxRaycastBuffer hits; // No touch buffer: closest accepted BLOCK.
    // Remove floating-point normalization drift after the frame rotation.
    const PxVec3 directionWorld =
        worldFromSensor.q.rotate(unitDirectionSensor).getNormalized();
    scene.raycast(worldFromSensor.p, directionWorld, maxRange, hits,
                  PxHitFlag::eDEFAULT, queryFilter, &filter);
    // No ANY_HIT, query cache, or geometry ANY_HIT; bool alone is not hasBlock.
    if (!hits.hasBlock) {
        return std::nullopt;
    }

    RayObservation result{hits.block.distance, std::nullopt, std::nullopt};
    if (hits.block.flags.isSet(PxHitFlag::ePOSITION)) {
        result.positionWorld = hits.block.position;
    }
    if (hits.block.flags.isSet(PxHitFlag::eNORMAL)) {
        result.normalWorld = hits.block.normal;
    }
    return result; // Values copied before lock, callback and query buffer die.
}
} // namespace physx_atlas
