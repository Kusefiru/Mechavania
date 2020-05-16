/**********************************************************************************************
*
*   Mphysac.hpp
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

#include <stdlib.h>                 // Required for: malloc(), free(), srand(), rand()
#include <math.h>                   // Required for: cosf(), sinf(), fabs(), sqrtf()

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

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define PHYSAC_MAX_BODIES               64
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
// Types and Structures Definition
// NOTE: Below types are required for PHYSAC_STANDALONE usage
//----------------------------------------------------------------------------------
typedef enum PhysicsShapeType { PHYSICS_CIRCLE, PHYSICS_POLYGON } PhysicsShapeType;

// Previously defined to be used in PhysicsShape struct as circular dependencies
typedef struct PhysicsBodyData *PhysicsBody;

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

typedef struct PolygonData {
    unsigned int vertexCount;                   // Current used vertex and normals count
    Vector2 positions[PHYSAC_MAX_VERTICES];     // Polygon vertex positions vectors
    Vector2 normals[PHYSAC_MAX_VERTICES];       // Polygon vertex normals vectors
} PolygonData;

typedef struct PhysicsShape {
    PhysicsShapeType type;                      // Physics shape type (circle or polygon)
    PhysicsBody body;                           // Shape physics body reference
    float radius;                               // Circle shape radius (used for circle shapes)
    Matrix2x2 transform;                        // Vertices transform matrix 2x2
    PolygonData vertexData;                     // Polygon shape vertices position and normals data (just used for polygon shapes)
} PhysicsShape;

typedef struct PhysicsBodyData {
    unsigned int id;                            // Reference unique identifier
    bool enabled;                               // Enabled dynamics state (collisions are calculated anyway)
    Vector2 position;                           // Physics body shape pivot
    Vector2 velocity;                           // Current linear velocity applied to position
    Vector2 force;                              // Current linear force (reset to 0 every step)
    float angularVelocity;                      // Current angular velocity applied to orient
    float torque;                               // Current angular force (reset to 0 every step)
    float orient;                               // Rotation in radians
    float inertia;                              // Moment of inertia
    float inverseInertia;                       // Inverse value of inertia
    float mass;                                 // Physics body mass
    float inverseMass;                          // Inverse value of mass
    float staticFriction;                       // Friction when the body has not movement (0 to 1)
    float dynamicFriction;                      // Friction when the body has movement (0 to 1)
    float restitution;                          // Restitution coefficient of the body (0 to 1)
    bool useGravity;                            // Apply gravity force to dynamics
    bool isGrounded;                            // Physics grounded on other body state
    bool freezeOrient;                          // Physics rotation constraint
    PhysicsShape shape;                         // Physics body shape information (type, radius, vertices, normals)
} PhysicsBodyData;

typedef struct PhysicsManifoldData {
    unsigned int id;                            // Reference unique identifier
    PhysicsBody bodyA;                          // Manifold first physics body reference
    PhysicsBody bodyB;                          // Manifold second physics body reference
    float penetration;                          // Depth of penetration from collision
    Vector2 normal;                             // Normal direction vector from 'a' to 'b'
    Vector2 contacts[2];                        // Points of contact during collision
    unsigned int contactsCount;                 // Current collision number of contacts
    float restitution;                          // Mixed restitution during collision
    float dynamicFriction;                      // Mixed dynamic friction during collision
    float staticFriction;                       // Mixed static friction during collision
} PhysicsManifoldData, *PhysicsManifold;

class Mphysac
{
    public:
        static Mphysac& getInstance()
        {
            static Mphysac instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

        //----------------------------------------------------------------------------------
        // Public Functions Declaration
        //----------------------------------------------------------------------------------
        void InitPhysics(void);                                                                           // Initializes physics values, pointers and creates physics loop thread
        void RunPhysicsStep(void);                                                                        // Run physics step, to be used if PHYSICS_NO_THREADS is set in your main loop
        void SetPhysicsTimeStep(double delta);                                                            // Sets physics fixed time step in milliseconds. 1.666666 by default
        bool IsPhysicsEnabled(void);                                                                      // Returns true if physics thread is currently enabled
        void SetPhysicsGravity(float x, float y);                                                         // Sets physics global gravity force
        PhysicsBody CreatePhysicsBodyCircle(Vector2 pos, float radius, float density);                    // Creates a new circle physics body with generic parameters
        PhysicsBody CreatePhysicsBodyRectangle(Vector2 pos, float width, float height, float density);    // Creates a new rectangle physics body with generic parameters
        PhysicsBody CreatePhysicsBodyPolygon(Vector2 pos, float radius, int sides, float density);        // Creates a new polygon physics body with generic parameters
        void PhysicsAddForce(PhysicsBody body, Vector2 force);                                            // Adds a force to a physics body
        void PhysicsAddTorque(PhysicsBody body, float amount);                                            // Adds an angular force to a physics body
        void PhysicsShatter(PhysicsBody body, Vector2 position, float force);                             // Shatters a polygon shape physics body to little physics bodies with explosion force
        int GetPhysicsBodiesCount(void);                                                                  // Returns the current amount of created physics bodies    
        PhysicsBody GetPhysicsBody(int index);                                                            // Returns a physics body of the bodies pool at a specific index
        int GetPhysicsShapeType(int index);                                                               // Returns the physics body shape type (PHYSICS_CIRCLE or PHYSICS_POLYGON)
        int GetPhysicsShapeVerticesCount(int index);                                                      // Returns the amount of vertices of a physics body shape
        Vector2 GetPhysicsShapeVertex(PhysicsBody body, int vertex);                                      // Returns transformed position of a body shape (body position + vertex transformed position)
        void SetPhysicsBodyRotation(PhysicsBody body, float radians);                                     // Sets physics body shape transform based on radians parameter
        void DestroyPhysicsBody(PhysicsBody body);                                                        // Unitializes and destroy a physics body
        void ResetPhysics(void);                                                                          // Destroys created physics bodies and manifolds and resets global values
        void ClosePhysics(void);                                                                          // Unitializes physics pointers and closes physics loop thread

    private:
        Mphysac() {}                    // Constructor (the {} brackets) are needed here.

        //----------------------------------------------------------------------------------
        // Static Variables Definition
        //----------------------------------------------------------------------------------
        #ifndef PHYSAC_NO_THREADS
        pthread_t physicsThreadId;                           // Physics thread id
        #endif
        unsigned int usedMemory = 0;                         // Total allocated dynamic memory
        bool physicsThreadEnabled = false;                   // Physics thread enabled state
        double baseTime = 0.0;                               // Offset time for MONOTONIC clock
        double startTime = 0.0;                              // Start time in milliseconds
        double deltaTime = 1.0/60.0/10.0 * 1000;             // Delta time used for physics steps, in milliseconds
        double currentTime = 0.0;                            // Current time in milliseconds
        unsigned long long int frequency = 0;                // Hi-res clock frequency

        double accumulator = 0.0;                            // Physics time step delta time accumulator
        unsigned int stepsCount = 0;                         // Total physics steps processed
        Vector2 gravityForce = { 0.0f, 9.81f };              // Physics world gravity force
        PhysicsBody bodies[PHYSAC_MAX_BODIES];               // Physics bodies pointers array
        unsigned int physicsBodiesCount = 0;                 // Physics world current bodies counter
        PhysicsManifold contacts[PHYSAC_MAX_MANIFOLDS];      // Physics bodies pointers array
        unsigned int physicsManifoldsCount = 0;              // Physics world current manifolds counter

        //----------------------------------------------------------------------------------
        // Private Functions Declaration
        //----------------------------------------------------------------------------------
        int FindAvailableBodyIndex();                                                                        // Finds a valid index for a new physics body initialization
        PolygonData CreateRandomPolygon(float radius, int sides);                                            // Creates a random polygon shape with max vertex distance from polygon pivot
        PolygonData CreateRectanglePolygon(Vector2 pos, Vector2 size);                                       // Creates a rectangle polygon shape based on a min and max positions
        void *PhysicsLoop(void *arg);                                                                        // Physics loop thread function
        void PhysicsStep(void);                                                                              // Physics steps calculations (dynamics, collisions and position corrections)
        int FindAvailableManifoldIndex();                                                                    // Finds a valid index for a new manifold initialization
        PhysicsManifold CreatePhysicsManifold(PhysicsBody a, PhysicsBody b);                                 // Creates a new physics manifold to solve collision
        void DestroyPhysicsManifold(PhysicsManifold manifold);                                               // Unitializes and destroys a physics manifold
        void SolvePhysicsManifold(PhysicsManifold manifold);                                                 // Solves a created physics manifold between two physics bodies
        void SolveCircleToCircle(PhysicsManifold manifold);                                                  // Solves collision between two circle shape physics bodies
        void SolveCircleToPolygon(PhysicsManifold manifold);                                                 // Solves collision between a circle to a polygon shape physics bodies
        void SolvePolygonToCircle(PhysicsManifold manifold);                                                 // Solves collision between a polygon to a circle shape physics bodies
        void SolvePolygonToPolygon(PhysicsManifold manifold);                                                // Solves collision between two polygons shape physics bodies
        void IntegratePhysicsForces(PhysicsBody body);                                                       // Integrates physics forces into velocity
        void InitializePhysicsManifolds(PhysicsManifold manifold);                                           // Initializes physics manifolds to solve collisions
        void IntegratePhysicsImpulses(PhysicsManifold manifold);                                             // Integrates physics collisions impulses to solve collisions
        void IntegratePhysicsVelocity(PhysicsBody body);                                                     // Integrates physics velocity into position and forces
        void CorrectPhysicsPositions(PhysicsManifold manifold);                                              // Corrects physics bodies positions based on manifolds collision information
        Vector2 GetSupport(PhysicsShape shape, Vector2 dir);
        float FindAxisLeastPenetration(int *faceIndex, PhysicsShape shapeA, PhysicsShape shapeB);            // Finds polygon shapes axis least penetration
        void FindIncidentFace(Vector2 *v0, Vector2 *v1, PhysicsShape ref, PhysicsShape inc, int index);      // Finds two polygon shapes incident face
        int Clip(Vector2 normal, float clip, Vector2 *faceA, Vector2 *faceB);                                // Calculates clipping based on a normal and two faces
        bool BiasGreaterThan(float valueA, float valueB);                                                    // Check if values are between bias range
        Vector2 TriangleBarycenter(Vector2 v1, Vector2 v2, Vector2 v3);                                      // Returns the barycenter of a triangle given by 3 points

        void InitTimer(void);                                                                                // Initializes hi-resolution MONOTONIC timer
        unsigned long long int GetTimeCount(void);                                                           // Get hi-res MONOTONIC time measure in mseconds
        double GetCurrentTime(void);                                                                         // Get current time measure in milliseconds

        // Math functions
        Vector2 MathCross(float value, Vector2 vector);                                                      // Returns the cross product of a vector and a value
        float MathCrossVector2(Vector2 v1, Vector2 v2);                                                      // Returns the cross product of two vectors
        float MathLenSqr(Vector2 vector);                                                                    // Returns the len square root of a vector
        float MathDot(Vector2 v1, Vector2 v2);                                                               // Returns the dot product of two vectors
        inline float DistSqr(Vector2 v1, Vector2 v2);                                                        // Returns the square root of distance between two vectors
        void MathNormalize(Vector2 *vector);                                                                 // Returns the normalized values of a vector
        #if defined(PHYSAC_STANDALONE)
        Vector2 Vector2Add(Vector2 v1, Vector2 v2);                                                          // Returns the sum of two given vectors
        Vector2 Vector2Subtract(Vector2 v1, Vector2 v2);                                                     // Returns the subtract of two given vectors
        #endif

        Matrix2x2 Mat2Radians(float radians);                                                                 // Creates a matrix 2x2 from a given radians value
        void Mat2Set(Matrix2x2 *matrix, float radians);                                                       // Set values from radians to a created matrix 2x2
        inline Matrix2x2 Mat2Transpose(Matrix2x2 matrix);                                                     // Returns the transpose of a given matrix 2x2
        inline Vector2 Mat2MultiplyVector2(Matrix2x2 matrix, Vector2 vector);                                 // Multiplies a vector by a matrix 2x2

    public:
        Mphysac(Mphysac const&)             = delete;
        void operator=(Mphysac const&)      = delete;
};

#endif