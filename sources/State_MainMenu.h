//
// Created by juicylucy on 4/9/23.
//

#ifndef ANNIHILATION64_STATE_MAINMENU_H
#define ANNIHILATION64_STATE_MAINMENU_H
#include "StateManager.h"
#include <SDL2/SDL_ttf.h>

class State_MainMenu : public BaseState{
public:
    /// constructor
    /// \param l_stateManager a pointer to a state manager
    State_MainMenu(StateManager* l_stateManager);


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

    /// bound to event function that sends the player to a different state , based on what they have selected
    /// \param l_details unused
    void Continue(EventDetails* l_details);
    /// bound to event function that moves the player's selection up , this loops if player tries to go higher than highest option
    /// \param l_details unused
    void SelectUp(EventDetails* l_details);
    /// bound to event function that moves the player's selection down , this loops if player tries to go lower than lowest option
    /// \param l_details unused
    void SelectDown(EventDetails* l_details);
private:
    TTF_Font* menu_font;
    TTF_Font* info_font;
    Texture* menu_logo;
    std::string options[3];
    int selected;
    SDL_Color selected_color,default_color;
    std::string info;
};


#endif //ANNIHILATION64_STATE_MAINMENU_H
