// Original reading fragment for the pinned PhysX 5.11 headers.
// Syntax/type checked only: no SDK build, link, simulation or CUDA execution.
// This is a row-prep function, not a complete constraint plugin. A caller still
// supplies a PxConstraintConnector, constant-block lifetime/dirty handling,
// actor/COM conversions, createConstraint/release, and runtime validation.
#include "PxConstraint.h"

namespace physx_atlas {
using namespace physx;

// Anchors are in the body COM frames accepted by PxConstraintSolverPrep.
// For a static/null actor (whose supplied transform is identity), store the
// anchor in world coordinates instead. The axis stays fixed in world space.
// Use one consistent unit system; SI units are shown below.
struct AxisSpringData {
    PxVec3 anchorA;
    PxVec3 anchorB;
    PxVec3 worldUnitAxis;
    PxReal restCoordinate; // m: n dot (pA - pB) at zero spring error
    PxReal stiffness;      // N/m: force spring, not acceleration spring
    PxReal damping;        // N*s/m
    PxReal impulseCap;     // N*s: raw symmetric impulse limit, NOT a force cap
};

PxU32 prepareAxisSpring(
    Px1DConstraint* rows, PxVec3p& bodyAWorldOffset, PxU32 maxConstraints,
    PxConstraintInvMassScale& invMassScale, const void* constantBlock,
    const PxTransform& bodyAToWorld, const PxTransform& bodyBToWorld,
    bool /*useExtendedLimits*/, PxVec3p& anchorAWorld, PxVec3p& anchorBWorld)
{
    // All mutable temporary state is local; no scene API access in this shader.
    invMassScale = PxConstraintInvMassScale(1.0f, 1.0f, 1.0f, 1.0f);
    bodyAWorldOffset = PxVec3(0.0f);
    anchorAWorld = bodyAToWorld.p;
    anchorBWorld = bodyBToWorld.p;
    if (!rows || !constantBlock || maxConstraints == 0)
        return 0;

    const auto& data = *static_cast<const AxisSpringData*>(constantBlock);
    if (!data.anchorA.isFinite() || !data.anchorB.isFinite()
        || !data.worldUnitAxis.isFinite()
        || PxAbs(data.worldUnitAxis.magnitudeSquared() - 1.0f) > 1.0e-4f
        || !PxIsFinite(data.restCoordinate)
        || !PxIsFinite(data.stiffness) || data.stiffness < 0.0f
        || !PxIsFinite(data.damping) || data.damping < 0.0f
        || !PxIsFinite(data.impulseCap) || data.impulseCap < 0.0f)
        return 0; // No row: validate/reject bad configuration before stepping.

    const PxVec3 pA = bodyAToWorld.transform(data.anchorA);
    const PxVec3 pB = bodyBToWorld.transform(data.anchorB);
    const PxVec3 rA = pA - bodyAToWorld.p;
    const PxVec3 rB = pB - bodyBToWorld.p;
    const PxVec3 n = data.worldUnitAxis;
    anchorAWorld = pA;
    anchorBWorld = pB;
    bodyAWorldOffset = rA; // Report moment about A's attachment, in world axes.

    Px1DConstraint& row = rows[0];
    row = {}; // Explicitly initialize every field, not only the spring fields.
    row.linear0 = n;
    row.angular0 = rA.cross(n);
    row.linear1 = n; // SDK forms J = [linear0, angular0, -linear1, -angular1].
    row.angular1 = rB.cross(n);
    row.geometricError = n.dot(pA - pB) - data.restCoordinate;
    row.velocityTarget = 0.0f;
    row.minImpulse = -data.impulseCap;
    row.maxImpulse = data.impulseCap;
    row.mods.spring.stiffness = data.stiffness;
    row.mods.spring.damping = data.damping;
    row.flags = Px1DConstraintFlag::eSPRING | Px1DConstraintFlag::eOUTPUT_FORCE;
    row.solveHint = PxConstraintSolveHint::eNONE;
    // Deliberately no eHAS_DRIVE_LIMIT or eACCELERATION_SPRING. The cap remains
    // an impulse even if the owning constraint has eDRIVE_LIMITS_ARE_FORCES.
    return 1;
}

// Check the actual native callback signature without creating an SDK object.
PxConstraintSolverPrep axisSpringPrep = &prepareAxisSpring;
} // namespace physx_atlas
