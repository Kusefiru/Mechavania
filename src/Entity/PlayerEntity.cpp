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

PlayerEntity::PlayerEntity(ResourcesLoader &rl)
{
    position += Vector2f(10,10);

    std::vector<SDL_Rect> rects(8);
	for (int i = 0; i < 8; ++i) {
		rects[i].x = i * 64; rects[i].y = 0;
		rects[i].w = 64; rects[i].h = 64;
	}

	playerSprite = new AnimatedSprite(rl, Image::Player, rects, std::chrono::duration<float>(0.1));
    playerSprite->setPosition((Vector2i)position);

	initInput();
}

PlayerEntity::~PlayerEntity() {
    delete playerSprite;
}

void PlayerEntity::initInput() {
	bind(PlayerAction::MoveUp, Action(SDL_SCANCODE_UP, (Action::Type)(Action::Type::Pressed | Action::Type::RealTime)), [this]() {
		move(Vector2f(0.f, -5.f));
	});
	bind(PlayerAction::MoveDown, Action(SDL_SCANCODE_DOWN, (Action::Type)(Action::Type::Pressed | Action::Type::RealTime)), [this]() {
		move(Vector2f(0.f, 5.f));
	});
	bind(PlayerAction::MoveLeft, Action(SDL_SCANCODE_LEFT, (Action::Type)(Action::Type::Pressed | Action::Type::RealTime)), [this]() {
		move(Vector2f(-5.f, 0.f));
	});
	bind(PlayerAction::MoveRight, Action(SDL_SCANCODE_RIGHT, (Action::Type)(Action::Type::Pressed | Action::Type::RealTime)), [this]() {
		move(Vector2f(5.f, 0.f));
	});
}