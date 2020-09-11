/*******************************************************************************************
*
*   Main.cpp
*
*
********************************************************************************************/


#define SDL_MAIN_HANDLED

#include <iostream>

#include "SDL.h"
#include "MCamera/MCamera.hpp"
#include "Entity/PlayerEntity.hpp"

#define VELOCITY    0.5f

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Initialize physics and default physics bodies
    glMPhysac->InitPhysics();

    // Create floor and walls
    MPhysacBody *floor = glMPhysac->CreatePhysicsBodyRectangle(Vector2f(screenWidth/2, screenHeight), screenWidth*2, 100, 10);
    MPhysacBody *wallLeft = glMPhysac->CreatePhysicsBodyRectangle(Vector2f(-5-screenWidth/2, screenHeight/2), 10, screenHeight, 10);
    MPhysacBody *wallRight = glMPhysac->CreatePhysicsBodyRectangle(Vector2f((screenWidth*1.5) + 5, screenHeight/2), 10, screenHeight, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->solidType = MPHYSAC_GROUND;
    floor->enabled = false;
    wallLeft->solidType = MPHYSAC_GROUND;
    wallLeft->enabled = false;
    wallRight->solidType = MPHYSAC_GROUND;
    wallRight->enabled = false;

    // Create movement physics body
    MPhysacBody *body = glMPhysac->CreatePhysicsBodyRectangle(Vector2f(screenWidth/2, screenHeight/2), 50, 50, 1);
    body->solidType = MPHYSAC_PASSABLE;
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts

    PlayerEntity player = PlayerEntity(Vector2f(screenWidth/2+10, screenHeight/2), 60, 60, 1, "");
    player.body->freezeOrient = true;

    MCamera mainCamera;
    //mainCamera.setFollowedPhysicsBody(player.body);
    //mainCamera.getCamera().target = (Vector2){body->position.x, body->position.y};

    //SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (false)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        glMPhysac->RunPhysicsStep();

        if (true)    // Reset physics input (R pressed)
        {
            // Reset movement physics body position, velocity and rotation
            player.body->position = Vector2f(screenWidth/2, screenHeight/2);
            player.body->velocity = Vector2f();
            player.body->SetMPhysacBodyRotation(0);
        }

        player.body->dynamicFriction = 1;

        // Horizontal movement input
        if (true) player.body->velocity.x = VELOCITY; // KEY_RIGHT
        else if (false) player.body->velocity.x = -VELOCITY; // KEY_LEFT

        //mainCamera.update();
        //mainCamera.getCamera().target = (Vector2){body->position.x, body->position.y};
        mainCamera.update();

        // Vertical movement input checking if player physics body is grounded
        if (true && player.body->isGrounded) // Key up
            player.body->velocity.y = -VELOCITY*4;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // BeginDrawing();

        //     ClearBackground(BLACK);

        //     DrawFPS(screenWidth - 90, screenHeight - 30);

        //     BeginMode2D(mainCamera.getCamera());

        //     // Draw created physics bodies
        //     int bodiesCount = glMPhysac->GetMPhysacBodiesCount();
        //     for (int i = 0; i < bodiesCount; i++)
        //     {
        //         MPhysacBody *body = glMPhysac->GetMPhysacBody(i);

        //         int vertexCount = body->GetMPhysacBodyShapeVerticesCount();
        //         for (int j = 0; j < vertexCount; j++)
        //         {
        //             // Get physics bodies shape vertices to draw lines
        //             // Note: GetPhysicsShapeVertex() already calculates rotation transformations
        //             Vector2 vertexA = body->GetMPhysacBodyShapeVertex(j);

        //             int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
        //             Vector2 vertexB = body->GetMPhysacBodyShapeVertex(jj);

        //             DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
        //         }
        //     }

            /*for (int k = 0; k < (player.getPhysicsBody()->shape.vertexData.vertexCount) ; k++){
                Vector2 vertexC = glMPhysac->GetPhysicsShapeVertex(player.getPhysicsBody(), k);
                int kk = (((k + 1) < player.getPhysicsBody()->shape.vertexData.vertexCount) ? (k + 1) : 0);   // Get next vertex or first to close the shape
                Vector2 vertexD = glMPhysac->GetPhysicsShapeVertex(player.getPhysicsBody(), kk);

                DrawLineV(vertexC, vertexD, RED);     // Draw a line between two vertex positions
            }*/

        //     EndMode2D();

        //     DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
        //     DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);
        //     DrawText(FormatText("%d bodies", bodiesCount), 10, 50, 10, WHITE);
        //     DrawText(FormatText("%d manifolds", glMPhysac->GetManifoldsCount()), 10, 70, 10, WHITE);
        //     //DrawText(FormatText("%d", player.body->freezeOrient), 10, 70, 10, WHITE);

        // EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    glMPhysac->ClosePhysics();       // Unitialize physics

    // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}