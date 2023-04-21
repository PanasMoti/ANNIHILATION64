#include "RenderWindow.h"
#include <iostream>
#include "header_onlys/GameData.h"


RenderWindow::RenderWindow(const char* title, int width, int height)
{
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* icon;
    icon = IMG_Load("assets/icon.png");
    SDL_SetWindowIcon(window,icon);
    SDL_FreeSurface(icon);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    buffer.first = nullptr;
    buffer.second = nullptr;
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

void RenderWindow::draw(const Texture& texture, int2 pos,bool2 isCentered) {
    SDL_Rect sdlRect {pos.x,pos.y,texture.GetWidth(),texture.GetHeight()};
    if(isCentered.x) sdlRect.x -= sdlRect.w/2;
    if(isCentered.y) sdlRect.y -= sdlRect.h/2;
    SDL_RenderCopy(renderer,texture.SDL_Tex(), nullptr,&sdlRect);
}

void RenderWindow::draw(const Texture& texture, int x, int y,bool2 isCentered) {
    SDL_Rect sdlRect = {x,y,texture.GetWidth(),texture.GetHeight()};
    if(isCentered.x) sdlRect.x -= sdlRect.w/2;
    if(isCentered.y) sdlRect.y -= sdlRect.h/2;
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



void RenderWindow::close() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = nullptr;
    renderer = nullptr;
}

void RenderWindow::draw(const char *str, TTF_Font *font, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font,str,color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect r; r.x = x; r.y = y; r.w = surface->w; r.h = surface->h;
    r.x -= r.w/2;
    r.y -= r.h/2;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}

void RenderWindow::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

}

void RenderWindow::draw(const std::string &txt, TTF_Font *font, int x, int y, SDL_Color color, bool isCentered) {
    SDL_Surface* surface = TTF_RenderText_Solid(font,txt.c_str(),color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect r; r.x = x; r.y = y; r.w = surface->w; r.h = surface->h;
    if(isCentered) {
        r.x -= r.w/2;
        r.y -= r.h/2;
    }
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}

void RenderWindow::draw(const std::string &txt, TTF_Font *font, int2 pos, SDL_Color color, bool isCentered) {
    SDL_Surface* surface = TTF_RenderText_Solid(font,txt.c_str(),color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect r; r.x = pos.x; r.y = pos.y; r.w = surface->w; r.h = surface->h;
    if(isCentered) {
        r.x -= r.w/2;
        r.y -= r.h/2;
    }
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);

}

void RenderWindow::draw(const std::string &txt, TTF_Font *font, int2 pos, Uint32 wrappedlen,SDL_Color color, bool isCentered,SDL_Rect *rect) {
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font,txt.c_str(),color,wrappedlen);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect r; r.x = pos.x; r.y = pos.y; r.w = surface->w; r.h = surface->h;
    if(isCentered) {
        r.x -= r.w/2;
        r.y -= r.h/2;
    }
    if(rect != nullptr) {
        rect->x = r.x;
        rect->y = r.y;
        rect->h = r.h;
        rect->w = r.w;
    }
//    std::cout << r.x << "," << r.y << "," << r.w << "," << r.h << std::endl;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}

void RenderWindow::draw(const std::string &txt, TTF_Font *font, int x, int y, Uint32 wrappedlen, SDL_Color color,
                        bool isCentered,SDL_Rect *rect) {
    SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font,txt.c_str(),color,wrappedlen);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect r; r.x = x; r.y = y; r.w = surface->w; r.h = surface->h;
    if(isCentered) {
        r.x -= r.w/2;
        r.y -= r.h/2;
    }
    if(rect != nullptr) {
        rect->x = r.x;
        rect->y = r.y;
        rect->h = r.h;
        rect->w = r.w;
    }
//    std::cout << r.x << "," << r.y << "," << r.w << "," << r.h << std::endl;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}


void RenderWindow::draw(const Texture &texture, SDL_Rect dst) {
    SDL_RenderCopy(renderer,texture.SDL_Tex(), nullptr,&dst);
}

void RenderWindow::create_buffer() {
    GameData& gameData = GameData::self();
    buffer.first = SDL_CreateRGBSurface(0,gameData.res.x,gameData.res.y,32,0,0,0,0);
    SDL_SetSurfaceRLE(buffer.first, true);
    clear_buffer();
    buffer.second = SDL_CreateTexture(renderer,buffer.first->format->format,SDL_TEXTUREACCESS_TARGET,gameData.res.x,gameData.res.y);
}

void RenderWindow::clear_buffer() const {
    SDL_LockSurface(buffer.first);
    SDL_memset(buffer.first->pixels,0,buffer.first->h*buffer.first->pitch);
    SDL_UnlockSurface(buffer.first);
}

void RenderWindow::update() {
    SDL_UpdateTexture(buffer.second, nullptr,buffer.first->pixels,buffer.first->pitch);
    SDL_RenderCopy(renderer,buffer.second, nullptr, nullptr);
}

void RenderWindow::render(int x, int y, SDL_Color color) {
    uint32_t c = SDL_MapRGBA(buffer.first->format,color.r,color.g,color.b,color.a);
    auto * const target_pixel = (Uint32 *) ((Uint8 *) buffer.first->pixels
                                            + y * buffer.first->pitch
                                            + x * buffer.first->format->BytesPerPixel);
    *target_pixel = c;
}



void RenderWindow::render(int2 point, SDL_Color color) {
    render(point.x,point.y,color);
}

void RenderWindow::render_vertical_line(int x, int y0, int y1, SDL_Color color) {
    SDL_Rect rect = {x,y0,1,y1-y0};
    uint32_t c = SDL_MapRGBA(buffer.first->format,color.r,color.g,color.b,color.a);
    SDL_FillRect(buffer.first,&rect,c);

}

void RenderWindow::drawShaded(const std::string &str, TTF_Font *font, int2 p, SDL_Color fg, SDL_Color bg, bool isCentered) {
    SDL_Surface* surface = TTF_RenderText_Shaded(font,str.c_str(),fg,bg);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect r; r.x = p.x; r.y = p.y; r.w = surface->w; r.h = surface->h;
    if(isCentered) {
        r.x -= r.w/2;
        r.y -= r.h/2;
    }
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &r);
    SDL_DestroyTexture(texture);
}

void RenderWindow::render(SDL_Surface* surface, SDL_Rect *srcRect, SDL_Rect *dstRect) {

    SDL_BlitSurface(surface,srcRect,buffer.first,dstRect);
//    SDL_BlitScaled(surface,srcRect,buffer.first,dstRect);
    SDL_FreeSurface(surface);
}

void RenderWindow::render(int x, int y, uint32_t color) {
    auto * const target_pixel = (Uint32 *) ((Uint8 *) buffer.first->pixels
                                            + y * buffer.first->pitch
                                            + x * buffer.first->format->BytesPerPixel);
    *target_pixel = color;
}

SDL_Renderer* RenderWindow::GetRenderer() {
    return renderer;
}

void RenderWindow::draw( Sprite *sprite, int x, int y, int frame,bool2 isCentered) {
    SDL_Rect dst = {x,y,sprite->GetWidth(),sprite->GetHeight()};
    SDL_Rect src = (*sprite)[frame];
    if(isCentered.x) dst.x -= dst.w/2;
    if(isCentered.y) dst.y -= dst.h/2;
    SDL_RenderCopy(renderer,sprite->SDL_Tex(),&src,&dst);

}






