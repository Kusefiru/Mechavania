/*******************************************************************************************
*   MPhysacWorld.hpp
*
*   This file implements most physics body manipulation from MPhysac in a C++ fashion.
*
********************************************************************************************/

#ifndef MPHYSAC_WORLD_HPP
#define MPHYSAC_WORLD_HPP

#include "MPhysacBody.hpp"

class MPhysacWorld
{
    public:
        static MPhysacWorld& getInstance()
        {
            static MPhysacWorld instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

        //----------------------------------------------------------------------------------
        // Public Functions Declaration
        //----------------------------------------------------------------------------------
        void InitPhysics(void);                                                                                 // Initializes physics values, pointers and creates physics loop thread
        void RunPhysicsStep(void);                                                                              // Run physics step, to be used if PHYSICS_NO_THREADS is set in your main loop
        void SetPhysicsTimeStep(double delta);                                                                  // Sets physics fixed time step in milliseconds. 1.666666 by default
        bool IsPhysicsEnabled(void);                                                                            // Returns true if physics thread is currently enabled
        void SetPhysicsGravity(float x, float y);                                                               // Sets physics global gravity force
        MPhysacBody* CreatePhysicsBodyCircle(const Vector2f &pos, float radius, float density);                 // Creates a new circle physics body with generic parameters
        MPhysacBody* CreatePhysicsBodyRectangle(const Vector2f &pos, float width, float height, float density); // Creates a new rectangle physics body with generic parameters
        MPhysacBody* CreatePhysicsBodyPolygon(const Vector2f &pos, float radius, int sides, float density);     // Creates a new polygon physics body with generic parameters
        int GetMPhysacBodiesCount(void);                                                                        // Returns the current amount of created physics bodies    
        int GetManifoldsCount(void);
        MPhysacBody* GetMPhysacBody(int index);                                                                 // Returns a physics body of the bodies pool at a specific index
        void DestroyMPhysacBody(MPhysacBody *body);                                                             // Unitializes and destroy a physics body
        void ResetPhysics(void);                                                                                // Destroys created physics bodies and manifolds and resets global values
        void ClosePhysics(void);                                                                                // Unitializes physics pointers and closes physics loop thread

    private:
        MPhysacWorld() {}                    // Constructor (the {} brackets) are needed here.

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
        Vector2f gravityForce = { 0.0f, 9.81f };              // Physics world gravity force
        std::vector<MPhysacBody*> bodies;                    // Physics bodies pointers array
        std::vector<PhysicsManifold*> contacts;               // Physics bodies pointers array

        //----------------------------------------------------------------------------------
        // Private Functions Declaration
        //----------------------------------------------------------------------------------
        int FindAvailableBodyIndex();                                                                        // Finds a valid index for a new physics body initialization
        void *PhysicsLoop(void *arg);                                                                        // Physics loop thread function
        void PhysicsStep(void);                                                                              // Physics steps calculations (dynamics, collisions and position corrections)
        PhysicsManifold* CreatePhysicsManifold(MPhysacBody *a, MPhysacBody *b);                               // Creates a new physics manifold to solve collision
        void DestroyPhysicsManifold(PhysicsManifold *manifold);                                               // Unitializes and destroys a physics manifold
        void SolvePhysicsManifold(PhysicsManifold *manifold);                                                 // Solves a created physics manifold between two physics bodies
        void SolveCircleToCircle(PhysicsManifold *manifold);                                                  // Solves collision between two circle shape physics bodies
        void SolveCircleToPolygon(PhysicsManifold *manifold);                                                 // Solves collision between a circle to a polygon shape physics bodies
        void SolvePolygonToCircle(PhysicsManifold *manifold);                                                 // Solves collision between a polygon to a circle shape physics bodies
        void SolvePolygonToPolygon(PhysicsManifold *manifold);                                                // Solves collision between two polygons shape physics bodies
        void IntegratePhysicsForces(MPhysacBody *body);                                                       // Integrates physics forces into velocity
        void InitializePhysicsManifolds(PhysicsManifold *manifold);                                           // Initializes physics manifolds to solve collisions
        void IntegratePhysicsImpulses(PhysicsManifold *manifold);                                             // Integrates physics collisions impulses to solve collisions
        void IntegratePhysicsVelocity(MPhysacBody *body);                                                     // Integrates physics velocity into position and forces
        void CorrectPhysicsPositions(PhysicsManifold *manifold);                                              // Corrects physics bodies positions based on manifolds collision information
        Vector2f GetSupport(const MPhysacShape &shape, const Vector2f &dir);
        float FindAxisLeastPenetration(int *faceIndex, const MPhysacShape &shapeA, const MPhysacShape &shapeB);            // Finds polygon shapes axis least penetration
        void FindIncidentFace(Vector2f *v0, Vector2f *v1, const MPhysacShape &ref, const MPhysacShape &inc, int index);      // Finds two polygon shapes incident face
        int Clip(const Vector2f &normal, float clip, Vector2f *faceA, Vector2f *faceB);                                // Calculates clipping based on a normal and two faces
        bool BiasGreaterThan(float valueA, float valueB);                                                    // Check if values are between bias range
        Vector2f TriangleBarycenter(const Vector2f &v1, const Vector2f &v2, const Vector2f &v3);                                      // Returns the barycenter of a triangle given by 3 points

        void InitTimer(void);                                                                                // Initializes hi-resolution MONOTONIC timer
        unsigned long long int GetTimeCount(void);                                                           // Get hi-res MONOTONIC time measure in mseconds
        double GetCurrentTime(void);                                                                         // Get current time measure in milliseconds

    public:
        MPhysacWorld(MPhysacWorld const&)        = delete;
        void operator=(MPhysacWorld const&)      = delete;
};

#endif