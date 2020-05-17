/*******************************************************************************************
*   Entity.cpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include "Entity.hpp"

bool Entity::getIsPassable(){
    return this->isPassable;
}

void Entity::setIsPassable(bool newIsPassable){
    this->isPassable = newIsPassable;
}

void Entity::setTexture(std::string newTextureName){
    this->textureName = newTextureName;
    // Update Texture
}

bool Entity::getIsSpawned(){
    return isSpawned;
}

void Entity::spawn(){
    isSpawned = true;
}

void Entity::despawn(){
    isSpawned = false;
}

Entity::Entity(Vector2 pos, int width, int height, int density, std::string textureName){
    // Create the PhysicsBody associated with the Entity
    this->body = glMPhysac->CreatePhysicsBodyRectangle(pos, width, height, density);
    std::cout<<"yo"<<std::endl;
    // Set the Texture Name
    this->textureName = textureName;
    // Create the Texture
}

Entity::~Entity(){
    // Destroys this PhysicsBody
    glMPhysac->DestroyMPhysacBody(this->body);
}