//
// Created by juicylucy on 4/20/23.
//

#ifndef ANNIHILATION64_GAMETEXTURE_H
#define ANNIHILATION64_GAMETEXTURE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "../import/picopng.h"


///@class a texture in the form of a vector of pixels
/// this is used when wanting to render a texture to the buffer , where pixel accessing is curciel
/// if the texture isn't meant to be rendered in the buffer DO NOT USE THIS, USE THE TEXTURE CLASS INSTEAD
/// entities use this class

class GameTexture {
public:
    ///constructor
    GameTexture();
    //destructor
    ~GameTexture();
    /// constructor
    /// \param fileName path to image
    GameTexture(const char* fileName);
    /// load from file
    /// \param fileName path to image
    void loadFromFile(const char* fileName);
    /// Getter
    /// \return the width of the Texture
    int Width() const;
    /// Getter
    /// \return the width of the Texture
    int Height() const;
    /// operator [] overloading, giving access to the pixels in an array fashion
    /// \param i the index of the pixel
    /// \return a reference to the wanted pixel
    uint32_t& operator[](size_t i);
private:
    /// if a bad file was passed to the constructor or wasn't passed at all, this generates a 16x16 checkerboard texture
    /// this way the program doesn't crash if the image is not found
    void NotFound();
    std::vector<uint32_t> pixels;
    int w;
    int h;
};


#endif //ANNIHILATION64_GAMETEXTURE_H
