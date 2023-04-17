//
// Created by elias on 17/04/2023.
//

#include "GameTexture.h"

GameTexture::GameTexture() {
    NotFoundTexture();
}

GameTexture::~GameTexture() {
    SDL_FreeSurface(surface);
    surface = nullptr;
}

GameTexture::GameTexture(const char *fileName) {
    surface = IMG_Load(fileName);
}

int GameTexture::Width() const {
    return 0;
}

int GameTexture::Height() const {
    return 0;
}

SDL_Surface *GameTexture::GetSurface() {
    return nullptr;
}

void GameTexture::NotFoundTexture() {
    // generating a not found texture
    w = 16; h = 16;
    uint32_t* pixels = (uint32_t*)malloc(sizeof(uint32_t)*w*h);
    for(int i = 0; i < w; i ++) {
        for(int j = 0; j < h; j++) {
            pixels[i + j*w] = ((i+j)%2 == 0) ? 0xFF00FFFF : 0x00000000;
        }
    }
    surface = SDL_CreateRGBSurfaceFrom((void*)pixels,w,h,32,4*h,0,0,0,0);

}

GameTexture::GameTexture(const GameTexture &other) {
    *this = other;
}




