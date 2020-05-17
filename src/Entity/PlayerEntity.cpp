/*******************************************************************************************
*   PlayerEntity.cpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#include "PlayerEntity.hpp"

PlayerEntity::PlayerEntity(Vector2 pos, int width, int height, int density, std::string textureName) : Entity(pos, width, height, density, textureName){
    std::cout<<"yo2"<<std::endl;
};