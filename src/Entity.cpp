/*******************************************************************************************
*   Entity.cpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include "Entity.hpp"

PhysicsBody Entity::GetPhysicsBody(){
    return this->Body;
}

bool Entity::GetIsPassable(){
    return this->IsPassable;
}

Entity::SetIsPassable(bool NewIsPassable){
    this->IsPassable = NewIsPassable;
}

Entity::SetTexture(std::string NewTextureName){
    this->TextureName = NewTextureName;
    // Update Texture
}

Entity::Entity(Vector2 Pos, int Width, int Height, int Density, std::string TextureName){
    // Create the PhysicsBody associated with the Entity
    this->Body = CreatePhysicsBodyRectangle(Pos, Width, Height, Density);
    // Set the Texture Name
    this->TextureName = TextureName;
    // Create the Texture
}

Entity::~Entity(){
    // Destroys this PhysicsBody
    DestroyPhysicsBody(this->Body);
}