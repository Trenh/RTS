#include "InputHandler.h"


using std::cout;

InputHandler::InputHandler()
{
	ctrl = false;
}


InputHandler::~InputHandler()
{
}


bool InputHandler::handleAllInput(Game* game) {

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			//right click
			if (event.button.button == SDL_BUTTON_RIGHT) {
				cout << "right button";
				//get mouse position
				int xMpos, yMpos;
				SDL_GetMouseState(&xMpos, &yMpos);
				bool doMove = true;
				//if one unit selected
				//if the right click is near of a enemy attack him
				if (game->getSelectedUnits().size() == 1) {
					for (int i = 0; i < game->getEnemyUnits().size(); i++) {
						if (game->getEnemyUnits().at(i).inNearby(xMpos, yMpos) > -1) {
							//modify
							game->unit0Attack(xMpos, yMpos);
							//game->getSelectedUnits().at(0)->setDestination(xMpos, yMpos);
							game->updateAttackedEnemy(i);
							//game->getAttackedEnemyUnits().push_back(&game->getEnemyUnits().at(i));
							doMove = false;
						}

					}
				}
				//if move is still on set destination of units
				if (doMove) {

					for (int i = 0; i < game->getSelectedUnits().size(); i++) {
						//modify
						game->getSelectedUnits().at(i)->setDestination(xMpos, yMpos, i, game->getSelectedUnits().size(), game->getSelectedUnits().at(i)->getSize()*1.5);

					}
				}

			}
			if (event.button.button == SDL_BUTTON_LEFT) {
				//if ctrl is off deselect units before selecting more
				cout << "left mouse selection";
				if (!ctrl) {
					//modify
					game->resetSelected();
					//game->getSelectedUnits().clear();
				}
				//modify
				if (!game->getLassoSelection()) {
					game->setLassoSelection(true);
					game->startMouseSelection();
//					SDL_GetMouseState(&game->getCoord()[0], &game->getCoord()[1]);
//					SDL_GetMouseState(&game->getCoord()[2], &game->getCoord()[3]);
				}
				else {
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			////////// A modifier ///////
			// if same coordinate between start and end then its a start
			game->setLassoSelection(false);
			if (game->getCoord()[0] == game->getCoord()[2] && game->getCoord()[1] == game->getCoord()[3]) {
				for (int i = 0; i < game->getPlayersUnits().size(); i++) {
					if (!game->getPlayersUnits().at(i).IsDead && game->getPlayersUnits().at(i).inUnit(game->getCoord()[0], game->getCoord()[1])) {
						if (game->findingAlready(i)) {
							//modify
							game->addToSelectedUnits(i);
//							game->getSelectedUnits().push_back(&game->getPlayersUnits().at(i));
						}
						break;
					}
				}
			}

			//make lasso selection in 2 ways
			game->reorganisedCoords();

			//push back to selected units 
			for (int i = 0; i < game->getPlayersUnits().size(); i++) {
				if (!game->getPlayersUnits().at(i).IsDead &&
					game->getCoord()[0] <= game->getPlayersUnits().at(i).getX()
					&& game->getPlayersUnits().at(i).getX() <= game->getCoord()[2]
					&& game->getCoord()[1] <= game->getPlayersUnits().at(i).getY()
					&& game->getPlayersUnits().at(i).getY() <= game->getCoord()[3])
				{
					///////////// maybe problem later
					if (game->findingAlready(i)) {
						game->addToSelectedUnits(i);
//						game->getSelectedUnits().push_back(&game->getPlayersUnits().at(i));

					}
				}
			}
			break;
		case SDL_MOUSEMOTION:
			if (game->getLassoSelection()) {
				game->updateMouseCoord();

			}

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LCTRL) {
				ctrl = true;
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LCTRL) {
				ctrl = false;
			}
			break;

		case SDL_QUIT:
			return true;

		}
		return false;

	}
	return false;
}