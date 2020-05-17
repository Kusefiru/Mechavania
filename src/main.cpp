/*******************************************************************************************
*
*   Main.cpp
*
*
********************************************************************************************/
#include <iostream>

#include "Libs.hpp"
#include "Mcamera.hpp"
#include "PlayerEntity.hpp"

#define VELOCITY    0.5f

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Mechavania");

    // Initialize physics and default physics bodies
    glMPhysac->InitPhysics();

    // Create floor and walls
    MPhysacBody *floor = glMPhysac->CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight }, screenWidth*2, 100, 10);
    MPhysacBody *wallLeft = glMPhysac->CreatePhysicsBodyRectangle((Vector2){ -5-screenWidth/2, screenHeight/2 }, 10, screenHeight, 10);
    MPhysacBody *wallRight = glMPhysac->CreatePhysicsBodyRectangle((Vector2){ (screenWidth*1.5) + 5, screenHeight/2 }, 10, screenHeight, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    MPhysacBody *body = glMPhysac->CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight/2 }, 50, 50, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
    body->useGravity = false;

    //PlayerEntity player = PlayerEntity((Vector2){ screenWidth/2+10, screenHeight/2 }, 60, 60, 1, "");
    //player.body->freezeOrient = true;

    //Mcamera mainCamera;
    //mainCamera.setFollowedPhysicsBody(player.body);
    //mainCamera.getCamera().target = (Vector2){body->position.x, body->position.y};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        glMPhysac->RunPhysicsStep();

        /*if (IsKeyPressed('R'))    // Reset physics input
        {
            // Reset movement physics body position, velocity and rotation
            player.body->position = (Vector2){ screenWidth/2, screenHeight/2 };
            player.body->velocity = (Vector2){ 0, 0 };
            player.body->SetMPhysacBodyRotation(0);
        }

        player.body->dynamicFriction = 1;

        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) player.body->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_LEFT)) player.body->velocity.x = -VELOCITY;
        */
        if (IsKeyDown(KEY_RIGHT)) body->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_LEFT)) body->velocity.x = -VELOCITY;
        if (IsKeyDown(KEY_UP)) body->velocity.y = -VELOCITY;
        else if (IsKeyDown(KEY_DOWN)) body->velocity.y = VELOCITY;

        /*
        //mainCamera.update();
        //mainCamera.getCamera().target = (Vector2){body->position.x, body->position.y};
        mainCamera.update();

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && player.body->isGrounded) player.body->velocity.y = -VELOCITY*4;*/
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);

            //BeginMode2D(mainCamera.getCamera());

            // Draw created physics bodies
            int bodiesCount = glMPhysac->GetMPhysacBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                MPhysacBody *body = glMPhysac->GetMPhysacBody(i);

                int vertexCount = body->GetMPhysacBodyShapeVerticesCount();
                for (int j = 0; j < vertexCount; j++)
                {
                    // Get physics bodies shape vertices to draw lines
                    // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                    Vector2 vertexA = body->GetMPhysacBodyShapeVertex(j);

                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                    Vector2 vertexB = body->GetMPhysacBodyShapeVertex(jj);

                    DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
                }
            }

            /*for (int k = 0; k < (player.getPhysicsBody()->shape.vertexData.vertexCount) ; k++){
                Vector2 vertexC = glMPhysac->GetPhysicsShapeVertex(player.getPhysicsBody(), k);
                int kk = (((k + 1) < player.getPhysicsBody()->shape.vertexData.vertexCount) ? (k + 1) : 0);   // Get next vertex or first to close the shape
                Vector2 vertexD = glMPhysac->GetPhysicsShapeVertex(player.getPhysicsBody(), kk);

                DrawLineV(vertexC, vertexD, RED);     // Draw a line between two vertex positions
            }*/

            //EndMode2D();

            DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
            DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);
            DrawText(FormatText("%d bodies", bodiesCount), 10, 50, 10, WHITE);
            DrawText(FormatText("%d manifolds", glMPhysac->GetManifoldsCount()), 10, 70, 10, WHITE);
            //DrawText(FormatText("%d", player.body->freezeOrient), 10, 70, 10, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    glMPhysac->ClosePhysics();       // Unitialize physics

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}