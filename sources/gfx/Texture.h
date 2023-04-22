//
// Created by juicylucy on 4/9/23.
//

#ifndef ANNIHILATION64_TEXTURE_H
#define ANNIHILATION64_TEXTURE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../import/linalg.h"
#include <ostream>
using namespace linalg::aliases;


///@class an image used to later on be displayed on the screen
///@attention this is different from GameTexture, which is only used in the gameplay state
class Texture {
public:
    ///Constructor
    Texture();
    /// Constructor
    /// \param path a path to an image
    /// \param renderer a pointer to an SDL_Renderer* that will display the Texture
    Texture(const char* path,SDL_Renderer* renderer);
    /// a function to load the texture from an image in case the texture was declared and defined in different instances
    /// \param path the path to an image
    /// \param renderer a pointer to an SDL_Renderer* that will display the Texture
    void loadFromFile(const char* path,SDL_Renderer* renderer);
    /// Destructor, used to unload the texture and remove it from the gpu
    ~Texture();
    /// used like the Destructor but doesn't have to be called when the texture object is destroyed
    void Unload();
    /// Setter - sets the size the texture would be displays as
    /// \param w width of the texture
    /// \param h height of the texture
    void SetSize(int w,int h);
    /// Setter - sets the size the texture would be displays as
    /// \param size {width of the texture,height of the texture}
    void SetSize(int2 size);
    /// Getter
    /// \return the size of the texture as {width,height}
    int2 GetSize() const;
    /// Getter
    /// \return Width
    int GetWidth() const;
    /// Setter
    /// \param w width
    void SetWidth(int w);
    /// Getter
    /// \return height
    int GetHeight() const;
    /// Setter
    /// \param h height
    void SetHeight(int h);
    /// Getter
    /// \return the actual dimensions of image
    int2 GetActualSize() const;
    /// Getter
    /// \return the actual width of the image
    int GetActualWidth() const;
    /// Getter
    /// \return the actual height of the image
    int GetActualHeight() const;
    /// Getter
    /// \return The pointer to the SDL_Texture
    SDL_Texture* SDL_Tex() const;
    /// overloading so we can print Texture Object with cout
    /// \param os std::ostream& the output stream
    /// \param texture const reference to a texture
    /// \return the os
    friend std::ostream& operator<<(std::ostream& os,const Texture& texture);
    /// scales the image up by p
    /// \param p multiplier to scale by
    void Scale(float p);
    /// scales the width by x and scales the height by y
    /// \param x multiplier to scale width by
    /// \param y multiplier to scale height by
    void Scale(float x,float y);
    /// Convert the SDL_Texture* to an SDL_Surface*
    /// \return an SDL_Surface* that was made from the pixels of the texture
    SDL_Surface* ToSurface() const;

protected:
    SDL_Texture* pixels{};
    int2 size;
    int2 actual_size;
};


#endif //ANNIHILATION64_TEXTURE_H
