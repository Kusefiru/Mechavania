/*******************************************************************************************
*   MPhysacShape.cpp
*
*   This file implements PhysicsShape from MPhysac in a C++ fashion.
*
********************************************************************************************/

#include "MPhysacShape.hpp"

// Creates a random polygon shape with max vertex distance from polygon pivot
PolygonData PolygonData::CreateRandomPolygon(float radius, int sides)
{
    PolygonData data;

    // Calculate polygon vertices positions
    for (int i = 0; i < sides; i++)
    {
        data.positions[i].x = cosf(360.0f/sides*i*PHYSAC_DEG2RAD)*radius;
        data.positions[i].y = sinf(360.0f/sides*i*PHYSAC_DEG2RAD)*radius;
    }

    // Calculate polygon faces normals
    for (int i = 0; i < sides; i++)
    {
        int nextIndex = (((i + 1) < sides) ? (i + 1) : 0);
        Vector2 face = Vector2Subtract(data.positions[nextIndex], data.positions[i]);

        data.normals[i] = (Vector2){ face.y, -face.x };
        MPhysac::MathNormalize(&data.normals[i]);
    }

    return data;
}

// Creates a rectangle polygon shape based on a min and max positions
PolygonData PolygonData::CreateRectanglePolygon(Vector2 pos, Vector2 size)
{
    PolygonData data;

    // Calculate polygon vertices positions
    data.positions[0] = (Vector2){ pos.x + size.x/2, pos.y - size.y/2 };
    data.positions[1] = (Vector2){ pos.x + size.x/2, pos.y + size.y/2 };
    data.positions[2] = (Vector2){ pos.x - size.x/2, pos.y + size.y/2 };
    data.positions[3] = (Vector2){ pos.x - size.x/2, pos.y - size.y/2 };

    // Calculate polygon faces normals
    for (int i = 0; i < data.positions.size(); i++)
    {
        int nextIndex = (((i + 1) < data.positions.size()) ? (i + 1) : 0);
        Vector2 face = Vector2Subtract(data.positions[nextIndex], data.positions[i]);

        data.normals[i] = (Vector2){ face.y, -face.x };
        MPhysac::MathNormalize(&data.normals[i]);
    }

    return data;
}