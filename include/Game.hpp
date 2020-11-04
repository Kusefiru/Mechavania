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

class Game {
	public:
		Game();
		~Game();

		void run();

	private:
		void update(const std::chrono::duration<float> &dt);
		void processInput();
		void setUpdateFPS(unsigned int fps) { updateFrameDuration = std::chrono::duration<float>(1.f / (float)fps); }
		void setRenderFPS(unsigned int fps) { renderFrameDuration = std::chrono::duration<float>(1.f / (float)fps); }
		void draw();

		void updateStats(const std::chrono::duration<float> &dtU, const std::chrono::duration<float> &dtR);

	private:
		// Must be declared in this order (to have a correct destruction order)
		Window win;
		Renderer renderer;
		Painter painter;
		ResourcesLoader resources;

		PlayerEntity testPlayer;

		bool running;
		std::chrono::duration<float> updateFrameDuration, renderFrameDuration;
};

#endif