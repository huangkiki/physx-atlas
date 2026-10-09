// Native API reading example for pinned PhysX headers 5.11.0.
// Compile-only checked, not linked or run; no main(), model importer or task loop.
// All functions require ordinary CPU API access (no Direct GPU API), one owner,
// and no simulation in flight. The caller owns SDK objects/cache and must inspect
// the SDK error callback. bool results only report the explicit input checks here.
#include "PxArticulationJointReducedCoordinate.h"
#include "PxArticulationLink.h"
#include "PxArticulationReducedCoordinate.h"

#include <algorithm>
#include <vector>

using namespace physx;

// Caller has built links/shapes/mass properties. This joint belongs to robot;
// the frames locate the joint in its parent/child ACTOR frames, not COM/world.
// The finite gains and 0..4 cm travel illustrate fields, not validated tuning.
bool configureFingerSlide(PxArticulationReducedCoordinate& robot,
                          PxArticulationJointReducedCoordinate& finger,
                          const PxTransform& parentJointFrame,
                          const PxTransform& childJointFrame)
{
    if (robot.getScene() ||
        &finger.getParentArticulationLink().getArticulation() != &robot ||
        !parentJointFrame.isValid() || !childJointFrame.isValid())
        return false;

    finger.setJointType(PxArticulationJointType::ePRISMATIC);
    finger.setParentPose(parentJointFrame);
    finger.setChildPose(childJointFrame);
    for (PxU32 i = 0; i < PxArticulationAxis::eCOUNT; ++i)
        finger.setMotion(static_cast<PxArticulationAxis::Enum>(i),
                         PxArticulationMotion::eLOCKED);
    finger.setMotion(PxArticulationAxis::eX, PxArticulationMotion::eLIMITED);
    finger.setLimitParams(PxArticulationAxis::eX, PxArticulationLimit(0.0f, 0.04f));
    // This flag applies to the WHOLE articulation, not only this finger.
    robot.setArticulationFlag(PxArticulationFlag::eDRIVE_LIMITS_ARE_FORCES, true);
    // Legacy maxForce path, still present in this revision. The constructor sets
    // envelope.maxEffort=0; enabling a nonzero envelope would change limit semantics.
    finger.setDriveParams(PxArticulationAxis::eX,
                         PxArticulationDrive(4000.0f, 80.0f, 20.0f,
                                             PxArticulationDriveType::eFORCE));
    finger.setMaxJointVelocity(PxArticulationAxis::eX, 0.1f);
    finger.setJointPosition(PxArticulationAxis::eX, 0.04f); // Initialization only.
    finger.setJointVelocity(PxArticulationAxis::eX, 0.0f);
    finger.setDriveTarget(PxArticulationAxis::eX, 0.04f, false);
    finger.setDriveVelocity(PxArticulationAxis::eX, 0.0f, false);
    return true;
}

// Call after adding the articulation to Scene, at a completed-fetch boundary.
// The application supplies a rate-limited trajectory; setters do not generate one.
bool commandFinger(PxArticulationJointReducedCoordinate& finger,
                   PxReal target, PxReal targetVelocity)
{
    if (!finger.getParentArticulationLink().getArticulation().getScene() ||
        finger.getJointType() != PxArticulationJointType::ePRISMATIC ||
        finger.getMotion(PxArticulationAxis::eX) != PxArticulationMotion::eLIMITED ||
        !PxIsFinite(target) || !PxIsFinite(targetVelocity))
        return false;
    const PxArticulationLimit limit = finger.getLimitParams(PxArticulationAxis::eX);
    if (target < limit.low || target > limit.high)
        return false;
    finger.setDriveTarget(PxArticulationAxis::eX, target);
    finger.setDriveVelocity(PxArticulationAxis::eX, targetVelocity);
    return true;
}

// Cache must have been created for the current robot structure. efforts is the
// COMPLETE low-level DOF-order vector, with N for slides and N m for rotations.
// Inputs persist until changed; call with an explicit all-zero vector to clear.
// This channel adds to configured drives: choose that combination intentionally.
bool setJointEfforts(PxArticulationReducedCoordinate& robot,
                     PxArticulationCache& cache,
                     const std::vector<PxReal>& efforts)
{
    if (!robot.getScene() || efforts.size() != robot.getDofs() || !cache.jointForce)
        return false;
    if (!std::all_of(efforts.begin(), efforts.end(),
                     [](PxReal value) { return PxIsFinite(value); }))
        return false;
    std::copy(efforts.begin(), efforts.end(), cache.jointForce);
    robot.applyCache(cache, PxArticulationCacheFlag::eFORCE);
    return true;
}

// State must already be current: after non-cache state setters, the caller first
// calls robot.updateKinematic(ePOSITION/eVELOCITY), as appropriate.
PxTransform toolWorldPose(const PxArticulationLink& link,
                         const PxTransform& toolInActor)
{
    return link.getGlobalPose() * toolInActor;
}

// The returned row block maps to link COM velocity in world coordinates.
// Cache ownership/version are caller preconditions. No IK solve is performed.
bool readJacobianBlock(PxArticulationReducedCoordinate& robot,
                       const PxArticulationLink& link,
                       PxArticulationCache& cache,
                       PxU32& firstRow, PxU32& rows, PxU32& columns)
{
    firstRow = rows = columns = 0;
    if (!robot.getScene() || &link.getArticulation() != &robot)
        return false;
    const PxU32 index = link.getLinkIndex();
    const bool fixedBase = robot.getArticulationFlags().isSet(PxArticulationFlag::eFIX_BASE);
    if (fixedBase && index == 0)
        return false; // The fixed root's six rows are absent.
    robot.commonInit();
    robot.computeDenseJacobian(cache, rows, columns);
    firstRow = 6 * (index - (fixedBase ? 1 : 0));
    return cache.denseJacobian && firstRow + 6 <= rows;
}

// Preconditions: row is a valid six-row block; column < columns. worldOffset is
// p_tool - p_COM in world coordinates, obtained from current, matching-frame data.
PxVec3 toolLinearJacobianColumn(const PxArticulationCache& cache,
                               PxU32 row, PxU32 column, PxU32 columns,
                               const PxVec3& worldOffset)
{
    const PxReal* j = cache.denseJacobian;
    const PxVec3 linear(j[row * columns + column],
                        j[(row + 1) * columns + column],
                        j[(row + 2) * columns + column]);
    const PxVec3 angular(j[(row + 3) * columns + column],
                         j[(row + 4) * columns + column],
                         j[(row + 5) * columns + column]);
    return linear + angular.cross(worldOffset);
}
