#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "import/linalg.h"
#include "gfx/Texture.h"
#include "header_onlys/Colors.h"
#include "gfx/Sprite.h"
using namespace linalg::aliases;


///@class Since SDL is a C library, we needs to keep a pointer of SDL_Window* and SDL_Renderer* to pass everytime to we
/// want to draw to the screen, this class combines them both and gives us an easier way to draw to the screen

class RenderWindow
{
public:
    /// Constructor
    /// \param title the title of the window
    /// \param width the width of the window
    /// \param height the height of the window
	RenderWindow(const char* title, int width, int height);
    /// load an SDL_Texture from a file
    /// \param fileName the path to the image
    /// \return SDL_Texture pointer
	SDL_Texture* loadSDL_Texture(const char* fileName);
    /// load a font from a file
    /// \param fileName the path to the font
    /// \param size the size of the font
    /// \return TTF_Font pointer
	TTF_Font* loadFont(const char* fileName, int size = 32);
    /// load a Texture from a file
    /// \param fileName path to the image
    /// \return Texture
    Texture loadTexture(const char* fileName);
    /// close the window
    void close();
    /// destructor
    ~RenderWindow();
    /// clear the renderer
	void clear_screen();
    /// draw SDL_Texture on the entire screen
    /// \param texture pointer to an SDL_Texture
	void draw(SDL_Texture* texture);
    /// draw Texture
    /// \param texture constant reference to a Texture
    /// \param x x coordinate on screen
    /// \param y y coordinate on screen
    /// \param isCentered should we draw it centered to (x,y)
    void draw(const Texture& texture,int x,int y,bool2 isCentered = {false,false});
    /// draw Texture
    /// \param texture constant reference to a Texture
    /// \param pos {x,y}
    /// \param isCentered should we draw it centered to (x,y)
    void draw(const Texture& texture,int2 pos,bool2 isCentered = {false,false});
    /// draw a texture
    /// \param texture const reference to a Texture
    /// \param dst SDL_Rect that will be displayed on
    void draw(const Texture& texture,SDL_Rect dst);
    /// draw texture on the whole screen
    /// \param texture const reference to a Texture
    void draw(const Texture& texture);
    /// draw text on the screen
    /// \param txt string of the text
    /// \param font pointer to the font
    /// \param x x coord
    /// \param y y coord
    /// \param color color of the text
    /// \param isCentered should we draw it centered to (x,y)
    void draw(const std::string& txt,TTF_Font* font,int x,int y, SDL_Color color=clWHITE,bool isCentered = false);
    /// draw text on the screen
    /// \param txt string of the text
    /// \param font pointer to the font
    /// \param pos {x,y}
    /// \param color color of the text
    /// \param isCentered should we draw it centered to (x,y)
    void draw(const std::string& txt,TTF_Font* font,int2 pos,SDL_Color color=clWHITE,bool isCentered = false);
    /// draw text wrapped on the screen
    /// \param txt string of the text
    /// \param font pointer to the font
    /// \param pos {x,y}
    /// \param wrappedlen maximum length perline
    /// \param color color of the text
    /// \param isCentered should we draw it centered to (x,y)
    /// \param rect pointer to a rect, used later to position things correctly
    void draw(const std::string& txt,TTF_Font* font,int2 pos,Uint32 wrappedlen,SDL_Color color=clWHITE,bool isCentered = false,SDL_Rect* rect = nullptr);
    /// draw text wrapped on the screen
    /// \param txt string of the text
    /// \param font pointer to the font
    /// \param pos {x,y}
    /// \param wrappedlen maximum length perline
    /// \param color color of the text
    /// \param isCentered should we draw it centered to (x,y)
    /// \param rect pointer to a rect, used later to position things correctly
    void draw(const std::string& txt,TTF_Font* font,int x,int y,Uint32 wrappedlen,SDL_Color color=clWHITE,bool isCentered = false,SDL_Rect* rect = nullptr);
    /// draw a rectangle of a single solid color on the screen
    /// \param sdlRect the rectangle
    /// \param color the color
    /// \param filled is it filled or not
    void draw(SDL_Rect sdlRect,SDL_Color color,bool filled = false);
    /// draw Sprite on the screen
    /// \param sprite the sprite
    /// \param x x coord
    /// \param y y coord
    /// \param frame the frame of the animation
    /// \param isCentered hould we draw it centered to (x,y)
    void draw(Sprite* sprite,int x,int y,int frame,bool2 isCentered = {false,false});
    /// display everything that was written to the renderer and or buffer
	void display();
    /// Getter
    /// \return the position of the mouse related to the window
	int2 GetMousePosition() const;
    /// Getter
    /// \return the center of the window
    int2 ScreenCenter() const;
    /// Getter
    /// \return the size of the window
    int2 GetScreenSize() const;
    /// destroy the SDL_Window* and SDL_Renderer*
    void destroy();
    /// convert the buffer from an SDL_Surface* (CPU) to SDL_Texture* (GPU) and copy to renderer
    void update();
    /// clear the buffer - set all pixels in it to 0 (black)
    void clear_buffer() const;
    /// create the buffer -> important because we do not create the buffer until gameData loads
    void create_buffer();
    /// the name is misleading, in actually it writes a pixel at x,y to the buffer.first
    /// \param x x coord in the buffer
    /// \param y y coord in the buffer
    /// \param color the color to set pixel as
    void render(int x,int y,SDL_Color color);
    /// the name is misleading, in actually it writes a pixel at x,y to the buffer.first
    /// \param x x coord in the buffer
    /// \param y y coord in the buffer
    /// \param color the value to set the pixel to
    void render(int x,int y,uint32_t color);
    /// Getter
    /// \return the renderer
    SDL_Renderer* GetRenderer();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
    std::pair<SDL_Surface*,SDL_Texture*> buffer;

};

