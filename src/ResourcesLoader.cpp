#include "ResourcesLoader.hpp"

#include <iostream>
#include <stdexcept>
#include <cassert>

void ResourcesLoader::load(Image::ID id, const std::string &filename) {
	SDL_Texture* loaded(IMG_LoadTexture(renderer, filename.c_str()));
	if (!loaded) // NULL ?
		throw std::runtime_error("[Error] ResourcesLoader::load - failed to load " + filename);

	// Insert and check success
	auto inserted = resourceMap.insert(std::make_pair(id, loaded));
	assert(inserted.second);
}

void ResourcesLoader::unload(Image::ID id) {
	// Check if resource with the key "id" exist and delete it
	auto found = resourceMap.find(id);
	SDL_DestroyTexture(found->second);
	
	size_t nbErased = resourceMap.erase(id);
	if (!nbErased)
		std::cerr << "[Warning] ResourcesLoader::unload - nothing to unload for key " << id << std::endl;
}

void ResourcesLoader::unloadAll() {
	for(auto &kv : resourceMap)
		SDL_DestroyTexture(kv.second);

	resourceMap.clear();
}

SDL_Texture* ResourcesLoader::get(Image::ID id) {
	auto found = resourceMap.find(id);

	if(found == resourceMap.end())
		throw std::runtime_error("[Error] ResourcesLoader::get - failed to get the resource");
	
	return found->second;
}

const SDL_Texture* ResourcesLoader::get(Image::ID id) const {
	auto found = resourceMap.find(id);

	if(found == resourceMap.end())
		throw std::runtime_error("[Error] ResourcesLoader::get - failed to get the resource");

	return found->second;
}