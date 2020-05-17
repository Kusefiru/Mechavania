/*******************************************************************************************
*   MPhysacWorld.cpp
*
*   This file implements most physics body manipulation from MPhysac in a C++ fashion.
*
********************************************************************************************/

#include "MPhysacWorld.hpp"

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Initializes physics values, pointers and creates physics loop thread
void MPhysacWorld::InitPhysics(void)
{
    #ifndef PHYSAC_NO_THREADS
        // NOTE: if defined, user will need to create a thread for PhysicsThread function manually
        // Create physics thread using POSIXS thread libraries
        pthread_create(&physicsThreadId, NULL,(void *(*)(void *)) &PhysicsLoop, NULL);
    #endif

    // Initialize high resolution timer
    InitTimer();

    #ifdef PHYSAC_DEBUG
        TRACELOG("[PHYSAC] physics module initialized successfully\n");
    #endif

    accumulator = 0.0;
    bodies.reserve(PHYSAC_MAX_BODIES);
    contacts.reserve(PHYSAC_MAX_MANIFOLDS);
}

// Returns true if physics thread is currently enabled
bool MPhysacWorld::IsPhysicsEnabled(void)
{
    return physicsThreadEnabled;
}

// Sets physics global gravity force
void MPhysacWorld::SetPhysicsGravity(float x, float y)
{
    gravityForce.x = x;
    gravityForce.y = y;
}

// Creates a new rectangle physics body with generic parameters
MPhysacBody* MPhysacWorld::CreatePhysicsBodyRectangle(Vector2 pos, float width, float height, float density)
{
    MPhysacBody* newBody = new MPhysacBody(pos, MPHYSAC_BOX, (Vector2){width, height}, density);

    // Add new body to bodies pointers array and update bodies count
    bodies.push_back(newBody);

    return newBody;
}

// Creates a new circle physics body with generic parameters
MPhysacBody* MPhysacWorld::CreatePhysicsBodyCircle(Vector2 pos, float radius, float density)
{
    return CreatePhysicsBodyPolygon(pos, radius, PHYSAC_CIRCLE_VERTICES, density);
}

// Creates a new polygon physics body with generic parameters
MPhysacBody* MPhysacWorld::CreatePhysicsBodyPolygon(Vector2 pos, float radius, int sides, float density)
{
    MPhysacBody* newBody = new MPhysacBody(pos, MPHYSAC_POLYGON, (Vector2){radius, sides}, density);

    // Add new body to bodies pointers array and update bodies count
    bodies.push_back(newBody);

    return newBody;
}

// Returns the number of bodies registered in the MPhysacWorld
int MPhysacWorld::GetMPhysacBodiesCount(){
    return bodies.size();
}

// Returns the number of bodies registered in the MPhysacWorld
int MPhysacWorld::GetManifoldsCount(){
    return contacts.size();
}

// Returns a MPhysacBody of the bodies pool at a specific index
MPhysacBody* MPhysacWorld::GetMPhysacBody(int index)
{
    return bodies.at(index);
}

// Remove MPhysacBody from bodies list then destroys it 
void MPhysacWorld::DestroyMPhysacBody(MPhysacBody *body)
{
    for (int i = 0; i < bodies.size(); i++)
    {
        if (bodies.at(i) == body)
        {
            bodies.at(i) = bodies.back();
            bodies.pop_back();
            break;
        }
    }

    delete body;
}

// Destroys created physics bodies and manifolds and resets global values
void MPhysacWorld::ResetPhysics(void)
{
    // Delete all MPhysacBodies and empty the list
    while(bodies.size() > 0)
    {
        delete bodies.back();
        bodies.pop_back();
    }

    // Delete all MPhysacManifolds and empty the list
    while(contacts.size() > 0)
    {
        delete contacts.back();
        contacts.pop_back();
    }
}

// Unitializes physics pointers and exits physics loop thread
void MPhysacWorld::ClosePhysics(void)
{
    // Exit physics loop thread
    physicsThreadEnabled = false;

    #ifndef PHYSAC_NO_THREADS
        pthread_join(physicsThreadId, NULL);
    #endif

    // Clear both MPhysacBodies and MPhysacManifolds lists
    ResetPhysics();

    #ifdef PHYSAC_DEBUG
        if (physicsBodiesCount > 0 || usedMemory != 0) TRACELOG("[PHYSAC] physics module closed with %i still allocated bodies [MEMORY: %i bytes]\n", physicsBodiesCount, usedMemory);
        else if (physicsManifoldsCount > 0 || usedMemory != 0) TRACELOG("[PHYSAC] physics module closed with %i still allocated manifolds [MEMORY: %i bytes]\n", physicsManifoldsCount, usedMemory);
        else TRACELOG("[PHYSAC] physics module closed successfully\n");
    #endif
}

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------
// Physics loop thread function
void *MPhysacWorld::PhysicsLoop(void *arg)
{
#ifndef PHYSAC_NO_THREADS
    #ifdef PHYSAC_DEBUG
        TRACELOG("[PHYSAC] physics thread created successfully\n");
    #endif

    // Initialize physics loop thread values
    physicsThreadEnabled = true;

    // Physics update loop
    while (physicsThreadEnabled)
    {
        RunPhysicsStep();
    }
#endif

    return NULL;
}

// Physics steps calculations (dynamics, collisions and position corrections)
void MPhysacWorld::PhysicsStep(void)
{
    // Update current steps count
    stepsCount++;
    
    // Clear previous generated collisions information
    for (int i = contacts.size() - 1; i >= 0; i--)
    {
        DestroyPhysicsManifold(contacts.at(i));
    }
    
    // Reset physics bodies grounded state
    for (int i = 0; i < bodies.size(); i++)
    {
        bodies.at(i)->isGrounded = false;
    }
    
    // Generate new collision information
    for (int i = 0; i < bodies.size(); i++)
    {
        MPhysacBody *bodyA = bodies.at(i);

        for (int j = i + 1; j < bodies.size(); j++)
        {
            MPhysacBody *bodyB = bodies.at(j);

            if ((bodyA->inverseMass == 0) && (bodyB->inverseMass == 0)) continue;

            PhysicsManifold *manifold = CreatePhysicsManifold(bodyA, bodyB);
            SolvePhysicsManifold(manifold);

            if (manifold->contactsCount > 0)
            {
                // Create a new manifold with same information as previously solved manifold and add it to the manifolds pool last slot
                PhysicsManifold *newManifold = CreatePhysicsManifold(bodyA, bodyB);
                newManifold->penetration = manifold->penetration;
                newManifold->normal = manifold->normal;
                newManifold->contacts[0] = manifold->contacts[0];
                newManifold->contacts[1] = manifold->contacts[1];
                newManifold->contactsCount = manifold->contactsCount;
                newManifold->restitution = manifold->restitution;
                newManifold->dynamicFriction = manifold->dynamicFriction;
                newManifold->staticFriction = manifold->staticFriction;
            }
        }
    }
    
    // Integrate forces to physics bodies
    for (int i = 0; i < bodies.size(); i++)
    {
        IntegratePhysicsForces(bodies.at(i));
    }
    
    // Initialize physics manifolds to solve collisions
    for (int i = 0; i < contacts.size(); i++)
    {
        InitializePhysicsManifolds(contacts.at(i));
    }
    
    // Integrate physics collisions impulses to solve collisions
    for (int i = 0; i < PHYSAC_COLLISION_ITERATIONS; i++)
    {
        for (int j = 0; j < contacts.size(); j++)
        {
            IntegratePhysicsImpulses(contacts.at(j));
        }
    }
    
    // Integrate velocity to physics bodies
    for (int i = 0; i < bodies.size(); i++)
    {
        IntegratePhysicsVelocity(bodies.at(i));
    }
    
    // Correct physics bodies positions based on manifolds collision information
    for (int i = 0; i < contacts.size(); i++)
    {
        CorrectPhysicsPositions(contacts.at(i));
    }
    
    // Clear physics bodies forces
    for (int i = 0; i < bodies.size(); i++)
    {
        bodies.at(i)->force = PHYSAC_VECTOR_ZERO;
        bodies.at(i)->torque = 0.0f;
    }
}

// Wrapper to ensure PhysicsStep is run with at a fixed time step
void MPhysacWorld::RunPhysicsStep(void)
{
    // Calculate current time
    currentTime = GetCurrentTime();

    // Calculate current delta time
    const double delta = currentTime - startTime;

    // Store the time elapsed since the last frame began
    accumulator += delta;

    // Fixed time stepping loop
    while (accumulator >= deltaTime)
    {
#ifdef PHYSAC_DEBUG
        //TRACELOG("currentTime %f, startTime %f, accumulator-pre %f, accumulator-post %f, delta %f, deltaTime %f\n",
        //       currentTime, startTime, accumulator, accumulator-deltaTime, delta, deltaTime);
#endif
        PhysicsStep();
        accumulator -= deltaTime;
    }

    // Record the starting of this frame
    startTime = currentTime;
}

void MPhysacWorld::SetPhysicsTimeStep(double delta)
{
    deltaTime = delta;
}

// Creates a new physics manifold to solve collision
PhysicsManifold* MPhysacWorld::CreatePhysicsManifold(MPhysacBody *a, MPhysacBody *b)
{
    PhysicsManifold *newManifold = new PhysicsManifold(a, b);

    // Add new body to bodies pointers array and update bodies count
    contacts.push_back(newManifold);

    return newManifold;
}

// Unitializes and destroys a physics manifold
void MPhysacWorld::DestroyPhysicsManifold(PhysicsManifold *manifold)
{
    for (int i = 0; i < contacts.size(); i++)
    {
        if (contacts.at(i) == manifold)
        {
            contacts.at(i) = contacts.back();
            contacts.pop_back();
            break;
        }
    }

    delete manifold;
}

// Solves a created physics manifold between two physics bodies
void MPhysacWorld::SolvePhysicsManifold(PhysicsManifold *manifold)
{
    switch (manifold->bodyA->shape.type)
    {
        case MPHYSAC_CIRCLE:
        {
            switch (manifold->bodyB->shape.type)
            {
                case MPHYSAC_CIRCLE: SolveCircleToCircle(manifold); break;
                case MPHYSAC_POLYGON: SolveCircleToPolygon(manifold); break;
                default: break;
            }
        } break;
        case MPHYSAC_POLYGON:
        {
            switch (manifold->bodyB->shape.type)
            {
                case MPHYSAC_CIRCLE: SolvePolygonToCircle(manifold); break;
                case MPHYSAC_POLYGON: SolvePolygonToPolygon(manifold); break;
                default: break;
            }
        } break;
        default: break;
    }

    // Update physics body grounded state if normal direction is down and grounded state is not set yet in previous manifolds
    if (!manifold->bodyB->isGrounded) manifold->bodyB->isGrounded = (manifold->normal.y < 0);
}

// Solves collision between two circle shape physics bodies
void MPhysacWorld::SolveCircleToCircle(PhysicsManifold *manifold)
{
    MPhysacBody *bodyA = manifold->bodyA;
    MPhysacBody *bodyB = manifold->bodyB;

    if ((bodyA == nullptr) || (bodyB == nullptr)) return;

    // Calculate translational vector, which is normal
    Vector2 normal = Vector2Subtract(bodyB->position, bodyA->position);

    float distSqr = MPhysac::MathLenSqr(normal);
    float radius = bodyA->shape.radius + bodyB->shape.radius;

    // Check if circles are not in contact
    if (distSqr >= radius*radius)
    {
        manifold->contactsCount = 0;
        return;
    }

    float distance = sqrtf(distSqr);
    manifold->contactsCount = 1;

    if (distance == 0.0f)
    {
        manifold->penetration = bodyA->shape.radius;
        manifold->normal = (Vector2){ 1.0f, 0.0f };
        manifold->contacts[0] = bodyA->position;
    }
    else
    {
        manifold->penetration = radius - distance;
        manifold->normal = (Vector2){ normal.x/distance, normal.y/distance }; // Faster than using MPhysac::MathNormalize() due to sqrt is already performed
        manifold->contacts[0] = (Vector2){ manifold->normal.x*bodyA->shape.radius + bodyA->position.x, manifold->normal.y*bodyA->shape.radius + bodyA->position.y };
    }

    // Update physics body grounded state if normal direction is down
    if (!bodyA->isGrounded) bodyA->isGrounded = (manifold->normal.y < 0);
}

// Solves collision between a circle to a polygon shape physics bodies
void MPhysacWorld::SolveCircleToPolygon(PhysicsManifold *manifold)
{
    MPhysacBody *bodyA = manifold->bodyA;
    MPhysacBody *bodyB = manifold->bodyB;

    if ((bodyA == nullptr) || (bodyB == nullptr)) return;

    manifold->contactsCount = 0;

    // Transform circle center to polygon transform space
    Vector2 center = bodyA->position;
    center = MPhysac::Mat2MultiplyVector2(MPhysac::Mat2Transpose(bodyB->shape.transform), Vector2Subtract(center, bodyB->position));

    // Find edge with minimum penetration
    // It is the same concept as using support points in SolvePolygonToPolygon
    float separation = -PHYSAC_FLT_MAX;
    int faceNormal = 0;
    PolygonData vertexData = bodyB->shape.vertexData;

    for (int i = 0; i < vertexData.positions.size(); i++)
    {
        float currentSeparation = MPhysac::MathDot(vertexData.normals[i], Vector2Subtract(center, vertexData.positions[i]));

        if (currentSeparation > bodyA->shape.radius) return;

        if (currentSeparation > separation)
        {
            separation = currentSeparation;
            faceNormal = i;
        }
    }

    // Grab face's vertices
    Vector2 v1 = vertexData.positions[faceNormal];
    int nextIndex = (((faceNormal + 1) < vertexData.positions.size()) ? (faceNormal + 1) : 0);
    Vector2 v2 = vertexData.positions[nextIndex];

    // Check to see if center is within polygon
    if (separation < PHYSAC_EPSILON)
    {
        manifold->contactsCount = 1;
        Vector2 normal = MPhysac::Mat2MultiplyVector2(bodyB->shape.transform, vertexData.normals[faceNormal]);
        manifold->normal = (Vector2){ -normal.x, -normal.y };
        manifold->contacts[0] = (Vector2){ manifold->normal.x*bodyA->shape.radius + bodyA->position.x, manifold->normal.y*bodyA->shape.radius + bodyA->position.y };
        manifold->penetration = bodyA->shape.radius;
        return;
    }

    // Determine which voronoi region of the edge center of circle lies within
    float dot1 = MPhysac::MathDot(Vector2Subtract(center, v1), Vector2Subtract(v2, v1));
    float dot2 = MPhysac::MathDot(Vector2Subtract(center, v2), Vector2Subtract(v1, v2));
    manifold->penetration = bodyA->shape.radius - separation;

    if (dot1 <= 0.0f) // Closest to v1
    {
        if (MPhysac::DistSqr(center, v1) > bodyA->shape.radius*bodyA->shape.radius) return;

        manifold->contactsCount = 1;
        Vector2 normal = Vector2Subtract(v1, center);
        normal = MPhysac::Mat2MultiplyVector2(bodyB->shape.transform, normal);
        MPhysac::MathNormalize(&normal);
        manifold->normal = normal;
        v1 = MPhysac::Mat2MultiplyVector2(bodyB->shape.transform, v1);
        v1 = Vector2Add(v1, bodyB->position);
        manifold->contacts[0] = v1;
    }
    else if (dot2 <= 0.0f) // Closest to v2
    {
        if (MPhysac::DistSqr(center, v2) > bodyA->shape.radius*bodyA->shape.radius) return;

        manifold->contactsCount = 1;
        Vector2 normal = Vector2Subtract(v2, center);
        v2 = MPhysac::Mat2MultiplyVector2(bodyB->shape.transform, v2);
        v2 = Vector2Add(v2, bodyB->position);
        manifold->contacts[0] = v2;
        normal = MPhysac::Mat2MultiplyVector2(bodyB->shape.transform, normal);
        MPhysac::MathNormalize(&normal);
        manifold->normal = normal;
    }
    else // Closest to face
    {
        Vector2 normal = vertexData.normals[faceNormal];

        if (MPhysac::MathDot(Vector2Subtract(center, v1), normal) > bodyA->shape.radius) return;

        normal = MPhysac::Mat2MultiplyVector2(bodyB->shape.transform, normal);
        manifold->normal = (Vector2){ -normal.x, -normal.y };
        manifold->contacts[0] = (Vector2){ manifold->normal.x*bodyA->shape.radius + bodyA->position.x, manifold->normal.y*bodyA->shape.radius + bodyA->position.y };
        manifold->contactsCount = 1;
    }
}

// Solves collision between a polygon to a circle shape physics bodies
void MPhysacWorld::SolvePolygonToCircle(PhysicsManifold *manifold)
{
    MPhysacBody *bodyA = manifold->bodyA;
    MPhysacBody *bodyB = manifold->bodyB;

    if ((bodyA == nullptr) || (bodyB == nullptr)) return;

    manifold->bodyA = bodyB;
    manifold->bodyB = bodyA;
    SolveCircleToPolygon(manifold);

    manifold->normal.x *= -1.0f;
    manifold->normal.y *= -1.0f;
}

// Solves collision between two polygons shape physics bodies
void MPhysacWorld::SolvePolygonToPolygon(PhysicsManifold *manifold)
{
    if ((manifold->bodyA == nullptr) || (manifold->bodyB == nullptr)) return;

    MPhysacShape bodyA = manifold->bodyA->shape;
    MPhysacShape bodyB = manifold->bodyB->shape;
    manifold->contactsCount = 0;

    // Check for separating axis with A shape's face planes
    int faceA = 0;
    float penetrationA = FindAxisLeastPenetration(&faceA, bodyA, bodyB);
    if (penetrationA >= 0.0f) return;

    // Check for separating axis with B shape's face planes
    int faceB = 0;
    float penetrationB = FindAxisLeastPenetration(&faceB, bodyB, bodyA);
    if (penetrationB >= 0.0f) return;

    int referenceIndex = 0;
    bool flip = false;  // Always point from A shape to B shape

    MPhysacShape refPoly; // Reference
    MPhysacShape incPoly; // Incident

    // Determine which shape contains reference face
    if (BiasGreaterThan(penetrationA, penetrationB))
    {
        refPoly = bodyA;
        incPoly = bodyB;
        referenceIndex = faceA;
    }
    else
    {
        refPoly = bodyB;
        incPoly = bodyA;
        referenceIndex = faceB;
        flip = true;
    }

    // World space incident face
    Vector2 incidentFace[2];
    FindIncidentFace(&incidentFace[0], &incidentFace[1], refPoly, incPoly, referenceIndex);

    // Setup reference face vertices
    PolygonData refData = refPoly.vertexData;
    Vector2 v1 = refData.positions[referenceIndex];
    referenceIndex = (((referenceIndex + 1) < refData.positions.size()) ? (referenceIndex + 1) : 0);
    Vector2 v2 = refData.positions[referenceIndex];

    // Transform vertices to world space
    v1 = MPhysac::Mat2MultiplyVector2(refPoly.transform, v1);
    v1 = Vector2Add(v1, refPoly.body->position);
    v2 = MPhysac::Mat2MultiplyVector2(refPoly.transform, v2);
    v2 = Vector2Add(v2, refPoly.body->position);

    // Calculate reference face side normal in world space
    Vector2 sidePlaneNormal = Vector2Subtract(v2, v1);
    MPhysac::MathNormalize(&sidePlaneNormal);

    // Orthogonalize
    Vector2 refFaceNormal = { sidePlaneNormal.y, -sidePlaneNormal.x };
    float refC = MPhysac::MathDot(refFaceNormal, v1);
    float negSide = MPhysac::MathDot(sidePlaneNormal, v1)*-1;
    float posSide = MPhysac::MathDot(sidePlaneNormal, v2);

    // Clip incident face to reference face side planes (due to floating point error, possible to not have required points
    if (Clip((Vector2){ -sidePlaneNormal.x, -sidePlaneNormal.y }, negSide, &incidentFace[0], &incidentFace[1]) < 2) return;
    if (Clip(sidePlaneNormal, posSide, &incidentFace[0], &incidentFace[1]) < 2) return;

    // Flip normal if required
    manifold->normal = (flip ? (Vector2){ -refFaceNormal.x, -refFaceNormal.y } : refFaceNormal);

    // Keep points behind reference face
    int currentPoint = 0; // Clipped points behind reference face
    float separation = MPhysac::MathDot(refFaceNormal, incidentFace[0]) - refC;
    if (separation <= 0.0f)
    {
        manifold->contacts[currentPoint] = incidentFace[0];
        manifold->penetration = -separation;
        currentPoint++;
    }
    else manifold->penetration = 0.0f;

    separation = MPhysac::MathDot(refFaceNormal, incidentFace[1]) - refC;

    if (separation <= 0.0f)
    {
        manifold->contacts[currentPoint] = incidentFace[1];
        manifold->penetration += -separation;
        currentPoint++;

        // Calculate total penetration average
        manifold->penetration /= currentPoint;
    }

    manifold->contactsCount = currentPoint;
}

// Integrates physics forces into velocity
void MPhysacWorld::IntegratePhysicsForces(MPhysacBody *body)
{
    if ((body->inverseMass == 0.0f) || !body->enabled) return;

    body->velocity.x += (body->force.x*body->inverseMass)*(deltaTime/2.0);
    body->velocity.y += (body->force.y*body->inverseMass)*(deltaTime/2.0);

    if (body->useGravity)
    {
        body->velocity.x += gravityForce.x*(deltaTime/1000/2.0);
        body->velocity.y += gravityForce.y*(deltaTime/1000/2.0);
    }

    if (!body->freezeOrient) body->angularVelocity += body->torque*body->inverseInertia*(deltaTime/2.0);
}

// Initializes physics manifolds to solve collisions
void MPhysacWorld::InitializePhysicsManifolds(PhysicsManifold *manifold)
{
    MPhysacBody *bodyA = manifold->bodyA;
    MPhysacBody *bodyB = manifold->bodyB;

    if ((bodyA == nullptr) || (bodyB == nullptr)) return;

    // Calculate average restitution, static and dynamic friction
    manifold->restitution = sqrtf(bodyA->restitution*bodyB->restitution);
    manifold->staticFriction = sqrtf(bodyA->staticFriction*bodyB->staticFriction);
    manifold->dynamicFriction = sqrtf(bodyA->dynamicFriction*bodyB->dynamicFriction);

    for (int i = 0; i < manifold->contactsCount; i++)
    {
        // Caculate radius from center of mass to contact
        Vector2 radiusA = Vector2Subtract(manifold->contacts[i], bodyA->position);
        Vector2 radiusB = Vector2Subtract(manifold->contacts[i], bodyB->position);

        Vector2 crossA = MPhysac::MathCross(bodyA->angularVelocity, radiusA);
        Vector2 crossB = MPhysac::MathCross(bodyB->angularVelocity, radiusB);

        Vector2 radiusV = { 0.0f, 0.0f };
        radiusV.x = bodyB->velocity.x + crossB.x - bodyA->velocity.x - crossA.x;
        radiusV.y = bodyB->velocity.y + crossB.y - bodyA->velocity.y - crossA.y;

        // Determine if we should perform a resting collision or not;
        // The idea is if the only thing moving this object is gravity, then the collision should be performed without any restitution
        if (MPhysac::MathLenSqr(radiusV) < (MPhysac::MathLenSqr((Vector2){ gravityForce.x*deltaTime/1000, gravityForce.y*deltaTime/1000 }) + PHYSAC_EPSILON)) manifold->restitution = 0;
    }
}

// Integrates physics collisions impulses to solve collisions
void MPhysacWorld::IntegratePhysicsImpulses(PhysicsManifold *manifold)
{
    MPhysacBody *bodyA = manifold->bodyA;
    MPhysacBody *bodyB = manifold->bodyB;

    if ((bodyA == nullptr) || (bodyB == nullptr)) return;

    // Early out and positional correct if both objects have infinite mass
    if (fabs(bodyA->inverseMass + bodyB->inverseMass) <= PHYSAC_EPSILON)
    {
        bodyA->velocity = PHYSAC_VECTOR_ZERO;
        bodyB->velocity = PHYSAC_VECTOR_ZERO;
        return;
    }
    
    for (int i = 0; i < manifold->contactsCount; i++)
    {
        // Calculate radius from center of mass to contact
        Vector2 radiusA = Vector2Subtract(manifold->contacts[i], bodyA->position);
        Vector2 radiusB = Vector2Subtract(manifold->contacts[i], bodyB->position);

        // Calculate relative velocity
        Vector2 radiusV = { 0.0f, 0.0f };
        radiusV.x = bodyB->velocity.x + MPhysac::MathCross(bodyB->angularVelocity, radiusB).x - bodyA->velocity.x - MPhysac::MathCross(bodyA->angularVelocity, radiusA).x;
        radiusV.y = bodyB->velocity.y + MPhysac::MathCross(bodyB->angularVelocity, radiusB).y - bodyA->velocity.y - MPhysac::MathCross(bodyA->angularVelocity, radiusA).y;

        // Relative velocity along the normal
        float contactVelocity = MPhysac::MathDot(radiusV, manifold->normal);

        // Do not resolve if velocities are separating
        if (contactVelocity > 0.0f) return;

        float raCrossN = MPhysac::MathCrossVector2(radiusA, manifold->normal);
        float rbCrossN = MPhysac::MathCrossVector2(radiusB, manifold->normal);

        float inverseMassSum = bodyA->inverseMass + bodyB->inverseMass + (raCrossN*raCrossN)*bodyA->inverseInertia + (rbCrossN*rbCrossN)*bodyB->inverseInertia;

        // Calculate impulse scalar value
        float impulse = -(1.0f + manifold->restitution)*contactVelocity;
        impulse /= inverseMassSum;
        impulse /= (float)manifold->contactsCount;

        // Apply impulse to each physics body
        Vector2 impulseV = { manifold->normal.x*impulse, manifold->normal.y*impulse };

        if (bodyA->enabled)
        {
            bodyA->velocity.x += bodyA->inverseMass*(-impulseV.x);
            bodyA->velocity.y += bodyA->inverseMass*(-impulseV.y);
            if (!bodyA->freezeOrient) bodyA->angularVelocity += bodyA->inverseInertia*MPhysac::MathCrossVector2(radiusA, (Vector2){ -impulseV.x, -impulseV.y });
        }

        if (bodyB->enabled)
        {
            bodyB->velocity.x += bodyB->inverseMass*(impulseV.x);
            bodyB->velocity.y += bodyB->inverseMass*(impulseV.y);
            if (!bodyB->freezeOrient) bodyB->angularVelocity += bodyB->inverseInertia*MPhysac::MathCrossVector2(radiusB, impulseV);
        }

        // Apply friction impulse to each physics body
        radiusV.x = bodyB->velocity.x + MPhysac::MathCross(bodyB->angularVelocity, radiusB).x - bodyA->velocity.x - MPhysac::MathCross(bodyA->angularVelocity, radiusA).x;
        radiusV.y = bodyB->velocity.y + MPhysac::MathCross(bodyB->angularVelocity, radiusB).y - bodyA->velocity.y - MPhysac::MathCross(bodyA->angularVelocity, radiusA).y;

        Vector2 tangent = { radiusV.x - (manifold->normal.x*MPhysac::MathDot(radiusV, manifold->normal)), radiusV.y - (manifold->normal.y*MPhysac::MathDot(radiusV, manifold->normal)) };
        MPhysac::MathNormalize(&tangent);

        // Calculate impulse tangent magnitude
        float impulseTangent = -MPhysac::MathDot(radiusV, tangent);
        impulseTangent /= inverseMassSum;
        impulseTangent /= (float)manifold->contactsCount;

        float absImpulseTangent = fabs(impulseTangent);

        // Don't apply tiny friction impulses
        if (absImpulseTangent <= PHYSAC_EPSILON) return;

        // Apply coulumb's law
        Vector2 tangentImpulse = { 0.0f, 0.0f };
        if (absImpulseTangent < impulse*manifold->staticFriction) tangentImpulse = (Vector2){ tangent.x*impulseTangent, tangent.y*impulseTangent };
        else tangentImpulse = (Vector2){ tangent.x*-impulse*manifold->dynamicFriction, tangent.y*-impulse*manifold->dynamicFriction };

        // Apply friction impulse
        if (bodyA->enabled)
        {
            bodyA->velocity.x += bodyA->inverseMass*(-tangentImpulse.x);
            bodyA->velocity.y += bodyA->inverseMass*(-tangentImpulse.y);

            if (!bodyA->freezeOrient) bodyA->angularVelocity += bodyA->inverseInertia*MPhysac::MathCrossVector2(radiusA, (Vector2){ -tangentImpulse.x, -tangentImpulse.y });
        }

        if (bodyB->enabled)
        {
            bodyB->velocity.x += bodyB->inverseMass*(tangentImpulse.x);
            bodyB->velocity.y += bodyB->inverseMass*(tangentImpulse.y);

            if (!bodyB->freezeOrient) bodyB->angularVelocity += bodyB->inverseInertia*MPhysac::MathCrossVector2(radiusB, tangentImpulse);
        }
    }
}

// Integrates physics velocity into position and forces
void MPhysacWorld::IntegratePhysicsVelocity(MPhysacBody *body)
{
    if (!body->enabled) return;

    body->position.x += body->velocity.x*deltaTime;
    body->position.y += body->velocity.y*deltaTime;

    if (!body->freezeOrient) body->orient += body->angularVelocity*deltaTime;
    MPhysac::Mat2Set(&body->shape.transform, body->orient);

    IntegratePhysicsForces(body);
}

// Corrects physics bodies positions based on manifolds collision information
void MPhysacWorld::CorrectPhysicsPositions(PhysicsManifold *manifold)
{
    MPhysacBody *bodyA = manifold->bodyA;
    MPhysacBody *bodyB = manifold->bodyB;

    if ((bodyA == nullptr) || (bodyB == nullptr)) return;

    Vector2 correction = { 0.0f, 0.0f };
    correction.x = (physacmax(manifold->penetration - PHYSAC_PENETRATION_ALLOWANCE, 0.0f)/(bodyA->inverseMass + bodyB->inverseMass))*manifold->normal.x*PHYSAC_PENETRATION_CORRECTION;
    correction.y = (physacmax(manifold->penetration - PHYSAC_PENETRATION_ALLOWANCE, 0.0f)/(bodyA->inverseMass + bodyB->inverseMass))*manifold->normal.y*PHYSAC_PENETRATION_CORRECTION;

    if (bodyA->enabled)
    {
        bodyA->position.x -= correction.x*bodyA->inverseMass;
        bodyA->position.y -= correction.y*bodyA->inverseMass;
    }

    if (bodyB->enabled)
    {
        bodyB->position.x += correction.x*bodyB->inverseMass;
        bodyB->position.y += correction.y*bodyB->inverseMass;
    }
}

// Returns the extreme point along a direction within a polygon
Vector2 MPhysacWorld::GetSupport(const MPhysacShape &shape, Vector2 dir)
{
    float bestProjection = -PHYSAC_FLT_MAX;
    Vector2 bestVertex = { 0.0f, 0.0f };
    PolygonData data = shape.vertexData;

    for (int i = 0; i < data.positions.size(); i++)
    {
        Vector2 vertex = data.positions[i];
        float projection = MPhysac::MathDot(vertex, dir);

        if (projection > bestProjection)
        {
            bestVertex = vertex;
            bestProjection = projection;
        }
    }

    return bestVertex;
}

// Finds polygon shapes axis least penetration
float MPhysacWorld::FindAxisLeastPenetration(int *faceIndex, const MPhysacShape &shapeA, const MPhysacShape &shapeB)
{
    float bestDistance = -PHYSAC_FLT_MAX;
    int bestIndex = 0;

    PolygonData dataA = shapeA.vertexData;
    //PolygonData dataB = shapeB.vertexData;

    for (int i = 0; i < dataA.positions.size(); i++)
    {
        // Retrieve a face normal from A shape
        Vector2 normal = dataA.normals[i];
        Vector2 transNormal = MPhysac::Mat2MultiplyVector2(shapeA.transform, normal);

        // Transform face normal into B shape's model space
        Matrix2x2 buT = MPhysac::Mat2Transpose(shapeB.transform);
        normal = MPhysac::Mat2MultiplyVector2(buT, transNormal);

        // Retrieve support point from B shape along -n
        Vector2 support = GetSupport(shapeB, (Vector2){ -normal.x, -normal.y });

        // Retrieve vertex on face from A shape, transform into B shape's model space
        Vector2 vertex = dataA.positions[i];
        vertex = MPhysac::Mat2MultiplyVector2(shapeA.transform, vertex);
        vertex = Vector2Add(vertex, shapeA.body->position);
        vertex = Vector2Subtract(vertex, shapeB.body->position);
        vertex = MPhysac::Mat2MultiplyVector2(buT, vertex);

        // Compute penetration distance in B shape's model space
        float distance = MPhysac::MathDot(normal, Vector2Subtract(support, vertex));

        // Store greatest distance
        if (distance > bestDistance)
        {
            bestDistance = distance;
            bestIndex = i;
        }
    }

    *faceIndex = bestIndex;
    return bestDistance;
}

// Finds two polygon shapes incident face
void MPhysacWorld::FindIncidentFace(Vector2 *v0, Vector2 *v1, const MPhysacShape &ref, const MPhysacShape &inc, int index)
{
    PolygonData refData = ref.vertexData;
    PolygonData incData = inc.vertexData;

    Vector2 referenceNormal = refData.normals[index];

    // Calculate normal in incident's frame of reference
    referenceNormal = MPhysac::Mat2MultiplyVector2(ref.transform, referenceNormal); // To world space
    referenceNormal = MPhysac::Mat2MultiplyVector2(MPhysac::Mat2Transpose(inc.transform), referenceNormal); // To incident's model space

    // Find most anti-normal face on polygon
    int incidentFace = 0;
    float minDot = PHYSAC_FLT_MAX;

    for (int i = 0; i < incData.positions.size(); i++)
    {
        float dot = MPhysac::MathDot(referenceNormal, incData.normals[i]);

        if (dot < minDot)
        {
            minDot = dot;
            incidentFace = i;
        }
    }

    // Assign face vertices for incident face
    *v0 = MPhysac::Mat2MultiplyVector2(inc.transform, incData.positions[incidentFace]);
    *v0 = Vector2Add(*v0, inc.body->position);
    incidentFace = (((incidentFace + 1) < incData.positions.size()) ? (incidentFace + 1) : 0);
    *v1 = MPhysac::Mat2MultiplyVector2(inc.transform, incData.positions[incidentFace]);
    *v1 = Vector2Add(*v1, inc.body->position);
}

// Calculates clipping based on a normal and two faces
int MPhysacWorld::Clip(Vector2 normal, float clip, Vector2 *faceA, Vector2 *faceB)
{
    int sp = 0;
    Vector2 out[2] = { *faceA, *faceB };

    // Retrieve distances from each endpoint to the line
    float distanceA = MPhysac::MathDot(normal, *faceA) - clip;
    float distanceB = MPhysac::MathDot(normal, *faceB) - clip;

    // If negative (behind plane)
    if (distanceA <= 0.0f) out[sp++] = *faceA;
    if (distanceB <= 0.0f) out[sp++] = *faceB;

    // If the points are on different sides of the plane
    if ((distanceA*distanceB) < 0.0f)
    {
        // Push intersection point
        float alpha = distanceA/(distanceA - distanceB);
        out[sp] = *faceA;
        Vector2 delta = Vector2Subtract(*faceB, *faceA);
        delta.x *= alpha;
        delta.y *= alpha;
        out[sp] = Vector2Add(out[sp], delta);
        sp++;
    }

    // Assign the new converted values
    *faceA = out[0];
    *faceB = out[1];

    return sp;
}

// Check if values are between bias range
bool MPhysacWorld::BiasGreaterThan(float valueA, float valueB)
{
    return (valueA >= (valueB*0.95f + valueA*0.01f));
}

// Returns the barycenter of a triangle given by 3 points
Vector2 MPhysacWorld::TriangleBarycenter(Vector2 v1, Vector2 v2, Vector2 v3)
{
    Vector2 result = { 0.0f, 0.0f };

    result.x = (v1.x + v2.x + v3.x)/3;
    result.y = (v1.y + v2.y + v3.y)/3;

    return result;
}

// Initializes hi-resolution MONOTONIC timer
void MPhysacWorld::InitTimer(void)
{
    srand(time(NULL));              // Initialize random seed

#ifdef _WIN32
    QueryPerformanceFrequency((unsigned long long int *) &frequency);
#endif

#if defined(__EMSCRIPTEN__) || defined(__linux__)
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) == 0) frequency = 1000000000;
#endif

#ifdef __APPLE__
    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);
    frequency = (timebase.denom*1e9)/timebase.numer;
#endif

    baseTime = GetTimeCount();      // Get MONOTONIC clock time offset
    startTime = GetCurrentTime();   // Get current time
}

// Get hi-res MONOTONIC time measure in seconds
unsigned long long int MPhysacWorld::GetTimeCount(void)
{
    unsigned long long int value = 0;

#ifdef _WIN32
    QueryPerformanceCounter((unsigned long long int *) &value);
#endif

#ifdef __linux__
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    value = (unsigned long long int)now.tv_sec*(unsigned long long int)1000000000 + (unsigned long long int)now.tv_nsec;
#endif

#ifdef __APPLE__
    value = mach_absolute_time();
#endif

    return value;
}

// Get current time in milliseconds
double MPhysacWorld::GetCurrentTime(void)
{
    return (double)(GetTimeCount() - baseTime)/frequency*1000;
}