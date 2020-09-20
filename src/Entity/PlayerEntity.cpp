/*******************************************************************************************
*   PlayerEntity.cpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#include <iostream>

#include "Entity/PlayerEntity.hpp"
#include "MPhysac/MPhysacBody.hpp"
#include "MPhysac/MPhysacWorld.hpp"

PlayerEntity::PlayerEntity(ResourcesLoader &rl) : MovableEntity(), playerSprite(rl, Image::Player, 0, 0, 68, 68) {
    position += Vector2f(10,10);

    MPhysacWorld &physacWorld = MPhysacWorld::getInstance();
    playerBody = physacWorld.CreatePhysicsBodyRectangle(position, 68.0, 68.0, 1.0);
    playerBody->solidType = MPHYSAC_NONPASSABLE;

    std::cout<<"Creating player entity"<<std::endl;
};