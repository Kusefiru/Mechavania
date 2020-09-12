#include "Game.hpp"

#include <iostream>

#include <SDL.h>

#include "MPhysac/MPhysacWorld.hpp"

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);

	win = SDL_CreateWindow("Mechavania",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, 0);
}

Game::~Game() {
	SDL_DestroyRenderer(renderer);
   	SDL_DestroyWindow(win);
	SDL_Quit();
}

// Core of the game loop
void Game::run() {
	running = true;
	fps = 60;

	while (running) {
		const uint32_t startTime = SDL_GetTicks();

		processInput();
		update();
		
		draw();
		uint32_t elapsedTime = SDL_GetTicks() - startTime;
		uint32_t timePerFrame = 1000 / fps; // (in ms)
		if (elapsedTime < timePerFrame)
			SDL_Delay(timePerFrame - elapsedTime); // Limit FPS (call sleep)
		// ...
	}
}

/* Another way to update the main loop but no sleep (using 100% of cpu)
 * Here, Physic is update as much as possible and only rendering is limited to 60 FPS
 * dt: elapsed time between iterations :

void Game::run() {
	running = true;

	// Time (in ms) since last update
	int sinceLastUpdate = 0;
	int timePerFrame = 1000 / (int)fps; // (in ms)

	while (running) {
		const int dt = SDL_GetTicks();
		while (sinceLastUpdate > timePerFrame) {
			sinceLastUpdate -= timePerFrame;
			processInput();
			update(dt);
		}
		
		draw();
	}
} */

void Game::update() {}

void Game::processInput() {
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		switch(ev.type) {
		case SDL_WINDOWEVENT:
			if (ev.window.windowID == SDL_GetWindowID(win)) {
				switch (ev.window.event) {
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					std::cout << "Gained Focus" << std::endl;
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					std::cout << "Lost Focus" << std::endl;
					break;
				default:
					break;
				}
			}
			break;
		case SDL_KEYDOWN:
			if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				running = false;
			break;
		case SDL_QUIT:
			running = false;
			break;
		default:
			break;
		}
	}
}

void Game::draw() {
	SDL_RenderClear(renderer);

	// Draw something

	SDL_RenderPresent(renderer);
}