#include "Game.h"



Game::Game(int Screen_Height,int Screen_Width)
{
	for (int i = 0; i < 10; i++) {
		playersUnits.push_back(Unit(5, Screen_Height, Screen_Width, true));

	}
	for (int i = 0; i < 8; i++) {
		enemyUnits.push_back(Unit(5, Screen_Height, Screen_Width, false));
	}
	lassoSelection = false;
}


Game::~Game()
{
}


void Game::update() {
//updating units
	
	for (int i = 0; i < playersUnits.size(); i++) {
		//if dead pass
		if (playersUnits.at(i).IsDead) {
			continue;
		}
		//update unit
		Unit* currentUnit = &playersUnits.at(i);
		currentUnit->update();
		
		//if at least one ennemy is attacked and one units is selected and this is the one
		if (attackedEnemyUnits.size() && selectedUnits.size()&& selectedUnits.at(0) == currentUnit) {
			//for each units attacked we check if it's in our unit range
			for (int j = 0; j < attackedEnemyUnits.size(); j++) {
				Unit* temp = attackedEnemyUnits.at(j);

				if (currentUnit->isInRange(temp->getX(), temp->getY(), currentUnit->getRange())) {
					//if it's in unite range Kill the units and erase it from both list
					temp->IsDead = true;
					
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
		currentUnit->updateAI();
		int nearDist = -1;
		Unit* nearest = currentUnit;
		//find if the unit should have a target
		//if there are multiple possibility find the nearest 
		for (int j = 0; j < playersUnits.size(); j++) {
			if (playersUnits.at(j).IsDead) {

			}
			else {
				
				int newDist = playersUnits.at(j).inNearby(currentUnit->getX(), currentUnit->getY());
				if (newDist > -1) {
					if (nearest == currentUnit || (nearDist > newDist)) {
						nearDist = newDist;
						nearest = &playersUnits.at(j);
						currentUnit->hasTarget = true;
					}
				}
			}
		}
		//if the unit has a target set destination to the target position
		//and if it's in range kill him and erase him from the selecteds units
		if (currentUnit->hasTarget) {
			currentUnit->setDestination(nearest->getX(), nearest->getY());
			if (currentUnit->isInRange(nearest->getX(), nearest->getY(), currentUnit->getRange())) {
				nearest->IsDead = true;
				for (int j = selectedUnits.size() - 1; j >= 0; j--) {
					if (nearest == selectedUnits.at(j)) {
						selectedUnits.erase(selectedUnits.begin() + j);
						break;
					}
				}
				currentUnit->hasTarget = false;
			}
		}
	
	}
}
void Game::load(SDL_Renderer* renderer) {

}
void Game::draw(SDL_Renderer* renderer) {
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

		selectedUnits.at(i)->selectDraw(renderer);

	}
	//render Ennemy in another color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	for (int i = 0; i < enemyUnits.size(); i++) {
		enemyUnits.at(i).draw(renderer);
	}
}

std::vector<Unit> Game::getPlayersUnits() {
	return playersUnits;
}
std::vector<Unit> Game::getEnemyUnits() {
	return enemyUnits;
}

std::vector<Unit*> Game::getSelectedUnits() {
	return selectedUnits;
}

std::vector<Unit*> Game::getAttackedEnemyUnits() {
	return attackedEnemyUnits;
}

std::array<int, 4> Game::getCoord() {
	return selectCoord;
}

bool Game::getLassoSelection() {
	return lassoSelection ;
}

void Game::setLassoSelection(bool lasso) {
	lassoSelection = lasso;
}

void Game::startMouseSelection() {
	SDL_GetMouseState(&selectCoord[0], &selectCoord[1]);
	SDL_GetMouseState(&selectCoord[2], &selectCoord[3]);

}

void Game::updateMouseCoord() {
	SDL_GetMouseState(&selectCoord[2], &selectCoord[3]);
}

void Game::unit0Attack(int xpos, int ypos) {
	selectedUnits.at(0)->setDestination(xpos, ypos);

}

void Game::updateAttackedEnemy(int i) {
	attackedEnemyUnits.push_back(&enemyUnits.at(i));
}

void Game::resetSelected() {
	selectedUnits.clear();

}

void Game::addToSelectedUnits(int i) {
	selectedUnits.push_back(&playersUnits.at(i));

}
void Game::reorganisedCoords() {

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
}

bool Game::findingAlready(int i) {
	return std::find(selectedUnits.begin(), selectedUnits.end(), &playersUnits.at(i)) == selectedUnits.end();
}

//check if player is dead
bool Game::playerIsDead() {
	for (int i = 0; i < playersUnits.size(); i++) {
		if (!playersUnits.at(i).IsDead) {
			return false;
		}
	}
	return true;
}

bool Game::enemyIsDead() {
	for (int i = 0; i < enemyUnits.size(); i++) {
		if (!enemyUnits.at(i).IsDead) {
			return false;
		}
	}
	return true;
}