#ifndef MECHA_RESOURCESLOADER_HPP
#define MECHA_RESOURCESLOADER_HPP

#include <map>
#include <vector>
#include <string>

#include <SDL.h>
#include <SDL2/SDL_image.h>

#include "ResourcesID.hpp"

class ResourcesLoader {
	public:
		ResourcesLoader(SDL_Renderer* renderer);
		virtual ~ResourcesLoader(); // unload all resources

		void load(Image::ID id);
		// void unload(Image::ID id); No unload for the moment

		SDL_Texture* get(Image::ID id);
		const SDL_Texture* get(Image::ID id) const;

	private:
		SDL_Renderer *renderer;

		// Loaded textures list
		std::map<Image::ID, SDL_Texture*> texturesMap;

		std::map<Image::ID, const std::string> imgFileMap;
		std::map<Image::ID, const std::vector<Image::ID>> correspondingImgMap;
};

#endif