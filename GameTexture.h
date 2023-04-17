//
// Created by elias on 17/04/2023.
//

#ifndef ANNIHILATION64_GAMETEXTURE_H
#define ANNIHILATION64_GAMETEXTURE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "linalg.h"



class GameTexture {
public:
    GameTexture();
    ~GameTexture();
    GameTexture(const char* fileName);
    GameTexture(const GameTexture& other);

    int Width() const; int Height() const;
    SDL_Surface* GetSurface();

private:
    void NotFoundTexture();
    SDL_Surface* surface;
    int w;
    int h;
};


#endif //ANNIHILATION64_GAMETEXTURE_H
