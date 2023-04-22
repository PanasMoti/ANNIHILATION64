//
// Created by juicylucy on 4/16/23.
//

#ifndef ANNIHILATION64_STATE_GAMEPLAY_H
#define ANNIHILATION64_STATE_GAMEPLAY_H
#include "StateManager.h"
#include <SDL2/SDL.h>
#include "header_onlys/GameData.h"
#include <unordered_map>
#include "gfx/Texture.h"
#include "gfx/GameTexture.h"
#include "gfx/Sprite.h"
#include "gfx/GameEntities.h"



class State_GamePlay : public BaseState{
    using keyboard = std::unordered_map<int,bool>;
public:
    State_GamePlay(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override;
    void Deactivate() override;
    void Update(float dt) override;
    void Draw() override;

    void KeyPressed(EventDetails* l_details);
    void KeyReleased(EventDetails* l_details);
    void DebugLoseHP(EventDetails* l_details);
    void DebugGainHP(EventDetails* l_details);
    void GameOver();
    void PlayerEntityCollision(int index);

    void EnemyAI(float dt);

private:

    void RenderBuffer();
    void RenderHud();
    keyboard keys_state;
    TTF_Font* hud_font;
    TTF_Font* info_font;
    std::string info_text;
    Texture* heart_img;
    Texture* bullet_img;
    GameTexture wallTexture;
    GameTexture doorTexture;
    Sprite* gunSprite;
    bool player_shot;

    std::vector<float> ZBuffer;
    bool gameOver;
    float2 hit_cooldown;

};


#endif //ANNIHILATION64_STATE_GAMEPLAY_H
