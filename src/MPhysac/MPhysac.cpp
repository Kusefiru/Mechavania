/*******************************************************************************************
*   MPhysac.cpp
*
*   This file implements Physac in a C++ fashion.
*
********************************************************************************************/

#include "MPhysac.hpp"

// Returns the cross product of a vector and a value
Vector2f MPhysac::MathCross(float value, Vector2f vector) {
    return Vector2f(-value*vector.y, value*vector.x);
}

// Returns the cross product of two vectors
float MPhysac::MathCrossVector2(Vector2f v1, Vector2f v2) {
    return (v1.x*v2.y - v1.y*v2.x);
}

// Returns the len square root of a vector
float MPhysac::MathLenSqr(Vector2f vector) {
    return (vector.x*vector.x + vector.y*vector.y);
}

// Returns the dot product of two vectors
float MPhysac::MathDot(Vector2f v1, Vector2f v2) {
    return (v1.x*v2.x + v1.y*v2.y);
}

// Returns the square root of distance between two vectors
float MPhysac::DistSqr(Vector2f v1, Vector2f v2) {
    Vector2f dir = v1 - v2;
    return MathDot(dir, dir);
}

// Returns the normalized values of a vector
void MPhysac::MathNormalize(Vector2f *vector) {
    float length, ilength;

    Vector2f aux = *vector;
    length = sqrtf(aux.x*aux.x + aux.y*aux.y);

    if (length == 0) length = 1.0f;

    ilength = 1.0f/length;

    vector->x *= ilength;
    vector->y *= ilength;
}

// Creates a matrix 2x2 from a given radians value
Matrix2x2 MPhysac::Mat2Radians(float radians) {
    float c = cosf(radians);
    float s = sinf(radians);

    return { c, -s, s, c };
}

// Set values from radians to a created matrix 2x2
void MPhysac::Mat2Set(Matrix2x2 *matrix, float radians) {
    float cos = cosf(radians);
    float sin = sinf(radians);

    matrix->m00 = cos;
    matrix->m01 = -sin;
    matrix->m10 = sin;
    matrix->m11 = cos;
}

// Returns the transpose of a given matrix 2x2
Matrix2x2 MPhysac::Mat2Transpose(Matrix2x2 matrix) {
    return { matrix.m00, matrix.m10, matrix.m01, matrix.m11 };
}

// Multiplies a vector by a matrix 2x2
Vector2f MPhysac::Mat2MultiplyVector2(Matrix2x2 matrix, Vector2f vector) {
    return Vector2f(matrix.m00*vector.x + matrix.m01*vector.y, matrix.m10*vector.x + matrix.m11*vector.y);
}