#include "Input/Action.hpp"

#include <SDL.h>

bool Action::test(const Uint8* kbState) const {
	if (!(type & Type::Pressed))
		return false;
	return (bool)kbState[key];
}

bool Action::operator==(const SDL_Event& ev) const {
	return ((ev.type == SDL_KEYUP && type == Action::Type::Released) || (ev.type == SDL_KEYDOWN && type == Action::Type::Pressed)) &&
			ev.key.keysym.scancode == key;
}

bool Action::operator==(const Action& a) const {
	return type == a.type && key == a.key;
}