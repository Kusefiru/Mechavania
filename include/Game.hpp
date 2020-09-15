#ifndef MECHA_GAME_HPP
#define MECHA_GAME_HPP

#include <stdint.h> // Or uint32_t is not defined => C3646

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class MPhysacWorld;
class ResourcesLoader;
class Sprite;
class Painter;

struct SDL_Window;
struct SDL_Renderer;

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

		SDL_Window *win;
		SDL_Renderer *renderer;

		bool running;
		uint32_t fps;

		Painter *painter;
		ResourcesLoader *resources;
		Sprite *test;

};

#endif