#ifndef MECHA_SPRITE_HPP
#define MECHA_SPRITE_HPP

#include <map>

#include "Utils/Vector.hpp"
#include "Drawable.hpp"
#include "ResourcesID.hpp"

class Painter;
struct SDL_Rect;
struct SDL_Texture;
class ResourcesLoader;

class Sprite : public Drawable {
	private:
		static std::map<Image::ID, SDL_Rect> srcRect;

	public:
		Sprite(ResourcesLoader &rl, Image::ID id, int srcX, int srcY, int srcW, int srcH);
		Sprite(ResourcesLoader &rl, Image::ID id, const Vector2i& pos, const Vector2i& size) : Sprite(rl, id, pos.x, pos.y, size.x, size.y) {}
		Sprite(ResourcesLoader &rl, Image::ID id, const SDL_Rect& sRect) : Sprite(rl, id, sRect.x, sRect.y, sRect.w, sRect.h) {}
		virtual ~Sprite();

		inline void setPosition(int x, int y) { dstRect.x = x; dstRect.y = y; }
		inline void setPosition(const Vector2i &pos) { dstRect.x = pos.x; dstRect.y = pos.y; }

		void draw(const Painter& painter) const;

	private:
		SDL_Rect dstRect;
		ResourcesLoader &resources;
		Image::ID imgID;
	
	private:
};

#endif