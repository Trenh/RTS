#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <array>
#include <iostream>
#include "RessourceManager.h"
#include "Unit.h"
#include <time.h> 

#pragma once
class Game
{
public:
	Game(int ScreenHeight,int ScreenWidth);
	~Game();
	void update();
	void load(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer);
	std::vector<Unit> getPlayersUnits();
	std::vector<Unit> getEnemyUnits();
	std::vector<Unit*> getSelectedUnits();
	std::vector<Unit*> getAttackedEnemyUnits();
	std::array<int, 4> getCoord();
	bool getLassoSelection();
	void setLassoSelection(bool lasso);
	void startMouseSelection();
	void updateMouseCoord();
	void unit0Attack(int xpos,int ypos);
	void updateAttackedEnemy(int i);
	void resetSelected();
	void addToSelectedUnits(int i);
	void reorganisedCoords();
	bool findingAlready(int i);
	bool playerIsDead();
	bool enemyIsDead();
private:

	bool lassoSelection;
	std::array<int, 4> selectCoord;

	std::vector<Unit> playersUnits;
	std::vector<Unit> enemyUnits;
	std::vector<Unit*> selectedUnits;
	std::vector<Unit*> attackedEnemyUnits;

};

