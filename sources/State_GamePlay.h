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


///@class a class for the StateType::GamePlay
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


    // these functions are used to combat input lag

    /// bound to event function, used to update the key_state of the keycode from the EventDetails* to true ( means the player is pressing that key )
    /// \param l_details used for the keycode
    void KeyPressed(EventDetails* l_details);
    /// bound to event function, used to update the key_state of the keycode from the EventDetails* to false ( means the player is no longer pressing that key )
    /// \param l_details used for the keycode
    void KeyReleased(EventDetails* l_details);

    /// bound to event function, *is not meant to be in the release of the game*, uses for quicker testing of gameplay
    /// decreases the player's health points
    /// \param l_details unused
    void DebugLoseHP(EventDetails* l_details);
    /// bound to event function, *is not meant to be in the release of the game*, uses for quicker testing of gameplay
    /// increases the player's health points
    /// \param l_details unused
    void DebugGainHP(EventDetails* l_details);

    /// when the gameOver variable is true this switches the current state to the GameOver state
    void GameOver();
    /// this process a collision between the player and an entity ,
    /// for example if the entity is an enemy, the player loses a health point
    /// note this function assumes there IS a collision and doesn't check for collision
    /// \param index the index of the entity the player is colliding with
    void PlayerEntityCollision(int index);

    /// this is what makes enemies move, currently it is simply if they see the player they walk towards them
    /// \param dt delta Time , time between frames in seconds
    void EnemyAI(float dt);

private:

    // quick explanation for the following functions: since the `3d` rendering code
    // write to a buffer on a cpu it is rendered differently than
    // the rest of stuff we show on the screen

    /// write to the buffer , scale it, render it to the renderer
    /// this is used to write pixels to the buffer aka calculating what the player sees and writing to a big array
    /// of pixels and only in the sending it to the gpu
    void RenderBuffer();
    /// write to the renderer on the GPU directly
    /// this is used to display `static` graphics that aren't relaying on the player's position , such as
    /// the hearts,the bullets,the gun, the text that explains the level
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
