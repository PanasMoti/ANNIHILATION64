#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_Text.h"
#include "linalg.h"

using namespace linalg::aliases;

class RenderWindow
{
public:
	RenderWindow(const char* title, int width, int height);
	SDL_Texture* loadTexture(const char* fileName);
	TTF_Font* loadFont(const char* fileName, int size = 32);
	~RenderWindow();
	void clear_screen();
	void draw(SDL_Texture* texture);
	void draw(SDL_Text text, int x, int y);
	void display();
	int2 GetMousePosition() const;

	
	
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};

