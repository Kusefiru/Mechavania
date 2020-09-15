#ifndef MECHA_PAINTER_HPP
#define MECHA_PAINTER_HPP

#include <SDL.h>

#include "Drawable.hpp"

class Drawable;
struct SDL_Renderer;

class Painter {
	public:
		Painter(SDL_Renderer *r) : renderer(r) {}

		void draw(const Drawable& d) const { d.draw(*this); }
		
		SDL_Renderer *renderer;
	private:
};

#endif