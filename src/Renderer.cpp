#include "Renderer.hpp"

#include <SDL.h>

#include "Window.hpp"

Renderer::Renderer(const Window &win) :
	r(SDL_CreateRenderer(win.w, -1, 0))
{}

Renderer::~Renderer() {
	SDL_DestroyRenderer(r);
}