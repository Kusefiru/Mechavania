#ifndef MECHA_ANIMATEDSPRITE_HPP
#define MECHA_ANIMATEDSPRITE_HPP

#include <map>
#include <vector>
#include <chrono>

#include "Drawable.hpp"
#include "ResourcesID.hpp"
#include "Utils/Vector.hpp"

#include <iostream>

class Painter;
struct SDL_Rect;
struct SDL_Texture;
class ResourcesLoader;

class AnimatedSprite : public Drawable {
	private:
		static std::map<Image::ID, std::vector<SDL_Rect>> srcRects;

	public:
		// speed is ms / step
		AnimatedSprite(ResourcesLoader &rl, Image::ID id, const std::vector<SDL_Rect>& sRects, std::chrono::duration<float> stepD);
		virtual ~AnimatedSprite();

		inline void setPosition(int x, int y) { dstRect.x = x; dstRect.y = y; }
		inline void setPosition(const Vector2i &pos) { dstRect.x = pos.x; dstRect.y = pos.y; }

		inline void setCurrentStep(size_t step) { currentStep = step; }

		void update(const std::chrono::duration<float>& dt);

		void draw(const Painter& painter) const;

	private:	
		SDL_Rect dstRect;
		ResourcesLoader &resources;
		Image::ID imgID;

		size_t currentStep;
		const std::chrono::duration<float> stepDuration;
		std::chrono::duration<float> dtStep;
};

#endif