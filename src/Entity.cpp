/*******************************************************************************************
*   Entity.cpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include "Entity.hpp"

PhysicsBody Entity::getPhysicsBody(){
    return this->body;
}

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

Entity::Entity(Vector2 pos, int width, int height, int density, std::string textureName){
    // Create the PhysicsBody associated with the Entity
    this->body = CreatePhysicsBodyRectangle(pos, width, height, density);
    // Set the Texture Name
    this->textureName = textureName;
    // Create the Texture
}

Entity::~Entity(){
    // Destroys this PhysicsBody
    DestroyPhysicsBody(this->body);
}