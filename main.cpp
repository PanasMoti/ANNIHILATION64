#define SDL_MAIN_HANDLED

#include <iostream>

#include "Game.h"


int main(int argc, char** argv) {
	Game* game = new Game();
	while (!game->shouldClose) {
		game->update();
		game->render();
	}

	delete game;
	return 0;
}
