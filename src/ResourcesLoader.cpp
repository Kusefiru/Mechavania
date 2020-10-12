#include "ResourcesLoader.hpp"

#include <iostream>
#include <stdexcept>
#include <cassert>

ResourcesLoader::ResourcesLoader(SDL_Renderer* renderer) :
	renderer(renderer) {
	imgFileMap = { {Image::Player, "anim_sprite.png"} };
	correspondingImgMap = { {Image::Player, {} } };
}

ResourcesLoader::~ResourcesLoader() {
	for(auto &kv : texturesMap)
		SDL_DestroyTexture(kv.second);
}

void ResourcesLoader::load(Image::ID id) {

	if (texturesMap.find(id) == texturesMap.end()) { // Texture already loaded?

		const std::string &filename = imgFileMap[id];
		
		SDL_Texture* loaded(IMG_LoadTexture(renderer, filename.c_str()));
			if (!loaded) // NULL ?
				throw std::runtime_error("[Error] ResourcesLoader::load - failed to load " + filename);

		// Insert the texture for all corresponding images
		texturesMap.insert(std::make_pair(id, loaded));
		for (auto img : correspondingImgMap[id])
			texturesMap.insert(std::make_pair(img, loaded));
	}
}

// void ResourcesLoader::unload(Image::ID id) {
// 	// Check if resource with the key "id" exist and delete it
// 	auto found = texturesMap.find(id);
// 	SDL_DestroyTexture(found->second);
	
// 	size_t nbErased = texturesMap.erase(id);
// 	if (!nbErased)
// 		std::cerr << "[Warning] ResourcesLoader::unload - nothing to unload for key " << id << std::endl;
// }

SDL_Texture* ResourcesLoader::get(Image::ID id) {
	auto found = texturesMap.find(id);

	if(found == texturesMap.end())
		throw std::runtime_error("[Error] ResourcesLoader::get - failed to get the resource");
	
	return found->second;
}

const SDL_Texture* ResourcesLoader::get(Image::ID id) const {
	auto found = texturesMap.find(id);

	if(found == texturesMap.end())
		throw std::runtime_error("[Error] ResourcesLoader::get - failed to get the resource");

	return found->second;
}