#ifndef MECHA_DRAWABLE_HPP
#define MECHA_DRAWABLE_HPP

#include <SDL.h>

class Painter;

class Drawable {
	public:
		virtual ~Drawable() {}

		virtual void draw(const Painter &p) const = 0;
};

#endif