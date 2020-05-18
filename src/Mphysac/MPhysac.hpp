/**********************************************************************************************
*
*   MPhysac.hpp
*
*   This file implements Physac in a C++ fashion. Based on :
*
*   Physac v1.0 - 2D Physics library for videogames
*
*   DESCRIPTION:
*
*   Physac is a small 2D physics engine written in pure C. The engine uses a fixed time-step thread loop
*   to simluate physics. A physics step contains the following phases: get collision information,
*   apply dynamics, collision solving and position correction. It uses a very simple struct for physic
*   bodies with a position vector to be used in any 3D rendering API.
*
*   CONFIGURATION:
*
*   #define PHYSAC_IMPLEMENTATION
*       Generates the implementation of the library into the included file.
*       If not defined, the library is in header only mode and can be included in other headers
*       or source files without problems. But only ONE file should hold the implementation.
*
*   #define PHYSAC_NO_THREADS
*       The generated implementation won't include pthread library and user must create a secondary thread to call PhysicsThread().
*       It is so important that the thread where PhysicsThread() is called must not have v-sync or any other CPU limitation.
*
*   #define PHYSAC_STANDALONE
*       Avoid raylib.h header inclusion in this file. Data types defined on raylib are defined
*       internally in the library and input management and drawing functions must be provided by
*       the user (check library implementation for further details).
*
*   #define PHYSAC_DEBUG
*       Traces log messages when creating and destroying physics bodies and detects errors in physics
*       calculations and reference exceptions; it is useful for debug purposes
*
*   #define PHYSAC_MALLOC()
*   #define PHYSAC_FREE()
*       You can define your own malloc/free implementation replacing stdlib.h malloc()/free() functions.
*       Otherwise it will include stdlib.h and use the C standard library malloc()/free() function.
*
*
*   NOTE 1: Physac requires multi-threading, when InitPhysics() a second thread is created to manage physics calculations.
*   NOTE 2: Physac requires static C library linkage to avoid dependency on MinGW DLL (-static -lpthread)
*
*   Use the following code to compile:
*   gcc -o $(NAME_PART).exe $(FILE_NAME) -s -static -lraylib -lpthread -lopengl32 -lgdi32 -lwinmm -std=c99
*
*   VERY THANKS TO:
*       Ramon Santamaria (github: @raysan5)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2016-2018 Victor Fisac (github: @victorfisac)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef MECHA_PHYSAC_HPP
#define MECHA_PHYSAC_HPP

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS

// Allow custom memory allocators
#ifndef PHYSAC_MALLOC
    #define PHYSAC_MALLOC(size)         malloc(size)
#endif
#ifndef PHYSAC_FREE
    #define PHYSAC_FREE(ptr)            free(ptr)
#endif

// Allow custom memory allocators
#ifndef PHYSAC_MALLOC
    #define PHYSAC_MALLOC(size)         malloc(size)
#endif
#ifndef PHYSAC_FREE
    #define PHYSAC_FREE(ptr)            free(ptr)
#endif

/***********************************************************************************
*
*   PHYSAC IMPLEMENTATION
*
************************************************************************************/

extern "C" {
#ifndef PHYSAC_NO_THREADS
    #include <pthread.h>            // Required for: pthread_t, pthread_create()
#endif

// Support TRACELOG macros
#ifdef PHYSAC_DEBUG
    #include <stdio.h>              // Required for: printf()
    #define TRACELOG(...) printf(__VA_ARGS__)
#else
    #define TRACELOG(...) (void)0
#endif

#ifndef PHYSAC_STANDALONE
    #include "raymath.h"            // Required for: Vector2Add(), Vector2Subtract()
#endif

// Time management functionality
#include <time.h>                   // Required for: time(), clock_gettime()
#ifdef _WIN32
    // Functions required to query time on Windows
    int __stdcall QueryPerformanceCounter(unsigned long long int *lpPerformanceCount);
    int __stdcall QueryPerformanceFrequency(unsigned long long int *lpFrequency);
#elif defined(__linux__)
    #if _POSIX_C_SOURCE < 199309L
        #undef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 199309L // Required for CLOCK_MONOTONIC if compiled with c99 without gnu ext.
    #endif
    #include <sys/time.h>           // Required for: timespec
#elif defined(__APPLE__)            // macOS also defines __MACH__
    #include <mach/mach_time.h>     // Required for: mach_absolute_time()
#endif
}

#include <stdlib.h>                 // Required for: malloc(), free(), srand(), rand()
#include <math.h>                   // Required for: cosf(), sinf(), fabs(), sqrtf()
#include <vector>

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define PHYSAC_MAX_BODIES               128
#define PHYSAC_MAX_MANIFOLDS            4096
#define PHYSAC_MAX_VERTICES             24
#define PHYSAC_CIRCLE_VERTICES          24

#define PHYSAC_COLLISION_ITERATIONS     100
#define PHYSAC_PENETRATION_ALLOWANCE    0.05f
#define PHYSAC_PENETRATION_CORRECTION   0.4f

#define PHYSAC_PI                       3.14159265358979323846
#define PHYSAC_DEG2RAD                  (PHYSAC_PI/180.0f)

#define physacmin(a,b)      (((a)<(b))?(a):(b))
#define physacmax(a,b)      (((a)>(b))?(a):(b))
#define PHYSAC_FLT_MAX      3.402823466e+38f
#define PHYSAC_EPSILON      0.000001f
#define PHYSAC_K            1.0f/3.0f
#define PHYSAC_VECTOR_ZERO  (Vector2){ 0.0f, 0.0f }

//----------------------------------------------------------------------------------
// Data Types Structure Definition
//----------------------------------------------------------------------------------

// Matrix2x2 type (used for polygon shape rotation matrix)
typedef struct Matrix2x2 {
    float m00;
    float m01;
    float m10;
    float m11;
} Matrix2x2;

enum MPhysacShapeType { MPHYSAC_BOX, MPHYSAC_CIRCLE, MPHYSAC_POLYGON };

// Solid type for physics calculation :
// - GROUND : for fixed ground solids
// - PASSABLE : affected by gravity, collides with GROUND solids
// - NONPASSABLE : affected by gravity, collides with GROUND and others NONPASSABLE solids
// defaults new solids are NONPASSABLE
enum MPhysacSolidType { MPHYSAC_GROUND, MPHYSAC_PASSABLE, MPHYSAC_NONPASSABLE };

class MPhysac {
    public:
        // Math functions
        static Vector2 MathCross(float value, Vector2 vector);                                                      // Returns the cross product of a vector and a value
        static float MathCrossVector2(Vector2 v1, Vector2 v2);                                                      // Returns the cross product of two vectors
        static float MathLenSqr(Vector2 vector);                                                                    // Returns the len square root of a vector
        static float MathDot(Vector2 v1, Vector2 v2);                                                               // Returns the dot product of two vectors
        static float DistSqr(Vector2 v1, Vector2 v2);                                                               // Returns the square root of distance between two vectors
        static void MathNormalize(Vector2 *vector);                                                                 // Returns the normalized values of a vector

        static Matrix2x2 Mat2Radians(float radians);                                                                 // Creates a matrix 2x2 from a given radians value
        static void Mat2Set(Matrix2x2 *matrix, float radians);                                                       // Set values from radians to a created matrix 2x2
        static Matrix2x2 Mat2Transpose(Matrix2x2 matrix);                                                            // Returns the transpose of a given matrix 2x2
        static Vector2 Mat2MultiplyVector2(Matrix2x2 matrix, Vector2 vector);                                        // Multiplies a vector by a matrix 2x2
};

#endif