#include "Sprite.hpp"

#include <SDL.h>

#include "Painter.hpp"
#include "ResourcesLoader.hpp"
#include "ResourcesID.hpp"

Sprite::Sprite(ResourcesLoader *rl, Image::ID id,  int x, int y, int w, int h) {
	resources = rl;
	imgID = id;
	sourceRect.x = x; sourceRect.y = y;
	sourceRect.w = w; sourceRect.h = h;
	
	resources->load(imgID, "sprite.png");
}

Sprite::~Sprite() {
	resources->unload(imgID);
}

void Sprite::draw(const Painter& painter) const {
	SDL_RenderCopy(painter.renderer, resources->get(imgID), NULL, NULL);
}