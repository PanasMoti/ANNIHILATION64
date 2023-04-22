//
// Created by juicylucy on 4/10/23.
//

#ifndef ANNIHILATION64_STATE_LOADGAME_H
#define ANNIHILATION64_STATE_LOADGAME_H
#include "StateManager.h"
#include "import/linalg.h"
#include <sqlite3.h>


///@class a class for the StateType::LoadGame
class State_LoadGame : public BaseState{
public:
    /// constructor
    /// \param l_stateManager a pointer to a state manager
    State_LoadGame(StateManager* l_stateManager);

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

    /// bound to event that just returns the player to the main menu screen
    /// \param l_details unused
    void ReturnToMenu(EventDetails* l_details);

    /// bound to event function that moves the player's selection up , this loops if player tries to go higher than highest option
    /// \param l_details unused
    void SelectUp(EventDetails* l_details);
    /// bound to event function that moves the player's selection down , this loops if player tries to go lower than lowest option
    /// \param l_details unused
    void SelectDown(EventDetails* l_details);

    /// bound to event function that moves the player's selection left , this loops if player tries to go more left than most left option
    /// \param l_details unused
    void SelectLeft(EventDetails* l_details);
    /// bound to event function that moves the player's selection right , this loops if player tries to go more right than most right option
    /// \param l_details unused
    void SelectRight(EventDetails* l_details);

    /// bound to event function that add the character the player's selection is currently on to the string that holds the password the user is trying to write
    /// \param l_details unused
    void PressSelected(EventDetails* l_details);
    /// bound to event function, changes the enter_guard boolean to true,
    /// this is used to make sure that when the player switches to this state the first stop pressing the Enter button so it won't read it as input accidently
    /// \param l_details unused
    void flip(EventDetails* l_details);
    /// bound to event function, used to delete the last character from the player's input string, this doesn't work when the
    /// player's input string is empty
    /// \param l_details unused
    void Backspace(EventDetails* l_details);

    /// bound to event function, used to submit the input string and check if that password
    /// is found in the database, if it is this function switches the state to the gameplay state
    /// if it is not found in the database it flips a bool value that resposible for showing an error msg
    /// \param l_details unused
    void SubmitInput(EventDetails* l_details);

private:
    TTF_Font* menu_font;
    TTF_Font* user_font;
    TTF_Font* info_font;
    std::string menu_text;
    std::string keyboard_text;
    std::string info_text;
    int user_font_size;
    std::string password;
    int selectedX,selectedY;
    std::string letters;
    const int max_pass_len = 17;
    bool enter_guard;
    sqlite3* DB;
    bool IsFoundInDB;
    bool showErrorMsg;
};


#endif //ANNIHILATION64_STATE_LOADGAME_H
