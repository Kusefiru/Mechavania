/*******************************************************************************************
*   MovableEntity.cpp
*
*   This file designs the PlayerEntity class based for the player
*
********************************************************************************************/

#include <iostream>

#include "Entity/MovableEntity.hpp"

MovableEntity::MovableEntity() : Entity() {
    std::cout<<"Creating movable entity"<<std::endl;
};