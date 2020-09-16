#include "Window.hpp"

#include <SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Window::Window() :
	w(SDL_CreateWindow("Mechavania",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0))
{}

Window::~Window() {
	   	SDL_DestroyWindow(w);
}