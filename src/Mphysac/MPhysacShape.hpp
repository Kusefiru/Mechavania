/*******************************************************************************************
*   MPhysacShape.hpp
*
*   This file implements PhysicsShape from MPhysac in a C++ fashion.
*
********************************************************************************************/

#ifndef MPHYSAC_SHAPE_HPP
#define MPHYSAC_SHAPE_HPP

#include "MPhysac.hpp"

class MPhysacBody;

//----------------------------------------------------------------------------------
// Class Definition
//---------------------------------------------------------------------------------
class PolygonData {
    public:
        std::vector<Vector2> positions;     // Polygon vertex positions vectors
        std::vector<Vector2> normals;       // Polygon vertex normals vectors

        static PolygonData CreateRectanglePolygon(Vector2 pos, Vector2 size);
        static PolygonData CreateRandomPolygon(float radius, int sides);
};

class MPhysacShape {
    public:
        MPhysacShapeType type;                      // Physics shape type (circle or polygon)
        MPhysacBody *body;                          // Shape physics body reference
        float radius;                               // Circle shape radius (used for circle shapes)
        Matrix2x2 transform;                        // Vertices transform matrix 2x2
        PolygonData vertexData;
};

#endif