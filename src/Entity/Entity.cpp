/*******************************************************************************************
*   Entity.cpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include <iostream>

#include "Entity/Entity.hpp"

bool Entity::getIsPassable(){
    return this->isPassable;
}

void Entity::setIsPassable(bool newIsPassable){
    this->isPassable = newIsPassable;
}

void Entity::setTexture(const std::string &newTextureName){
    //this->textureName = newTextureName;
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

Entity::Entity(const Vector2f &pos, int width, int height, int density, const std::string &name){
    // Create the PhysicsBody associated with the Entity
    //body = glMPhysac->CreatePhysicsBodyRectangle(pos, width, height, density);
    std::cout << "yo" << std::endl;
    // Set the Texture Name
    textureName = name;
    // Create the Texture
}

Entity::~Entity(){
    // Destroys this PhysicsBody
    //glMPhysac->DestroyMPhysacBody(body);
}