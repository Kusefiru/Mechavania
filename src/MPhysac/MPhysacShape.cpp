/*******************************************************************************************
*   MPhysacShape.cpp
*
*   This file implements PhysicsShape from MPhysac in a C++ fashion.
*
********************************************************************************************/

#include <vector>

#include "MPhysac/MPhysacShape.hpp"

// Creates a random polygon shape with max vertex distance from polygon pivot
void PolygonData::CreateRandomPolygon(float radius, int sides) {
    // Calculate polygon vertices positions
    for (int i = 0; i < sides; i++) {
        positions.push_back(Vector2f(cosf(360.0f/sides*(float)i*PHYSAC_DEG2RAD)*radius, sinf(360.0f/(float)sides*i*PHYSAC_DEG2RAD)*radius));
    }

    // Calculate polygon faces normals
    for (int i = 0; i < sides; i++) {
        int nextIndex = (((i + 1) < sides) ? (i + 1) : 0);
        Vector2f face = positions[nextIndex] - positions[i];

        normals.push_back(Vector2f(face.y, -face.x));
        MPhysac::MathNormalize(&normals[i]);
    }
}

// Creates a rectangle polygon shape based on a min and max positions
void PolygonData::CreateRectanglePolygon(const Vector2f& pos, const Vector2f& size) {
    // Calculate polygon vertices positions
    positions.push_back(Vector2f(pos.x + size.x/2, pos.y - size.y/2));
    positions.push_back(Vector2f(pos.x + size.x/2, pos.y + size.y/2));
    positions.push_back(Vector2f(pos.x - size.x/2, pos.y + size.y/2));
    positions.push_back(Vector2f(pos.x - size.x/2, pos.y - size.y/2));

    // Calculate polygon faces normals
    for (int i = 0; i < positions.size(); i++) {
        int nextIndex = (((i + 1) < positions.size()) ? (i + 1) : 0);
        Vector2f face = positions[nextIndex] - positions[i];

        normals.push_back(Vector2f(face.y, -face.x));
        MPhysac::MathNormalize(&normals[i]);
    }
}