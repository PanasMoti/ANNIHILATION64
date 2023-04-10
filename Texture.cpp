//
// Created by juicylucy on 4/9/23.
//

#include "Texture.h"

Texture::Texture() {
    this->pixels = nullptr;
    this->size = {0,0};
    this->actual_size = {0,0};
}
Texture::~Texture() {
    SDL_DestroyTexture(this->pixels);
    this->pixels = nullptr;
}

void Texture::loadFromFile(const char *path, SDL_Renderer *renderer) {
    this->pixels = IMG_LoadTexture(renderer,path);
    SDL_QueryTexture(this->pixels, nullptr, nullptr,&this->actual_size.x,&this->actual_size.y);
    this->size = this->actual_size;
}

void Texture::SetSize(int w, int h) {
    this->size = {w,h};
}

void Texture::SetSize(int2 size) {
    this->size = size;
}

int2 Texture::GetSize() const {
    return this->size;
}

int Texture::GetWidth() const {
    return this->size.x;
}

void Texture::SetWidth(int w) {
    this->size.x = w;
}

int Texture::GetHeight() const {
    return this->size.y;
}

void Texture::SetHeight(int h) {
    this->size.y = h;
}

int2 Texture::GetActualSize() const {
    return this->actual_size;
}

int Texture::GetActualWidth() const {
    return this->actual_size.x;
}

int Texture::GetActualHeight() const {
    return this->actual_size.y;
}

SDL_Texture *Texture::SDL_Tex() const {
    return this->pixels;
}

void Texture::Unload() {
    SDL_DestroyTexture(this->pixels);
    this->pixels = nullptr;
}

Texture::Texture(const char *path, SDL_Renderer *renderer) {
    this->loadFromFile(path,renderer);
}

std::ostream &operator<<(std::ostream &os, const Texture &texture) {
    return os << texture.size.x << "," << texture.size.y ;
}

void Texture::Scale(float p) {
    this->size.x*=p;
    this->size.y*=p;

}

void Texture::Scale(float x, float y) {
    this->size.x*=x;
    this->size.y*=y;
}


