#ifndef MECHA_ACTION_HPP
#define MECHA_ACTION_HPP

#include <SDL.h>

/**
 * Input management inspired by https://github.com/Krozark/SFML-book/blob/master/05_GUI/include/SFML-Book/Action.hpp
 */

class Action {
	public:
		enum Type {
			RealTime = 1,
			Pressed = 1 << 1,
			Released = 1 << 2,
		};

	public:
		Action(const SDL_Scancode &k, Type t) : key(k), type(t) {}

		// Pass the state of the keyboard
		bool test(const Uint8* kbState) const;

		bool operator==(const SDL_Event& ev) const;
		bool operator==(const Action& a) const;
	
	private:
		template<typename> friend class InputTarget;

	private:
		Type type;
		SDL_Scancode key;
};

#endif