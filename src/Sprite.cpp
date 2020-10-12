#include "Sprite.hpp"

#include <iostream>

#include <SDL.h>

#include "Painter.hpp"
#include "ResourcesLoader.hpp"
#include "ResourcesID.hpp"

std::map<Image::ID, SDL_Rect> Sprite::srcRect;

Sprite::Sprite(ResourcesLoader &rl, Image::ID id, int srcX, int srcY, int srcW, int srcH) :
	resources(rl),
	imgID(id)
{
	if (srcRect.find(id) == srcRect.end()) {
		srcRect.insert(std::make_pair(id, SDL_Rect()));

		srcRect[id].x = srcX; srcRect[id].y = srcY;
		srcRect[id].w = srcW; srcRect[id].h = srcH;
	}

	dstRect.x = 0; dstRect.y = 0;
	dstRect.w = srcW; dstRect.h = srcH;
	
	resources.load(imgID);
}

Sprite::~Sprite() {}

void Sprite::draw(const Painter& painter) const {
	SDL_RenderCopy(painter.renderer, resources.get(imgID), &(srcRect[imgID]), &dstRect);
}