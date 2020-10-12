#include "AnimatedSprite.hpp"

#include <iostream>

#include <SDL.h>

#include "Painter.hpp"
#include "ResourcesLoader.hpp"
#include "ResourcesID.hpp"

std::map<Image::ID, std::vector<SDL_Rect>> AnimatedSprite::srcRects;

AnimatedSprite::AnimatedSprite(ResourcesLoader &rl, Image::ID id, const std::vector<SDL_Rect>& sRects, std::chrono::duration<float> stepD) :
	resources(rl),
	imgID(id),
	currentStep(0),
	stepDuration(stepD),
	dtStep(0.f)
{
	if (srcRects.find(id) == srcRects.end()) {
		srcRects.insert(std::make_pair(id, sRects));
	}

	dstRect.x = 0; dstRect.y = 0;
	dstRect.w = sRects[0].w; dstRect.h = sRects[0].h;
	
	resources.load(imgID);
}

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::update(const std::chrono::duration<float>& dt) {
	dtStep += dt;
	if (dtStep >= stepDuration) {
		dtStep -= stepDuration;
		currentStep++;
		currentStep %= srcRects[imgID].size();
	}
}

void AnimatedSprite::draw(const Painter& painter) const {
	SDL_RenderCopy(painter.renderer, resources.get(imgID), &(srcRects[imgID][currentStep]), &dstRect);
}