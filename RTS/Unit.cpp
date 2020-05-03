#include "Unit.h"
#include <stdlib.h>   
#include <time.h> 
#include <iostream>

using std::cout;
using std::endl;
Unit::Unit()
{
}

Unit::Unit(int px,int py,int ray)
	:x(px),y(py),rayon(ray),toX(px),toY(py),speed(3),IsDead(false),range(15), hasTarget(false)
{
}
Unit::Unit(int ray,int sH,int sW,bool player)
	:x(),y(),rayon(ray),toX(),toY(),speed(3),IsDead(false),range(15),hasTarget(false)
{
	setStart(sH, sW, player);
	if (player) {
		range = 25;
	}
}

Unit::~Unit()
{
}

//draw a circle with position and radius 
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{

	
		int y1, y2;
		SDL_Rect rect;
		for (y1 = -radius, y2 = radius; y1; ++y1, --y2)
		{
			int xr = (int)(sqrt(radius*radius - y1 * y1) + 0.5);
			rect.x = centreX - xr;
			rect.y = centreY + y1;
			rect.w = 2 * xr;
			rect.h = 1;
			SDL_RenderFillRect(renderer, &rect);
			rect.y = centreY + y2;
			rect.h = 1;
			SDL_RenderFillRect(renderer, &rect);
		}
		rect.x = centreX - radius;
		rect.y = centreY;
		rect.w = 2 * radius;
		rect.h = 1;
		SDL_RenderFillRect(renderer, &rect);
	
}

//draw the unit if it is alive
void Unit::draw(SDL_Renderer* renderer)
{
	if (IsDead) {
		return;
	}
	DrawCircle(renderer, x, y, rayon);
}

//reset position 
void Unit::reset(int height, int width) {
	x = rand() % (width / 4);
	y = rand() % (height / 4);
	toX = x;
	toY = y;

}

//update unit
void Unit::update() {
	//if the unit is not at is destination position update is position
	if (x != toX || y != toY) {
		angle = atan2(toY - y,toX-x);
		double dx = (double)(cos(angle) * speed);
		double dy = (double)(sin(angle) * speed);

		if (abs(toX - x) < speed) {
			x = toX;
		
		}
		else {
			x += dx;

		}
		if (abs(toY - y) < speed) {
			y = toY;
		
		}
		else {
			y += dy;

		}
	}
}

// update movement for AI 
void Unit::updateAI() {
	//if the AI doesn't have a target go the  15 15 position with a random speed in each axes
	if (!hasTarget) {
		toX = 15;
		toY = 15;
		angle = atan2(toY - y, toX - x);
		double dx = (double)(cos(angle) * (rand() % speed));
		double dy = (double)(sin(angle) * (rand() % speed));

		x += dx;
		y += dy;
	}
	//else update normally
	else {
		
		update();

	}
	if (toX < 0 ) {
		toX = 15;
	}
	if (toY < 0 ) {
		toY = 15;
	}
}


//Set destination with just destination
void Unit::setDestination(int destX, int destY) {
	int offx = (rand() % 6) - 3;

	int offy = (rand() % 6) - 3;
	toX = destX + offx;
	toY = destY + offy;
}
//set destination with max offset and depl between each offset
void Unit::setDestination(int destX, int destY,int offset,int depl) {
	int offx = ((rand() % (offset*2)) - offset)*depl;

	int offy = ((rand() % (offset * 2)) - offset)*depl;
	toX = destX + offx;
	toY = destY + offy;
}

//set destination escadron
void Unit::setDestination(int destX, int destY, int unitNumber, int numberUnits, int spacing) {
	int unitPerRows = floor(sqrt(numberUnits + 2));
	int rowNumbers =ceil((double) numberUnits/unitPerRows) ;
	int offsetX = ((unitNumber % unitPerRows)*spacing)-(unitPerRows-1)*spacing/2;
	int offsetY = ((floor(unitNumber / unitPerRows))*spacing)-(rowNumbers-1)*spacing/2;
	toX = destX + offsetX;
	toY = destY + offsetY;
	// verify offset : 
	//cout << "my offsets :" << offsetX << " and " << offsetY << endl;

}

int Unit::getX() {
	return x;
}

int Unit::getY() {
	return y;
}

//draw rectangle around unit
void Unit::selectDraw(SDL_Renderer* renderer) {
	int startX = x - rayon - 2;
	int endX = x + rayon + 2;
	int startY = y - rayon - 2;
	int endY = y + rayon + 2;

	SDL_RenderDrawLine(renderer, startX, startY, startX, endY);
	SDL_RenderDrawLine(renderer, startX, startY, endX, startY);
	SDL_RenderDrawLine(renderer, endX, startY, endX, endY);
	SDL_RenderDrawLine(renderer, startX, endY, endX, endY);
}

//check if position is in unit
bool Unit::inUnit(int pX,int pY){
	return  x-rayon<=pX 
		&&  pX<= x + rayon
		&& y - rayon <= pY
		&& pY <= y + rayon;
}

int Unit::getSize() {
	return (rayon * 2);
}

//set correctly stat position 
void Unit::setStart(int sHeight, int sWidth, bool player) {
	if (player) {
		reset(sHeight, sWidth);
	}
	else {
		x = rand() % (sWidth / 4) + (sWidth * 3/ 4);
		y = rand() % (sHeight / 4) + (sHeight * 3 / 4);

		toX = x;
		toY = y;
	}
}

int Unit::getRange() {
	return range;
}

//check if tje position is nearby
int Unit::inNearby(int pX, int pY) {
	if (x - rayon - 30 <= pX
		&& pX <= x + rayon + 30
		&& y - rayon - 30 <= pY
		&& pX <= x + rayon + 30) {
		return sqrt(pow(pX-x,2)+ pow(pY - y, 2));
	}
	else 
		return -1;
	}

// check if the position is in attack range
bool Unit::isInRange(int pX, int pY,int ran) {
	return  x - rayon - ran <= pX
		&& pX <= x + rayon + ran
		&& y - rayon - ran <= pY
		&& pX <= x + rayon + ran;
}