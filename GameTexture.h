//
// Created by juicylucy on 4/20/23.
//

#ifndef ANNIHILATION64_GAMETEXTURE_H
#define ANNIHILATION64_GAMETEXTURE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "picopng.h"

class GameTexture {

public:
    GameTexture();
    ~GameTexture();
    GameTexture(const char* fileName);
    void loadFromFile(const char* fileName);
    int Width() const; int Height() const;
    uint32_t& operator[](size_t i);
private:
    void NotFound();
    std::vector<uint32_t> pixels;
    int w;
    int h;
};


#endif //ANNIHILATION64_GAMETEXTURE_H
