#include "Game.hpp"

#include <iostream>

#include <SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity/PlayerEntity.hpp"

Game::Game() :
	win(),
	renderer(win),
	painter(renderer.r),
	resources(renderer.r),
	testPlayer(resources),
	updateFrameDuration(1.f / 60.f),
	renderFrameDuration(1.f / 144.f)
{}

Game::~Game() {}

// Core of the game loop
void Game::run() {
	running = true;
	std::chrono::duration<float> sinceLastUpdate(0.f), sinceLastRender(0.f);

	std::chrono::time_point<std::chrono::steady_clock> lastUpdate(std::chrono::steady_clock::now()), lastRender(std::chrono::steady_clock::now());
	std::chrono::duration<float> dt(0);

	while (running) {
		std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
		dt = now - lastUpdate;
		lastUpdate = now;

		sinceLastUpdate += dt;
		sinceLastRender += dt;

		while (sinceLastUpdate > updateFrameDuration) {
			sinceLastUpdate -= updateFrameDuration;
			
			processInput();
			testPlayer.processRealTimeEvents();

			update(updateFrameDuration);
			updateStats(updateFrameDuration, std::chrono::seconds::zero());
		}

		if (sinceLastRender > renderFrameDuration) {
			updateStats(std::chrono::seconds::zero(), sinceLastRender);
			draw();
			sinceLastRender = std::chrono::seconds::zero();
		}
	}
}

void Game::updateStats(const std::chrono::duration<float> &dtU, const std::chrono::duration<float> &dtR) {
	static std::chrono::duration<float> dtRender(0), dtUpdate(0); 
	static int renderFps = 0, updateFps = 0;
	
	dtRender += dtR; dtUpdate += dtU;

	if (dtR.count() != 0)
		renderFps++;

	if (dtU.count() != 0)
		updateFps++;

	if (dtRender > std::chrono::duration<float>(1.f)) {
		std::cout << "FPS (Render): " << renderFps << std::endl;
		
		dtRender -= std::chrono::duration<float>(1.f);
		renderFps = 0;
	}
	
	if (dtUpdate > std::chrono::duration<float>(1.f)) {
		std::cout << "FPS (Update): " << updateFps << std::endl;
		
		dtUpdate -= std::chrono::duration<float>(1.f);
		updateFps = 0;
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
			break;
		case SDL_QUIT:
			running = false;
			break;
		default:
			break;
		}
	}
	testPlayer.processEvent(ev);
}

void Game::draw() {
	SDL_RenderClear(renderer.r);

	// Draw something
	painter.draw(testPlayer);

	SDL_RenderPresent(renderer.r);
}
