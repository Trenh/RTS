#pragma once
#include <SDL.h>

class Unit
{
public:
	Unit();
	~Unit();
	Unit(int px, int py, int ray);
	Unit(int rad,int sHeight,int sWidth, bool player);
	
	void draw(SDL_Renderer* renderer);
	void reset(int height, int width);
	bool IsDead;
	bool hasTarget;

	void update();
	void updateAI();
	void setDestination(int destX, int destY);
	void setStart(int screenHeight, int screenWidth,bool player);
	void setDestination(int destX,int destY,int offset,int depl);
	void setDestination(int destX,int destY,int unitNumber,int numberUnits,int spacing);
	void selectDraw(SDL_Renderer* renderer);

	int getX();
	int getY();
	int getSize();
	int getRange();

	bool inUnit(int pX, int pY);
	int inNearby(int px,int pY);
	bool isInRange(int px,int pY,int ran);


private:
	int x;
	int y;
	int rayon;
	int speed;
	int toX;
	int toY;
	double angle;
	int range;
};