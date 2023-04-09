#include "RenderWindow.h"

RenderWindow::RenderWindow(const char* title, int width, int height)
{
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* fileName)
{
	SDL_Texture* texture = IMG_LoadTexture(this->renderer, fileName);
	return texture;
}


RenderWindow::~RenderWindow()
{
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void RenderWindow::clear_screen()
{
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->renderer);
}

void RenderWindow::draw(SDL_Texture* texture)
{
	SDL_RenderCopy(this->renderer, texture, nullptr, nullptr);
}

void RenderWindow::display()
{
	SDL_RenderPresent(this->renderer);
}

int2 RenderWindow::GetMousePosition() const
{
	int2 temp;
	SDL_GetMouseState(&temp.x, &temp.y);
	return temp;
}

TTF_Font* RenderWindow::loadFont(const char* fileName, int size) {
	return TTF_OpenFont(fileName, size);
}

void RenderWindow::draw(SDL_Text text, int x, int y) {
	SDL_Surface* surface = TTF_RenderText_Solid(*text.font, text.text.c_str(), text.color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect r; r.x = x; r.y = y; r.w = surface->w; r.h = surface->h;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, nullptr, &r);
	SDL_DestroyTexture(texture);
}

int2 RenderWindow::ScreenCenter() const {
    int x,y;
    SDL_GetWindowSize(window,&x,&y);
    return {x/2,y/2};
}

