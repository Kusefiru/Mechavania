#ifndef MECHA_GAME_HPP
#define MECHA_GAME_HPP

#include <stdint.h> // Or uint32_t is not defined => C3646

#include "Window.hpp"
#include "Renderer.hpp"
#include "Painter.hpp"
#include "ResourcesLoader.hpp"
#include "Sprite.hpp"

class MPhysacWorld;

class Game {
	public:
		Game();
		~Game();

		void run();

	private:
		void update();
		void processInput();
		void draw();

	private:
		MPhysacWorld *glMPhysac;

		// Must be declared in this order (to have a correct destruction order)
		Window win;
		Renderer renderer;
		Painter painter;
		ResourcesLoader resources;
		Sprite test;

		bool running;
		uint32_t fps;

};

#endif