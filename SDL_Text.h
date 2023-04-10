#pragma once
#include <SDL2/SDL_ttf.h>
#include <string>
#include "linalg.h"
using namespace linalg::aliases;

struct SDL_Text {
	std::string text;
	TTF_Font** font;
	SDL_Color color{};
    SDL_Text() {
        font = nullptr;
        text = "";
    }
	explicit SDL_Text(TTF_Font** font);
	SDL_Text(TTF_Font** font, const std::string& text);
	SDL_Text(TTF_Font** font, const std::string& text,SDL_Color color);
    static int2 GetSize(const SDL_Text& text);

};

