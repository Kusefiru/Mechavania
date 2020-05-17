/*******************************************************************************************
*   MCamera.cpp
*
*   This file designs the MCamera class for the game camera
*
********************************************************************************************/

#include "MCamera.hpp"

void MCamera::update() {
    if(this->followedPhysicsBody != nullptr) {
        this->camera.target = (Vector2){(this->followedPhysicsBody)->position.x, (this->followedPhysicsBody)->position.y}; // TODO : the camera is not centered on the entity for now
    } else {
        this->camera.target = (Vector2){(this->cameraPhysicsBody)->position.x, (this->cameraPhysicsBody)->position.y};
    }
}

void MCamera::setFollowedPhysicsBody(MPhysacBody* body){
    this->followedPhysicsBody = body;
    this->update();
}

Camera2D& MCamera::getCamera(){
    return this->camera;
}

MCamera::MCamera(){
    this->camera = {0};
    this->camera.offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_WIDTH/2 };
    this->camera.target = (Vector2){ SCREEN_WIDTH/2, SCREEN_WIDTH/2 };
    this->camera.rotation = 0.0f;
    this->camera.zoom = 1.0f;

    this->cameraPhysicsBody = glMPhysac->CreatePhysicsBodyRectangle(camera.target, 1, 1, 1);
    this->followedPhysicsBody = nullptr;
}