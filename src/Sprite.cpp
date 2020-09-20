#include "Sprite.hpp"

#include <iostream>

#include <SDL.h>

#include "Painter.hpp"
#include "ResourcesLoader.hpp"
#include "ResourcesID.hpp"

Sprite::Sprite(ResourcesLoader &rl, Image::ID id, int x, int y, int w, int h) :
	resources(rl),
	imgID(id)
{
	sourceRect.x = x; sourceRect.y = y;
	sourceRect.w = w; sourceRect.h = h;
	
	resources.load(imgID, "sprite.png");
}

Sprite::~Sprite() {
	resources.unload(imgID);
}

void Sprite::draw(const Painter& painter, int x, int y) const {
    SDL_Rect destRect;
    destRect.w = sourceRect.w; destRect.h = sourceRect.h;
    destRect.x = x; destRect.y = y;
    SDL_RenderCopy(painter.renderer, resources.get(imgID), &sourceRect, &destRect);
}