#define SDL_MAIN_HANDLED

#include <iostream>

#include "Game.h"


int main(int argc, char** argv) {
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
