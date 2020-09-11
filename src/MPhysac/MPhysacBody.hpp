/*******************************************************************************************
*   MPhysacBody.hpp
*
*   This file implements PhysicsBody from MPhysac in a C++ fashion.
*
********************************************************************************************/

#ifndef MPHYSAC_BODY_HPP
#define MPHYSAC_BODY_HPP

#include "MPhysacShape.hpp"

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Below types are required for PHYSAC_STANDALONE usage
//---------------------------------------------------------------------------------
class MPhysacBody {
    public:
        bool enabled;                            // Enabled dynamics state (collisions are calculated anyway)
        Vector2f position;                       // Physics body shape pivot
        Vector2f velocity;                       // Current linear velocity applied to position
        Vector2f force;                          // Current linear force (reset to 0 every step)
        float angularVelocity;                   // Current angular velocity applied to orient
        float torque;                            // Current angular force (reset to 0 every step)
        float orient;                            // Rotation in radians
        float inertia;                           // Moment of inertia
        float inverseInertia;                    // Inverse value of inertia
        float mass;                              // Physics body mass
        float inverseMass;                       // Inverse value of mass
        float staticFriction;                    // Friction when the body has not movement (0 to 1)
        float dynamicFriction;                   // Friction when the body has movement (0 to 1)
        float restitution;                       // Restitution coefficient of the body (0 to 1)
        bool useGravity;                         // Apply gravity force to dynamics
        bool isGrounded;                         // Physics grounded on other body state
        bool freezeOrient;                       // Physics rotation constraint
        MPhysacShape shape;                      // Physics body shape information (type, radius, vertices, normals)
        MPhysacSolidType solidType;

        void PhysicsAddForce(const Vector2f &force);  // Add force to the MPhysacBody
        void PhysicsAddTorque(float amount);          // Add torque to the MPhysacBody
        int GetMPhysacBodyShapeType();
        int GetMPhysacBodyShapeVerticesCount();
        Vector2f GetMPhysacBodyShapeVertex(int vertex);
        void SetMPhysacBodyRotation(float radians);

    private:
        friend class MPhysacWorld;
        MPhysacBody(const Vector2f &pos, MPhysacShapeType type, const Vector2f &dim, int density);
        
};

class PhysicsManifold {
    public:
        MPhysacBody *bodyA;                         // Manifold first physics body reference
        MPhysacBody *bodyB;                         // Manifold second physics body reference
        float penetration;                          // Depth of penetration from collision
        Vector2f normal;                            // Normal direction vector from 'a' to 'b'
        Vector2f contacts[2];                       // Points of contact during collision
        unsigned int contactsCount;                 // Current collision number of contacts
        float restitution;                          // Mixed restitution during collision
        float dynamicFriction;                      // Mixed dynamic friction during collision
        float staticFriction;                       // Mixed static friction during collision

    private:
        friend class MPhysacWorld;
        PhysicsManifold(MPhysacBody *body1, MPhysacBody *body2);
};

#endif