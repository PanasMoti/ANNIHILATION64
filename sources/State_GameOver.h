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


class State_GameOver : public BaseState{
public:
    //constructor
    State_GameOver(StateManager* l_stateManager);

    // override functions from the base class
    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override;
    void Deactivate() override;
    void Update(float dt) override;
    void Draw() override;


    /// a callback function that just returns the player to the main menu screen
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
