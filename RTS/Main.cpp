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
#include "Game.h"
#include "InputHandler.h"
using std::cout;
using std::endl;

//Definitions

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool quit = false;
bool gameon= true;


Game game = Game(SCREEN_HEIGHT, SCREEN_WIDTH);
InputHandler inputHandler = InputHandler();
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
		 quit=handleInput();

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

	// Game.load();
	game.load(renderer);
	
	//load text
	ResultDisplay.load(renderer, "");

}

bool handleInput() {
	bool  doquit = inputHandler.handleAllInput(&game);
	cout << doquit << endl;
	return doquit;

}

void update(SDL_Renderer* renderer) {
	if (!gameon) {
		return;
	}
	//Game .update
	game.update();
	//if one of the side has no units end the game
	if (game.enemyIsDead() || game.playerIsDead()) {
		//cout << "GAME ENDED";
		gameon = false;
		if (game.enemyIsDead()) {
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
	//	Game.update
		
		game.draw(renderer);
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

