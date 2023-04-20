#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "linalg.h"
#include "Texture.h"
#include "Colors.h"
#include "Sprite.h"
using namespace linalg::aliases;

class RenderWindow
{
public:
	RenderWindow(const char* title, int width, int height);
	SDL_Texture* loadSDL_Texture(const char* fileName);
	TTF_Font* loadFont(const char* fileName, int size = 32);
    Texture loadTexture(const char* fileName);
    void close();
    ~RenderWindow();
	void clear_screen();
	void draw(SDL_Texture* texture);

    void draw(const Texture& texture,int x,int y);
    void draw(const Texture& texture,int2 pos);
    void draw(const Texture& texture,SDL_Rect dst);
    void draw(const Texture& texture);
    void draw(const std::string& txt,TTF_Font* font,int x,int y, SDL_Color color={255,255,255,255},bool isCentered = false);
    void draw(const std::string& txt,TTF_Font* font,int2 pos,SDL_Color color={255,255,255,255},bool isCentered = false);
    void draw(const std::string& txt,TTF_Font* font,int2 pos,Uint32 wrappedlen,SDL_Color color={255,255,255,255},bool isCentered = false,SDL_Rect* rect = nullptr);
    void draw(const std::string& txt,TTF_Font* font,int x,int y,Uint32 wrappedlen,SDL_Color color={255,255,255,255},bool isCentered = false,SDL_Rect* rect = nullptr);
    void draw(const char* str,TTF_Font* font,int x,int y,SDL_Color color);
    void draw(SDL_Rect sdlRect,SDL_Color color,bool filled = false);
    void drawShaded(const std::string& str,TTF_Font* font,int2 p,SDL_Color fg = clWHITE,SDL_Color bg = clBLACK,bool isCentered = false);
    void draw_guidlines(const SDL_Color& sdlColor);
    void draw(Sprite* sprite,int x,int y,int frame);
	void display();
	int2 GetMousePosition() const;
    int2 ScreenCenter() const;
    int2 GetScreenSize() const;
    void destroy();
    void update();
    void clear_buffer() const;
    void create_buffer();
    void render(int x,int y,SDL_Color color);
    void render(int2 point,SDL_Color color);
    void render(int x,int y,uint32_t color);
    void render_vertical_line(int x, int y0,int y1,SDL_Color color);
    void render(SDL_Surface* surface,SDL_Rect* srcRect,SDL_Rect* dstRect);
    SDL_Renderer* GetRenderer();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
    std::pair<SDL_Surface*,SDL_Texture*> buffer;

};

