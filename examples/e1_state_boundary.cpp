// Source-reading example for the pinned PhysX 5.11.0 headers.
// No main(), SDK construction, runtime validation, or experiment is included.
// Preconditions: valid Physics/Material/Scene; ordinary CPU access (no Direct GPU
// API); a single application owner; no simulation currently in flight. The caller
// owns returned actors and must release them before destroying their dependencies.
#include "PxPhysics.h"
#include "PxRigidDynamic.h"
#include "PxScene.h"
#include "PxShape.h"
#include "extensions/PxRigidBodyExt.h"
#include "geometry/PxBoxGeometry.h"

using namespace physx;

// SI units: a 1 m cube at 1 kg/m^3. Analytic mass = 1 kg; diagonal
// inertia = (1/6, 1/6, 1/6) kg m^2. These are not measured results.
PxRigidDynamic* createUnitBox(PxPhysics& physics, PxMaterial& material)
{
    PxRigidDynamic* body = physics.createRigidDynamic(PxTransform(PxIdentity));
    if (!body)
        return nullptr;

    PxShape* shape = physics.createShape(
        PxBoxGeometry(0.5f, 0.5f, 0.5f), material, true);
    if (!shape)
    {
        body->release();
        return nullptr;
    }

    const bool attached = body->attachShape(*shape);
    shape->release(); // Drop creation reference; successful attach retains one.
    if (!attached || !PxRigidBodyExt::updateMassAndInertia(*body, 1.0f))
    {
        body->release();
        return nullptr;
    }
    return body; // Caller explicitly adds it to a Scene and owns its lifetime.
}

struct FrameObservation
{
    PxTransform worldActor;
    PxTransform worldMass;
    PxTransform worldShape;
    PxVec3 worldComVelocity;
    PxVec3 worldAngularVelocity;
};

// Precondition: shape is attached to body; called at a completed-fetch boundary
// (or before the first step). Return value copies data, not SDK object pointers.
FrameObservation readFrames(const PxRigidDynamic& body, const PxShape& shape)
{
    const PxTransform worldActor = body.getGlobalPose();
    return {worldActor,
            worldActor * body.getCMassLocalPose(),
            worldActor * shape.getLocalPose(),
            body.getLinearVelocity(),
            body.getAngularVelocity()};
}

// Caller must also inspect its registered SDK error callback. Returning true is
// only the submit/fetch result, not an assurance of physical correctness. On false,
// do not advance the application clock or retry blindly: diagnose Scene state.
bool stepAtBoundary(PxScene& scene, PxReal h)
{
    if (!PxIsFinite(h) || h <= 0.0f)
        return false;
    if (!scene.simulate(h))
        return false;
    PxU32 errorState = 0;
    const bool fetched = scene.fetchResults(true, &errorState);
    return fetched && errorState == 0;
}
