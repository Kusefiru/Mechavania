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

PlayerEntity::PlayerEntity(ResourcesLoader &rl) :
    MovableEntity()
{
    position += Vector2f(10,10);

    std::vector<SDL_Rect> rects(8);
	for (int i = 0; i < 8; ++i) {
		rects[i].x = i * 64; rects[i].y = 0;
		rects[i].w = 64; rects[i].h = 64;
	}

	playerSprite = new AnimatedSprite(rl, Image::Player, rects, std::chrono::duration<float>(0.1));
    playerSprite->setPosition((Vector2i)position);

    MPhysacWorld &physacWorld = MPhysacWorld::getInstance();
    playerBody = physacWorld.CreatePhysicsBodyRectangle(position, 64.0, 64.0, 1.0);
    playerBody->solidType = MPHYSAC_NONPASSABLE;

    std::cout<<"Creating player entity"<<std::endl;
}

PlayerEntity::~PlayerEntity() {
    delete playerSprite;
}