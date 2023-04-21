#define SDL_MAIN_HANDLED

#include <iostream>

#include "Game.h"
#include "header_onlys/GameData.h"


int main(int argc, char** argv) {
    GameData& gameData = GameData::self();
    gameData.init();
    srand(time(nullptr));
	Game* game = new Game();
	while (!game->shouldClose) {
		game->update();
		game->render();
        game->lateUpdate();
	}
    game->destroy();
//	delete game;
	return 0;
}
