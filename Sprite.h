//
// Created by juicylucy on 4/20/23.
//

#ifndef ANNIHILATION64_SPRITE_H
#define ANNIHILATION64_SPRITE_H
#include "Texture.h"

class Sprite : public Texture{
public:
    Sprite();
    ~Sprite();
    Sprite(const char* img,const char* json,SDL_Renderer* renderer);
    Sprite(const char* dirPath,SDL_Renderer* renderer);
    SDL_Rect operator[](size_t index);
    int NumOfFrames() const;
    void StartPlaying();
    void StopPlaying();
    void Update(float dt);
    bool isPlaying();
    int GetCurrentFrameIndex() const;
    float time_between_frames;
protected:
    void Init(const char* img,const char* json,SDL_Renderer* renderer);
    std::vector<SDL_Rect> frames;
    std::pair<bool,int> player;
    float temp_time;
};


#endif //ANNIHILATION64_SPRITE_H
