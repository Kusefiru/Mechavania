/*******************************************************************************************
*   Mcamera.cpp
*
*   This file designs the Mcamera class for the game camera
*
********************************************************************************************/

#include "Mcamera.hpp"

void Mcamera::update() {
    if(this->followedPhysicsBody != nullptr) {
        this->camera.target = (Vector2){(*this->followedPhysicsBody)->position.x, (*this->followedPhysicsBody)->position.y}; // TODO : the camera is not centered on the entity for now
    } else {
        this->camera.target = (Vector2){(this->cameraPhysicsBody)->position.x, (this->cameraPhysicsBody)->position.y};
    }
}

void Mcamera::setFollowedPhysicsBody(PhysicsBody* body){
    this->followedPhysicsBody = body;
    this->update();
}

Camera2D& Mcamera::getCamera(){
    return this->camera;
}

Mcamera::Mcamera(){
    this->camera = {0};
    this->camera.offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_WIDTH/2 };
    this->camera.target = (Vector2){ SCREEN_WIDTH/2, SCREEN_WIDTH/2 };
    this->camera.rotation = 0.0f;
    this->camera.zoom = 1.0f;

    this->cameraPhysicsBody = glMphysac->CreatePhysicsBodyRectangle(camera.target, 1, 1, 1);
    this->followedPhysicsBody = nullptr;
}