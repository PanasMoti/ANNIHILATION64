//
// Created by juicylucy on 4/20/23.
//

#ifndef ANNIHILATION64_COLORS_H
#define ANNIHILATION64_COLORS_H
#include <SDL2/SDL.h>




//! convert a Hex code of a color to rgba(x,x,x,x)
//! \param hex unsigned 32 bit hex code of a color in the format of RGBA
//! \example 0xFF00FFFF -> rgba(255,0,255,255)
//! \return SDL_Color with the rgba corresponding to the hex code
static SDL_Color ToColor(uint32_t hex) {
    return SDL_Color {
            static_cast<uint8_t>((hex >> 24) & 0xFF),
            static_cast<uint8_t>((hex >> 16) & 0xFF),
            static_cast<uint8_t>((hex >> 8) & 0xFF),
            static_cast<uint8_t>((hex >> 0) & 0xFF)
    };
}


/* color constants for easier use */
/* -------------------------------*/

static const SDL_Color clWHITE = {255,255,255,255};
static const SDL_Color clBLACK = {0,0,0,255};
static const SDL_Color clRED = {255,0,0,255};
static const SDL_Color clGREEN = {0,255,0,255};
static const SDL_Color clBLUE = {0,0,255,255};
static const SDL_Color clYELLOW = {255,255,0,255};
static const SDL_Color clMAGENTA = {255,0,255,255};
static const SDL_Color clCYAN = {0,255,255,255};
static const SDL_Color clORANGE = {255,165,0,255};
static const SDL_Color clPURPLE = {128,0,128,255};
static const SDL_Color clVIOLET = {238,130,238,255};
static const SDL_Color clLIME = {50,205,50};
static const SDL_Color clGRAY = {128,128,128,255};
static const SDL_Color clDARKCYAN = {	0, 139, 139,255};

/* -------------------------------*/



#endif //ANNIHILATION64_COLORS_H
