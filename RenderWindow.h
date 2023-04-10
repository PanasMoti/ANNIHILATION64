#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_Text.h"
#include "linalg.h"
#include "Texture.h"

using namespace linalg::aliases;

class RenderWindow
{
public:
	RenderWindow(const char* title, int width, int height);
	SDL_Texture* loadSDL_Texture(const char* fileName);
	TTF_Font* loadFont(const char* fileName, int size = 32);
    Texture loadTexture(const char* fileName);
    ~RenderWindow();
	void clear_screen();
	void draw(SDL_Texture* texture);
	void draw(SDL_Text text, int x, int y);
    void draw(SDL_Text text,int2 pos);
    void draw(SDL_Text text,int x,int y, Uint32 wrappedlen);
    void draw(const Texture& texture,int x,int y);
    void draw(const Texture& texture,int2 pos);
    void draw(const Texture& texture);
    void draw_text_uncentered(SDL_Text text,int x,int y);
    void draw(SDL_Rect sdlRect,SDL_Color color,bool filled = false);
    void draw_guidlines(const SDL_Color& sdlColor);
	void display();
	int2 GetMousePosition() const;
    int2 ScreenCenter() const;
    int2 GetScreenSize() const;

	
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};

