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
    /// constructor
    /// \param l_stateManager a pointer to a state manager
    State_GamePlay(StateManager* l_stateManager);

    /// override method ,used when we create the state ( the begining of the game ) used to load assets and initalize variables
    void OnCreate() override;
    /// override method ,used when we close the state ( the end of the game ) used to unload assets and delete variables that live on the heap
    void OnDestroy() override;
    /// override method ,used when we switch to this state used to reset variables to their default states and open files if needed
    void Activate() override;
    /// override method ,used when we switch to a different state from this state, used to reset variables and close files if they're open
    void Deactivate() override;
    /// override method ,used every frame, called to update variables for example player position, hp , enemy positions etc...
    /// \param dt delta Time ( the time between frames in seconds ) , this used to have the game run in the same speed no matter the frame rate
    void Update(float dt) override;
    /// override method, used every frame, this function passes what needs to be showed on the screen to the RenderWindow Object that handles all of the displaying logic
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
    GameTexture floorTexture;
    Sprite* gunSprite;
    bool player_shot;

    std::vector<float> ZBuffer;
    bool gameOver;
    float2 hit_cooldown;

};


#endif //ANNIHILATION64_STATE_GAMEPLAY_H
