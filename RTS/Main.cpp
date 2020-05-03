#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Text.h"
#include <cstdio>
#include "SoundEffect.h"
#include <vector>
#include <array>
#include <iostream>
#include "RessourceManager.h"
#include "Unit.h"
#include <time.h> 

using std::cout;
using std::endl;

//Definitions

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool quit = false;
bool lassoSelection = false;
bool ctrl = false;
bool gameon= true;

std::array<int, 4> selectCoord;

std::vector<Unit> playersUnits;
std::vector<Unit> enemyUnits;
std::vector<Unit*> selectedUnits;
std::vector<Unit*> attackedEnemyUnits;


RessourceManager ressManager = RessourceManager();
Text ResultDisplay = Text(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 3, 250, 50);


//fonctions
void load(SDL_Renderer*);
bool handleInput();
void update(SDL_Renderer*);
void draw(SDL_Renderer*);
void close(SDL_Window*, SDL_Renderer*);

bool AABBcollision(SDL_Rect* rectA, SDL_Rect* rectB);
bool playerIsDead();

int main(int argc, char* args[]) {


	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	srand((unsigned)time(NULL));


	//Init Window
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	window = SDL_CreateWindow("RTS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	TTF_Init();
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//UnitCreation
	for (int i = 0; i < 10; i++) {
		playersUnits.push_back(Unit(5, SCREEN_HEIGHT, SCREEN_WIDTH, true));

	}
	for (int i = 0; i < 8; i++) {
		enemyUnits.push_back(Unit(5, SCREEN_HEIGHT, SCREEN_WIDTH, false));
	}

	//Load
 	load(renderer);
	while (!quit) {
		//Inputs
		quit = handleInput();

		//Update
		update(renderer);

		//Draw 
		draw(renderer);
	}
	//END
	close(window, renderer);
	return 0;
}


void load(SDL_Renderer* renderer) {
	//image ressources load 
	//ressManager.setAllTexture(renderer, imageToLoad);

	//load text
	ResultDisplay.load(renderer, "");

}

bool handleInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			//right click
			if (e.button.button == SDL_BUTTON_RIGHT) {
				//get mouse position
				int xMpos, yMpos;
				SDL_GetMouseState(&xMpos, &yMpos);
				bool doMove= true;
				//if one unit selected
				//if the right click is near of a enemy attack him
				if (selectedUnits.size() == 1) {
					for (int i = 0; i < enemyUnits.size(); i++) {
						if (enemyUnits.at(i).inNearby(xMpos, yMpos)>-1) {
							(*selectedUnits.at(0)).setDestination(xMpos, yMpos);
							attackedEnemyUnits.push_back(&enemyUnits.at(i));
							doMove = false;
						}

					}
				}
				//if move is still on set destination of units
				if (doMove) {

					for (int i = 0; i < selectedUnits.size(); i++) {
					
						(*selectedUnits.at(i)).setDestination(xMpos, yMpos,i,selectedUnits.size(), (*selectedUnits.at(i)).getSize()*1.5);
					
					}
				}

			}
			if (e.button.button == SDL_BUTTON_LEFT) {
				//if ctrl is off deselect units before selecting more
				if (!ctrl) {
					selectedUnits.clear();
				}
				if (!lassoSelection) {
					lassoSelection = true;
					SDL_GetMouseState(&selectCoord[0], &selectCoord[1]);
					SDL_GetMouseState(&selectCoord[2], &selectCoord[3]);
				}
				else {
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			// if same coordinate between start and end then its a start
			lassoSelection = false;
			if (selectCoord[0] == selectCoord[2] && selectCoord[1] == selectCoord[3]) {
				for (int i = 0; i < playersUnits.size(); i++) {
					if ( !playersUnits.at(i).IsDead && playersUnits.at(i).inUnit(selectCoord[0], selectCoord[1])) {
						if (std::find(selectedUnits.begin(), selectedUnits.end(), &playersUnits.at(i)) == selectedUnits.end() ) {
							selectedUnits.push_back(&playersUnits.at(i));
						}
						break;
					}
				}
			}

			//make lasso selection in 2 ways

			if (selectCoord[0] > selectCoord[2]) {
				int temp = selectCoord[0];
				selectCoord[0] = selectCoord[2];
				selectCoord[2] = temp;
			}
			if (selectCoord[1] > selectCoord[3]) {
				int temp = selectCoord[1];
				selectCoord[1] = selectCoord[3];
				selectCoord[3] = temp;
			}

			//push back to selected units 
			for (int i = 0; i < playersUnits.size(); i++) {
				if (!playersUnits.at(i).IsDead &&
					selectCoord[0] <= playersUnits.at(i).getX()
					&& playersUnits.at(i).getX() <= selectCoord[2]
					&& selectCoord[1] <= playersUnits.at(i).getY()
					&& playersUnits.at(i).getY() <= selectCoord[3])
				{
					if (std::find(selectedUnits.begin(), selectedUnits.end(), &playersUnits.at(i)) == selectedUnits.end()) {
						selectedUnits.push_back(&playersUnits.at(i));

					}
				}
			}
			break;
		case SDL_MOUSEMOTION:
			if (lassoSelection) {
				SDL_GetMouseState(&selectCoord[2], &selectCoord[3]);

			}

		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_LCTRL) {
				ctrl = true;
			}
			break;
		
		case SDL_KEYUP:
			if (e.key.keysym.sym == SDLK_LCTRL) {
				ctrl = false;
			}
			break;

		case SDL_QUIT:
			return true;

		}
		return false;
		
	}
}

void update(SDL_Renderer* renderer) {
	if (!gameon) {
		return;
	}
	//updating units
	
	for (int i = 0; i < playersUnits.size(); i++) {
		//if dead pass
		if (playersUnits.at(i).IsDead) {
			continue;
		}
		//update unit
		Unit* currentUnit = &playersUnits.at(i);
		(*currentUnit).update();
		
		//if at least one ennemy is attacked and one units is selected and this is the one
		if (attackedEnemyUnits.size() && selectedUnits.size()&& selectedUnits.at(0) == currentUnit) {
			//for each units attacked we check if it's in our unit range
			for (int j = 0; j < attackedEnemyUnits.size(); j++) {
				Unit* temp = attackedEnemyUnits.at(j);

				if ((*currentUnit).isInRange((*temp).getX(), (*temp).getY(), (*currentUnit).getRange())) {
					//if it's in unite range Kill the units and erase it from both list
					(*temp).IsDead = true;
					
					for (int k = enemyUnits.size() - 1; k >= 0; k--) {
						if (attackedEnemyUnits.at(j) == &enemyUnits.at(k)) {
							enemyUnits.erase(enemyUnits.begin() + k);
							break;
						}
							
					}
					attackedEnemyUnits.erase(attackedEnemyUnits.begin() + (j));
					

				}
			}
		}
		
	}
	//updating enemy units
	//
	for (int i = 0; i < enemyUnits.size(); i++) {
		if (enemyUnits.at(i).IsDead) {
			continue;
		}
		Unit* currentUnit = &enemyUnits.at(i);
		(*currentUnit).updateAI();
		int nearDist = -1;
		Unit* nearest = currentUnit;
		//find if the unit should have a target
		//if there are multiple possibility find the nearest 
		for (int j = 0; j < playersUnits.size(); j++) {
			if (playersUnits.at(j).IsDead) {

			}
			else {
				
				int newDist = playersUnits.at(j).inNearby((*currentUnit).getX(), (*currentUnit).getY());
				if (newDist > -1) {
					if (nearest == currentUnit || (nearDist > newDist)) {
						nearDist = newDist;
						nearest = &playersUnits.at(j);
						(*currentUnit).hasTarget = true;
					}
				}
			}
		}
		//if the unit has a target set destination to the target position
		//and if it's in range kill him and erase him from the selecteds units
		if ((*currentUnit).hasTarget) {
			(*currentUnit).setDestination((*nearest).getX(), (*nearest).getY());
			if ((*currentUnit).isInRange((*nearest).getX(), (*nearest).getY(), (*currentUnit).getRange())) {
				(*nearest).IsDead = true;
				for (int j = selectedUnits.size() - 1; j >= 0; j--) {
					if (nearest == selectedUnits.at(j)) {
						selectedUnits.erase(selectedUnits.begin() + j);
						break;
					}
				}
				(*currentUnit).hasTarget = false;
			}
		}
	
	}

	//if one of the side has no units end the game
	if (!enemyUnits.size() || playerIsDead()) {
		//cout << "GAME ENDED";
		gameon = false;
		if (!enemyUnits.size()) {
			ResultDisplay.changeText(renderer, "Congratulation player You win");
		}
		else
		{
			ResultDisplay.changeText(renderer, "Sorry Player you lose");
		}
	}
	
}

//Drawing on the renderer
void draw(SDL_Renderer* renderer) {

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x50, 0xFF);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	if (!gameon) {
		ResultDisplay.draw(renderer);

	}
	else {
	
		for (int i = 0; i < playersUnits.size(); i++) {
			playersUnits.at(i).draw(renderer);
		}
		
		//render lasso
		if (lassoSelection) {
			SDL_RenderDrawLine(renderer, selectCoord[0], selectCoord[1], selectCoord[0], selectCoord[3]);
			SDL_RenderDrawLine(renderer, selectCoord[0], selectCoord[1], selectCoord[2], selectCoord[1]);
			SDL_RenderDrawLine(renderer, selectCoord[2], selectCoord[1], selectCoord[2], selectCoord[3]);
			SDL_RenderDrawLine(renderer, selectCoord[0], selectCoord[3], selectCoord[2], selectCoord[3]);
		}
		//draw Rectangle Selected Tile
		for (int i = 0; i < selectedUnits.size(); i++) {
	
			(*selectedUnits.at(i)).selectDraw(renderer);
	
		}
		//render Ennemy in another color
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		for (int i = 0; i < enemyUnits.size(); i++) {
			enemyUnits.at(i).draw(renderer);
		}
	}

	//Render
	SDL_RenderPresent(renderer);

}
//closing the window
void close(SDL_Window* window, SDL_Renderer* renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}

//check if player is dead
bool playerIsDead() {
	for (int i = 0; i < playersUnits.size(); i++) {
		if (!playersUnits.at(i).IsDead) {
			return false;
		}
	}
	return true;
}