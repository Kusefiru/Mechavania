#ifndef MECHA_WINDOW_HPP
#define MECHA_WINDOW_HPP

struct SDL_Window;

class Window {
	public:
		Window();
		~Window();

		SDL_Window *w;
};

#endif