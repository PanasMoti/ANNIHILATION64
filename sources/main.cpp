#define SDL_MAIN_HANDLED

#include <iostream>

#include "Game.h"
#include "header_onlys/GameData.h"

/// the main function or the entry point of the program
/// \param argc the amount of arguments the executable recives from the command line,since our program isn't meant to be used by the command
/// line , we are not using this in our program, however it could come in handy in the future and is a good practice to have it anyway
/// \param argv an array of string which represents the arguments the executable recives from the command line. same as the argc, we don't intened on using the program from the commandline
/// but it could come in handy in the future and can be used to handle asset loading errors
/// \return in C++ the main function is an int and returns 0 upon successful run.
int main(int argc, char** argv) {
    GameData& gameData = GameData::self(); // accessing the instance of GameData and initalizing it
    gameData.init();
    srand(time(nullptr)); // giving the random system the time of the system as a seed so our program will generate different random values every run
	Game* game = new Game(); // creating a new Game Object that lives on the heap
	while (!game->shouldClose) { // this is our game while loop which means the program will run as long as the game shouln't close
		game->update(); // we update the game
		game->render(); // we render/draw the game to the screen
        game->lateUpdate(); // we let our game process late updates ( something our stateManager needs )
	} // while loop
    game->destroy(); // when we exit the while loop that means we should close the game so we call the destroy function which
    // closes the RenderWindow Object, closes TTF , IMG and SDL, deletes everything and closes
//	delete game;
	return 0; // we return 0 for successful execution of the program
}
