#ifndef MECHA_SPRITE_HPP
#define MECHA_SPRITE_HPP

#include "Drawable.hpp"
#include "ResourcesID.hpp"

class Painter;
struct SDL_Rect;
struct SDL_Texture;
class ResourcesLoader;

class Sprite : public Drawable {
	public:
		Sprite(ResourcesLoader &rl, Image::ID id, int x, int y, int w, int h);
		virtual ~Sprite();

		void draw(const Painter& painter) const;

	protected:
		SDL_Rect sourceRect;
	
	private:
		ResourcesLoader &resources;
		Image::ID imgID;
};

#endif