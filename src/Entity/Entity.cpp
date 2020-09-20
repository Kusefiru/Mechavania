/*******************************************************************************************
*   Entity.cpp
*
*   This file designs the Entity class based for various entities (player, ennemies)
*
********************************************************************************************/

#include <iostream>

#include "Entity/Entity.hpp"

void Entity::spawn(){
    isSpawned = true;
}

void Entity::despawn(){
    isSpawned = false;
}

Entity::Entity(){
    // Create the PhysicsBody associated with the Entity
    //body = glMPhysac->CreatePhysicsBodyRectangle(pos, width, height, density);
    std::cout << "creating entity" << std::endl;
}

Entity::~Entity(){
    // Destroys this PhysicsBody
    //glMPhysac->DestroyMPhysacBody(body);
}