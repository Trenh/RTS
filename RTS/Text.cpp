#include "Text.h"



Text::Text(int pX, int pY, int pWidth, int pHeight)
	:x(pX), y(pY), width(pWidth), height(pHeight), font(nullptr), color({ 0xFF,0xFF,0xFF }), surface(nullptr), texture(nullptr)
{
}
Text::Text()
{
}


Text::~Text()
{
}

void Text::load(SDL_Renderer* renderer, const char newText[2])
{
	font = TTF_OpenFont("OpenSans-Regular.ttf", 28);
	changeText(renderer, newText);
}

void Text::changeText(SDL_Renderer* renderer, const char* newText)
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
	surface = TTF_RenderText_Solid(font, newText, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Text::draw(SDL_Renderer* renderer)
{
	SDL_Rect rect = { x,y,width,height };
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}