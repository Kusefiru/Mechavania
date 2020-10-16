#include "Game.hpp"

#include <iostream>

#include <SDL.h>
#include <SDL2/SDL_image.h>

#include "MPhysac/MPhysacWorld.hpp"
#include "Entity/PlayerEntity.hpp"

Game::Game() :
	win(),
	renderer(win),
	painter(renderer.r),
	resources(renderer.r),
	statsdt(0),
	testPlayer(resources)
{}

Game::~Game() {}

// Core of the game loop
void Game::run() {
	running = true;
	fps = 60;
	std::chrono::duration<float> frameDuration(1.f / fps);
	std::chrono::duration<float> sinceLastUpdate(0);
	std::chrono::time_point<std::chrono::steady_clock> lastUpdate(std::chrono::steady_clock::now());
	std::chrono::duration<float> dt(0);

	glMPhysac->InitPhysics();

	while (running) {
		lastUpdate = std::chrono::steady_clock::now();
		sinceLastUpdate += dt;

		while (sinceLastUpdate > frameDuration) {
			sinceLastUpdate -= frameDuration;
		
			processInput();
			update(frameDuration);
			glMPhysac->RunPhysicsStep(frameDuration);
		}

		updateStats(dt);
		draw();
		dt = std::chrono::steady_clock::now() - lastUpdate;
		// if (elapsedTime < timePerFrame)
		// 	SDL_Delay(timePerFrame - elapsedTime); // Limit FPS (call sleep)
		// ...
	}
}

void Game::updateStats(const std::chrono::duration<float> &dt) {
	statsdt += dt;
	fpsStats++;

	if (statsdt > std::chrono::duration<float>(1.f)) {
		std::cout << "FPS: " << fpsStats << std::endl;
		
		statsdt -= std::chrono::duration<float>(1.f);
		fpsStats = 0;
	}
}

void Game::update(const std::chrono::duration<float> &dt) {
	testPlayer.update(dt);
}

void Game::processInput() {
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		switch(ev.type) {
		case SDL_WINDOWEVENT:
			if (ev.window.windowID == SDL_GetWindowID(win.w)) {
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
			if (ev.key.keysym.scancode == SDL_SCANCODE_UP)
				testPlayer.move(-Vector2f(0,5));
			if (ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
				testPlayer.move(Vector2f(0,5));
			if (ev.key.keysym.scancode == SDL_SCANCODE_RIGHT)
				testPlayer.move(Vector2f(5,0));
			if (ev.key.keysym.scancode == SDL_SCANCODE_LEFT)
				testPlayer.move(-Vector2f(5,0));
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
	SDL_RenderClear(renderer.r);

	// Draw something
	painter.draw(testPlayer);

	SDL_RenderPresent(renderer.r);
}
