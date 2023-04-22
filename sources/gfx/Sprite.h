//
// Created by juicylucy on 4/20/23.
//

#ifndef ANNIHILATION64_SPRITE_H
#define ANNIHILATION64_SPRITE_H
#include "Texture.h"

///@class an animated texture derived from the Texture class
class Sprite : public Texture{
public:
    /// constructor
    Sprite();
    /// destructor
    ~Sprite();
    /// constructor
    /// \param img path to the image
    /// \param json path to a json file that defines the frames
    /// \param renderer the renderer to load on
    Sprite(const char* img,const char* json,SDL_Renderer* renderer);
    /// constructor
    /// \param dirPath a path to a directory that has both a json file and a png file with the same name
    /// \param renderer the renderer to load on
    Sprite(const char* dirPath,SDL_Renderer* renderer);
    /// operator [] overloading that lets you access the frames rectangle like an array
    /// \param index the index of the frame
    /// \return the SDL_Rect of th frame
    SDL_Rect operator[](size_t index);
    /// Getter - Get the amount of frames
    /// \return frames.size()
    int NumOfFrames() const;
    /// change player.second = 0, change player.first to true
    void StartPlaying();
    /// change player.second = 0, change player.first to false
    void StopPlaying();
    /// update the current frame if enough time has passed
    /// \param dt delta Time
    void Update(float dt);
    /// Getter
    /// \return player.first
    bool isPlaying();
    /// Getter
    /// \return player.second
    int GetCurrentFrameIndex() const;
    /// @property the time between each frame
    float time_between_frames;
    /// Setter
    /// \param fps frames per second
    void SetFPS(int fps);
    /// changes is_looping to NOT is_looping
    void ToggleLoop();
    /// changes player.second = 0
    void resetPlayer();
protected:
    void Init(const char* img,const char* json,SDL_Renderer* renderer);
    std::vector<SDL_Rect> frames;
    std::pair<bool,int> player;
    float temp_time;
    bool is_looping;
};


#endif //ANNIHILATION64_SPRITE_H
