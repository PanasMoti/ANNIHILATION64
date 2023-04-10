#include "RenderWindow.h"
#include <iostream>
RenderWindow::RenderWindow(const char* title, int width, int height)
{
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadSDL_Texture(const char* fileName)
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
    r.x -= r.w/2;
    r.y -= r.h/2;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, texture, nullptr, &r);
	SDL_DestroyTexture(texture);
}

int2 RenderWindow::ScreenCenter() const {
    int x,y;
    SDL_GetWindowSize(window,&x,&y);
    return {x/2,y/2};
}

int2 RenderWindow::GetScreenSize() const {
    int x,y;
    SDL_GetWindowSize(window,&x,&y);
    return {x,y};
}

Texture RenderWindow::loadTexture(const char *fileName) {
    return Texture(fileName,renderer);
}

void RenderWindow::draw(const Texture& texture, int2 pos) {
    SDL_Rect sdlRect {pos.x,pos.y,texture.GetWidth(),texture.GetHeight()};
    SDL_RenderCopy(renderer,texture.SDL_Tex(), nullptr,&sdlRect);
}

void RenderWindow::draw(const Texture& texture, int x, int y) {
    SDL_Rect sdlRect = {x,y,texture.GetWidth(),texture.GetHeight()};
    SDL_RenderCopy(renderer,texture.SDL_Tex(), nullptr,&sdlRect);
}

void RenderWindow::draw(const Texture &texture) {
    SDL_RenderCopy(renderer,texture.SDL_Tex(), nullptr, nullptr);
}

void RenderWindow::draw_guidlines(const SDL_Color &sdlColor) {
    int dotSize = 16; // Size of the dots
    int dotSpace = 16; // Space between the dots
    auto SDL_DrawPoint = [this] (int2 pos) {
        SDL_Rect sdlRect = {pos.x-4,pos.y-4,8,8};
        SDL_RenderFillRect(this->renderer,&sdlRect);
    };
    auto SDL_DrawDottedLine = [&dotSize,&dotSpace, this, &SDL_DrawPoint] (int2 start,int2 end)  {
        int x1 = start.x,   y1=start.y;
        int x2 = end.x,     y2=end.y;
        for (int i = x1; i <= x2; i += (dotSize + dotSpace)) {
            SDL_DrawPoint({i, y1}); // Draw a dot
        }

        for (int j = y1; j <= y2; j += (dotSize + dotSpace)) {
            // Draw a dot
            SDL_DrawPoint({x2, j});
        }
    };


    auto p = this->ScreenCenter();
    auto s = this->GetScreenSize();
    SDL_SetRenderDrawColor(renderer,sdlColor.r,sdlColor.g,sdlColor.b,sdlColor.a);
    SDL_DrawDottedLine({p.x,0},{p.x,s.y});
    SDL_DrawDottedLine({0,p.y},{s.x,p.y});
}

void RenderWindow::draw(SDL_Rect sdlRect, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    if(filled) SDL_RenderFillRect(renderer,&sdlRect);
    else SDL_RenderDrawRect(renderer,&sdlRect);
}

void RenderWindow::draw(SDL_Text text, int2 pos) {
    this->draw(text,pos.x,pos.y);
}

void RenderWindow::draw(SDL_Text text, int x, int y, Uint32 wrappedlen) {
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(*text.font,text.text.c_str(),text.color,wrappedlen);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect r; r.x = x; r.y = y; r.w = surface->w; r.h = surface->h;
    r.x -= r.w/2;
    r.y -= r.h/2;
    std::cout << r.x << "," << r.y << "," << r.w << "," << r.h << std::endl;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}

void RenderWindow::draw_text_uncentered(SDL_Text text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(*text.font, text.text.c_str(), text.color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect r; r.x = x; r.y = y; r.w = surface->w; r.h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}

