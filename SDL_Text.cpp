#include "SDL_Text.h"


SDL_Text::SDL_Text(TTF_Font** font) {
	this->font = font;
	this->text = "";
	this->color = SDL_Color{ 255,255,255,255 };
}

SDL_Text::SDL_Text(TTF_Font** font, const std::string& text)
{
	this->font = font;
	this->text = text;
	this->color = SDL_Color{ 255,255,255,255 };

}

SDL_Text::SDL_Text(TTF_Font** font, const std::string& text, SDL_Color color)
{
	this->font = font;
	this->text = text;
	this->color = color;
}
