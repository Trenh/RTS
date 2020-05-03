#pragma once
#include <SDL.h> 
#include <SDL_image.h> 
#include <stdio.h>
#include <map>
#include <vector>
#include <string>


class RessourceManager
{
public:
	RessourceManager();
	~RessourceManager();
	SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string name);

	void setAllTexture(SDL_Renderer* renderer, std::vector<std::string>);
	SDL_Texture* getTextureByString(std::string name);

private:
	std::map<std::string, SDL_Texture*> images;


};

