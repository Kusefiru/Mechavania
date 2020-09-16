#ifndef MECHA_RENDERER_HPP
#define MECHA_RENDERER_HPP

class Window;
struct SDL_Renderer;

class Renderer {
	public:
		Renderer(const Window &win);
		~Renderer();

		SDL_Renderer *r;
};

#endif