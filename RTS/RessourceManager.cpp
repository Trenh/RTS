#include "RessourceManager.h"



RessourceManager::RessourceManager()
{
}


RessourceManager::~RessourceManager()
{
}

//Load a texture
SDL_Texture* RessourceManager::loadTexture(SDL_Renderer* renderer, std::string name) {
	//The final texture
	SDL_Texture *newTexture = NULL;

	//Load image at specified path
	newTexture = IMG_LoadTexture(renderer, name.c_str());

	return newTexture;
}

void RessourceManager::setAllTexture(SDL_Renderer* renderer, std::vector<std::string> names) {
	//foreach name
	// load the texture
	//add the texture and name to the map 

	for (auto &name : names) {
		SDL_Texture *newTexture = loadTexture(renderer, name);
		images.insert({ name,newTexture });


	}
}

//return the texture in correspondance with the name
SDL_Texture* RessourceManager::getTextureByString(std::string name) {
	return images.find(name)->second;
}
