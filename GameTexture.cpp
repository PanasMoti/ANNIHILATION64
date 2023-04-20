//
// Created by juicylucy on 4/20/23.
//

#include "GameTexture.h"

GameTexture::GameTexture() {
    this->NotFound();

}

GameTexture::~GameTexture() {
    this->pixels.clear();
}

GameTexture::GameTexture(const char *fileName) {
    unsigned long tw,th,error = 0;
    error = loadImage(pixels,tw,th,fileName);
    if(error ) {
        std::cerr << "couldnt load the image:" << fileName << "\t resulting to default texture"<< std::endl;
        NotFound();
    } else {
        w = tw;
        h = th;
    }
}

void GameTexture::loadFromFile(const char *fileName) {
    unsigned long tw,th,error = 0;
    std::vector<uint32_t>buffer;
    error = loadImage(buffer,tw,th,fileName);
    if(error ) {
        std::cerr << "couldnt load the image:" << fileName << "\t not changing the current texture" << std::endl;
    } else {
        pixels = buffer;
        w = tw;
        h = th;
    }
}



int GameTexture::Width() const {
    return w;
}

int GameTexture::Height() const {
    return h;
}

void GameTexture::NotFound() {
    w = 16; h = 16;
    pixels.resize(w*h);
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            pixels[x + y*w] = ((x+y)%2 == 0) ? 0xFF00FF : 0x000000;
        }
    }
}

uint32_t &GameTexture::operator[](size_t i) {
    return pixels[i];
}
