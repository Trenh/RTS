#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
class Text
{
public:
	Text(int pX, int pY, int pWidth, int pHeight);
	Text();
	~Text();

	void load(SDL_Renderer* renderer, const char* newText);
	void changeText(SDL_Renderer* renderer, const char* newText);
	void draw(SDL_Renderer* renderer);

private:
	int x;
	int y;
	int width;
	int height;
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* surface;
	SDL_Texture* texture;

};

