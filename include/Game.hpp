#ifndef MECHA_GAME_HPP
#define MECHA_GAME_HPP

#include <chrono>
#include <stdint.h> // Or uint32_t is not defined => C3646

#include "Window.hpp"
#include "Renderer.hpp"
#include "Painter.hpp"
#include "ResourcesLoader.hpp"
#include "Sprite.hpp"
#include "Entity/PlayerEntity.hpp"

class MPhysacWorld;

class Game {
	public:
		Game();
		~Game();

		void run();

	private:
		void update(const std::chrono::duration<float> &dt);
		void processInput();
		void draw();

		void updateStats(const std::chrono::duration<float> &dt);

	private:
		MPhysacWorld *glMPhysac;

		// Must be declared in this order (to have a correct destruction order)
		Window win;
		Renderer renderer;
		Painter painter;
		ResourcesLoader resources;

		int fpsStats = 0;
		std::chrono::duration<float> statsdt;
		
		PlayerEntity testPlayer;

		bool running;
		uint32_t fps;

};

#endif