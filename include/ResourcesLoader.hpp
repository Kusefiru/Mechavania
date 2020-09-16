#ifndef MECHA_RESOURCESLOADER_HPP
#define MECHA_RESOURCESLOADER_HPP

#include <map>
#include <string>

#include <SDL.h>
#include <SDL2/SDL_image.h>

#include "ResourcesID.hpp"

class ResourcesLoader {
	public:
		ResourcesLoader(SDL_Renderer* renderer) : renderer(renderer) {}
		virtual ~ResourcesLoader(); // unload all resources

		void load(Image::ID id, const std::string &filename);
		void unload(Image::ID id);

		SDL_Texture* get(Image::ID id);
		const SDL_Texture* get(Image::ID id) const;

	private:
		std::map<Image::ID, SDL_Texture*> resourceMap;
		SDL_Renderer *renderer;
};

#endif