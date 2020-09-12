/*******************************************************************************************
*   PlayerEntity.cpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#include <iostream>

#include "Entity/PlayerEntity.hpp"

PlayerEntity::PlayerEntity(const Vector2f &pos, int width, int height, int density, const std::string &textureName) :
    Entity(pos, width, height, density, textureName) 
    {
    std::cout<<"yo2"<<std::endl;
};