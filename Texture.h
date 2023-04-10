//
// Created by juicylucy on 4/9/23.
//

#ifndef ANNIHILATION64_TEXTURE_H
#define ANNIHILATION64_TEXTURE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "linalg.h"
#include <ostream>
using namespace linalg::aliases;

class Texture {
public:
    Texture();
    Texture(const char* path,SDL_Renderer* renderer);
    void loadFromFile(const char* path,SDL_Renderer* renderer);
    ~Texture();
    void Unload();
    void SetSize(int w,int h);
    void SetSize(int2 size);
    int2 GetSize() const;
    int GetWidth() const;   void SetWidth(int w);
    int GetHeight() const;  void SetHeight(int h);
    int2 GetActualSize() const;
    int GetActualWidth() const;
    int GetActualHeight() const;
    SDL_Texture* SDL_Tex() const;
    friend std::ostream& operator<<(std::ostream& os,const Texture& texture);
    void Scale(float p);
    void Scale(float x,float y);
private:
    SDL_Texture* pixels{};
    int2 size;
    int2 actual_size;
};


#endif //ANNIHILATION64_TEXTURE_H
