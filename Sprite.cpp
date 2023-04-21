//
// Created by juicylucy on 4/20/23.
//

#include "Sprite.h"
#include "fileutil.h"
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Sprite::Sprite() : Texture() {}

Sprite::~Sprite() {
    frames.clear();
}

Sprite::Sprite(const char *img, const char *json,SDL_Renderer* renderer){
    Init(img,json,renderer);
}

Sprite::Sprite(const char *dirPath,SDL_Renderer* renderer) {
    auto find_str = [](std::string s,std::string del) {
        int end = s.find(del);
        std::vector<std::string> vec;
        while(end != -1) {
            // Loop until no delimiter is left in the string
            vec.push_back(s.substr(0,end));
            s.erase(s.begin(),s.begin()+end+1);
            end = s.find(del);
        }
        vec.push_back(s.substr(0,end));
        return vec;
    };

    std::string name = find_str(dirPath,"/").back();
    std::string base_path = std::string(dirPath) + "/" + name;
    Init(std::string(base_path+".png").c_str(),std::string(base_path+".json").c_str(),renderer);
}

void Sprite::Init(const char *img, const char *jsonFile, SDL_Renderer *renderer) {
    std::ifstream f(jsonFile);
    json data = json::parse(f);
    auto jframes = data["frames"];
    for(auto F : jframes) {
        frames.push_back(SDL_Rect {F["frame"]["x"],F["frame"]["y"],F["frame"]["w"],F["frame"]["h"]});
    }
    this->loadFromFile(img,renderer);
    player.second = 0; player.first = false;
}

SDL_Rect Sprite::operator[](size_t index) {
    return frames.at(index);
}

int Sprite::NumOfFrames() const {
    return frames.size();
}

void Sprite::StartPlaying() {
    if(player.first) return;
    player.first = true;
    player.second = 0;
}

void Sprite::StopPlaying() {
    player.first = is_looping;
    resetPlayer();
}

void Sprite::Update(float dt) {
    if(player.first) {
        temp_time += dt;
        if(temp_time >= time_between_frames) {
            if(++player.second >= frames.size()) {
                StopPlaying();
            }
            temp_time = 0;
        }
    }
}

bool Sprite::isPlaying() {
    return player.first;
}

int Sprite::GetCurrentFrameIndex() const {
    return player.second;
}

void Sprite::SetFPS(int fps) {
    float s = static_cast<float>(fps);
    float p = static_cast<float>(NumOfFrames());
    time_between_frames = p/s;
}

void Sprite::ToggleLoop() {
    is_looping = !is_looping;
}

void Sprite::resetPlayer() {
    player.second = 0;
}

