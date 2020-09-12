/*******************************************************************************************
*   MPhysacBody.cpp
*
*   This file implements PhysicsBody from MPhysac in a C++ fashion.
*
********************************************************************************************/

#include "MPhysac/MPhysacBody.hpp"

// Adds a force to a physics body
void MPhysacBody::PhysicsAddForce(const Vector2f& addforce) {
    force = force + addforce;
}

// Adds an angular force to a physics body
void MPhysacBody::PhysicsAddTorque(float amount) {
    torque += amount;
}

// Returns the physics body shape type (MPHYSAC_BOX, MPHYSAC_CIRCLE or MPHYSAC_POLYGON)
int MPhysacBody::GetMPhysacBodyShapeType() {
    return shape.type;
}

// Returns the amount of vertices of a physics body shape
size_t MPhysacBody::GetMPhysacBodyShapeVerticesCount() {
    return shape.vertexData.positions.size();
}

// Returns transformed position of a body shape (body position + vertex transformed position)
Vector2f MPhysacBody::GetMPhysacBodyShapeVertex(int vertex) {
    return position + MPhysac::Mat2MultiplyVector2(shape.transform, shape.vertexData.positions[vertex]);
}

// Sets physics body shape transform based on radians parameter
void MPhysacBody::SetMPhysacBodyRotation(float radians) {
    orient = radians;

    if (shape.type != MPHYSAC_CIRCLE) shape.transform = MPhysac::Mat2Radians(radians);
}

// Create a MPhysacBody
MPhysacBody::MPhysacBody(const Vector2f& pos, MPhysacShapeType type, const Vector2f& dim, float density) :
    enabled(true),
    position(pos),
    velocity(),
    force(),
    angularVelocity(0.f),
    torque(0.f),
    orient(0.f) {
    // Initialize new body with generic values
    shape.type = type;
    shape.body = this;
    shape.radius = 0.0f;
    shape.transform = MPhysac::Mat2Radians(0.0f);

    switch(type) {
        case MPHYSAC_BOX: shape.vertexData.CreateRectanglePolygon(pos, dim); break; // TODO : segfault here
        // case MPHYSAC_POLYGON: shape.vertexData.CreateRandomPolygon(dim.x, dim.y); break; Strange call to Random Polygon here
        // case MPHYSAC_CIRCLE: shape.vertexData.CreateRandomPolygon(dim.x, dim.y); break;Strange call to Random Polygon here
        default: break;
    }

    // Calculate centroid and moment of inertia
    Vector2f center;
    float area = 0.0f;
    float inertia = 0.0f;

    for (int i = 0; i < shape.vertexData.positions.size(); i++) {
        // Triangle vertices, third vertex implied as (0, 0)
        Vector2f p1 = shape.vertexData.positions[i];
        int nextIndex = (((i + 1) < shape.vertexData.positions.size()) ? (i + 1) : 0);
        Vector2f p2 = shape.vertexData.positions[nextIndex];

        float D = MPhysac::MathCrossVector2(p1, p2);
        float triangleArea = D/2;

        area += triangleArea;

        // Use area to weight the centroid average, not just vertex position
        center.x += triangleArea*PHYSAC_K*(p1.x + p2.x);
        center.y += triangleArea*PHYSAC_K*(p1.y + p2.y);

        float intx2 = p1.x*p1.x + p2.x*p1.x + p2.x*p2.x;
        float inty2 = p1.y*p1.y + p2.y*p1.y + p2.y*p2.y;
        inertia += (0.25f*PHYSAC_K*D)*(intx2 + inty2);
    }

    center.x *= 1.0f/area;
    center.y *= 1.0f/area;

    // Translate vertices to centroid (make the centroid (0, 0) for the polygon in model space)
    // Note: this is not really necessary
    for (int i = 0; i < shape.vertexData.positions.size(); i++) {
        shape.vertexData.positions[i].x -= center.x;
        shape.vertexData.positions[i].y -= center.y;
    }

    mass = density*area;
    inverseMass = ((mass != 0.0f) ? 1.0f/mass : 0.0f);
    inertia = density*inertia;
    inverseInertia = ((inertia != 0.0f) ? 1.0f/inertia : 0.0f);
    staticFriction = 0.4f;
    dynamicFriction = 0.2f;
    restitution = 0.0f;
    useGravity = true;
    isGrounded = false;
    freezeOrient = false;
    solidType = MPHYSAC_NONPASSABLE;
}

PhysicsManifold::PhysicsManifold(MPhysacBody *body1, MPhysacBody *body2) {
    // Initialize new manifold with generic values
    bodyA = body1;
    bodyB = body2;
    penetration = 0;
    normal = PHYSAC_VECTOR_ZERO;
    contacts[0] = PHYSAC_VECTOR_ZERO;
    contacts[1] = PHYSAC_VECTOR_ZERO;
    contactsCount = 0;
    restitution = 0.0f;
    dynamicFriction = 0.0f;
    staticFriction = 0.0f;
}
