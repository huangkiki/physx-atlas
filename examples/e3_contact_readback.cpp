// Original reading fragment for PhysX SDK 5.11.0 at the pinned source commit.
// Syntax/type checked only: no main(), SDK link, engine run or experiment.
// Call snapshotContactPair from an application's onContact callback. The caller
// owns scene setup, legal pair filtering, stable actor/shape IDs, step numbering,
// removed-object handling, callback synchronization and storage lifetime.
#include "PxFiltering.h"
#include "PxSimulationEventCallback.h"
#include <cstdint>
#include <vector>

namespace physx_atlas {

// Apply only to a pair which the application's filter already chose to solve.
// Do not use these flags to overwrite trigger, suppression or collision masks.
physx::PxPairFlags discreteContactReportFlags() {
    return physx::PxPairFlag::eCONTACT_DEFAULT |
           physx::PxPairFlag::eNOTIFY_TOUCH_FOUND |
           physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
           physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
           physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
}

struct ContactSnapshot {
    std::uint64_t step = 0;
    physx::PxPairFlags events;
    physx::PxContactPairFlags pairFlags;
    bool normalStreamAvailable = false;
    bool normalImpulsesAvailable = false;
    bool frictionStreamAvailable = false;
    std::vector<physx::PxContactPairPoint> normalPoints;
    std::vector<physx::PxContactPairFrictionAnchor> frictionAnchors;
};

ContactSnapshot snapshotContactPair(const physx::PxContactPair& pair,
                                    std::uint64_t publicStep) {
    ContactSnapshot result;
    result.step = publicStep;
    result.events = pair.events;
    result.pairFlags = pair.flags;
    result.normalStreamAvailable = pair.contactPatches && pair.contactPoints;
    // Read this SDK-generated flag; applications must not manufacture it.
    result.normalImpulsesAvailable = result.normalStreamAvailable &&
        pair.contactImpulses &&
        pair.flags.isSet(physx::PxContactPairFlag::eINTERNAL_HAS_IMPULSES);
    result.frictionStreamAvailable = pair.contactPatches && pair.frictionPatches;

    if (result.normalStreamAvailable) {
        result.normalPoints.resize(pair.contactCount);
        const auto count = pair.extractContacts(result.normalPoints.data(),
                                                pair.contactCount);
        result.normalPoints.resize(count);
    }
    if (result.frictionStreamAvailable) {
        // This pinned patch model has at most two anchors per patch.
        // Store them separately: they are not one-to-one with contact points.
        const auto capacity = physx::PxU32(pair.patchCount) * 2;
        result.frictionAnchors.resize(capacity);
        const auto count = pair.extractFrictionAnchors(
            result.frictionAnchors.data(), capacity);
        result.frictionAnchors.resize(count);
    }
    // All callback stream data have been copied. No actor/shape is dereferenced,
    // no SDK state is mutated and no borrowed stream pointer escapes.
    return result;
}

struct ReportedPointImpulse {
    physx::PxVec3 linear{0.0f};
    physx::PxVec3 momentAboutWorldOrigin{0.0f};
};

// Accumulate available reported point impulses, NOT a complete contact wrench.
// No normal impulse stream -> normal geometry is retained but not summed as a
// measurement. Absence of a friction stream (e.g. CCD) is NOT measured zero.
// Pure torsional solver rows are not included in the anchor point impulses.
ReportedPointImpulse reportedPointImpulse(const ContactSnapshot& sample,
                                         const physx::PxVec3& worldOrigin) {
    ReportedPointImpulse result;
    if (sample.normalImpulsesAvailable) {
        for (const auto& point : sample.normalPoints) {
            result.linear += point.impulse;
            result.momentAboutWorldOrigin +=
                (point.position - worldOrigin).cross(point.impulse);
        }
    }
    if (sample.frictionStreamAvailable) {
        for (const auto& anchor : sample.frictionAnchors) {
            result.linear += anchor.impulse;
            result.momentAboutWorldOrigin +=
                (anchor.position - worldOrigin).cross(anchor.impulse);
        }
    }
    // Retain the snapshot's availability metadata alongside this partial sum.
    // Divide by the matching public-step duration only after selecting actor
    // sign and verifying event coverage. This function does not aggregate CCD.
    return result;
}

}  // namespace physx_atlas
