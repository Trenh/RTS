#include <SDL.h>
#include "Game.h"

#pragma once
class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	bool handleAllInput(Game* game);

private:
	bool ctrl ;
};

