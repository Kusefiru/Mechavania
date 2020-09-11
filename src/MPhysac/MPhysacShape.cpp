/*******************************************************************************************
*   MPhysacShape.cpp
*
*   This file implements PhysicsShape from MPhysac in a C++ fashion.
*
********************************************************************************************/

#include "MPhysacShape.hpp"

// Creates a random polygon shape with max vertex distance from polygon pivot
void PolygonData::CreateRandomPolygon(float radius, int sides)
{
    // Calculate polygon vertices positions
    for (int i = 0; i < sides; i++)
    {
        positions.push_back((Vector2){cosf(360.0f/sides*i*PHYSAC_DEG2RAD)*radius, sinf(360.0f/sides*i*PHYSAC_DEG2RAD)*radius});
    }

    // Calculate polygon faces normals
    for (int i = 0; i < sides; i++)
    {
        int nextIndex = (((i + 1) < sides) ? (i + 1) : 0);
        Vector2 face = Vector2Subtract(positions[nextIndex], positions[i]);

        normals.push_back((Vector2){ face.y, -face.x });
        MPhysac::MathNormalize(&normals[i]);
    }
}

// Creates a rectangle polygon shape based on a min and max positions
void PolygonData::CreateRectanglePolygon(Vector2 pos, Vector2 size)
{
    // Calculate polygon vertices positions
    positions.push_back((Vector2){ pos.x + size.x/2, pos.y - size.y/2 });
    positions.push_back((Vector2){ pos.x + size.x/2, pos.y + size.y/2 });
    positions.push_back((Vector2){ pos.x - size.x/2, pos.y + size.y/2 });
    positions.push_back((Vector2){ pos.x - size.x/2, pos.y - size.y/2 });

    // Calculate polygon faces normals
    for (int i = 0; i < positions.size(); i++)
    {
        int nextIndex = (((i + 1) < positions.size()) ? (i + 1) : 0);
        Vector2 face = Vector2Subtract(positions[nextIndex], positions[i]);

        normals.push_back((Vector2){ face.y, -face.x });
        MPhysac::MathNormalize(&normals[i]);
    }
}