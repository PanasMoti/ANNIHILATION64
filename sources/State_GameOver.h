//
// Created by juicylucy on 4/21/23.
//

#ifndef ANNIHILATION64_STATE_GAMEOVER_H
#define ANNIHILATION64_STATE_GAMEOVER_H
#include "StateManager.h"
#include "gfx/Texture.h"
#include "gfx/Sprite.h"
#include "RenderWindow.h"

///@brief a Game Over state that is Derived from the BaseState class
/*
 * this state has a simple job :
 * 1.) show the "GAME OVER" image
 * 2.) show the animated skull
 * 3.) show the "press Z to return to menu...." text
 * 4.) show the player their score
 * 5.) show the player if they Won or lost
 * 6.) if the player chooses to go back to the mainmenu then switch the current state to the menu state
 * */

///@class a class for the StateType::GameOver
class State_GameOver : public BaseState{
public:
    /// constructor
    /// \param l_stateManager a pointer to a state manager
    State_GameOver(StateManager* l_stateManager);

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


    /// bound to event function that just returns the player to the main menu screen
    /// \param l_details unused
    void ReturnToMenu(EventDetails* l_details);

private:
    Sprite* skull;
    Texture* you_died_img;
    TTF_Font* info_font;
    std::string info_text;
    bool flicker;
    TTF_Font* hud_font;
    TTF_Font* score_font;

};


#endif //ANNIHILATION64_STATE_GAMEOVER_H
