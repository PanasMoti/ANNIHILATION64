//
// Created by juicylucy on 4/11/23.
//

#ifndef ANNIHILATION64_STATE_MAPEDIT_H
#define ANNIHILATION64_STATE_MAPEDIT_H
#include "StateManager.h"
#include "Map.h"
#include <sqlite3.h>

///@class a class for the StateType::MapEdit
class State_MapEdit : public BaseState{
public:
    /// constructor
    /// \param l_stateManager a pointer to a state manager
    State_MapEdit(StateManager* l_stateManager);

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
    /// bound to event function, used to send the map the player created to the database,upon successful insertion
    /// to the database this flips a boolean var that make the state no longer show the map but display the password it saved it in the database
    /// \param l_details unused
    void SaveMap(EventDetails* l_details);
    /// bound to event function, used to change the map size, either make it bigger by 1 or smaller by 1, maximum size is 32 and minimum is 16
    /// \param l_details used to decide if it should be enlarged or made smaller
    void ChaneMapSize(EventDetails* l_details);

    /// function that displays an error message that tells the player that they did something that makes the map not valid
    void MapIsntValid();
    /// function that displays an error message that tells the player they cannot change the border
    void NotAllowed();
    /// bound to event function, changes the enter_guard boolean to true,
    /// this is used to make sure that when the player switches to this state the first stop pressing the Enter button so it won't read it as input accidently
    /// \param l_details unused
    void flip(EventDetails* l_details);
private:
    Map map;
    TTF_Font* menu_font;
    std::string menu_text;
    TTF_Font* info_font;
    std::string info_text;
    int2 pointer;
    bool enter_guard;
    SDL_Color pointer_color;
    std::string index_text;
    sqlite3* DB;
    std::string custom_pass;
    bool written_to_db;
    bool map_is_valid;
    bool is_not_allowed;
};


static bool CompareSDLColors(SDL_Color c,SDL_Color c2) {
    return c.r == c2.r && c.g == c2.g && c.b == c2.b && c.a == c2.a;
}


#endif //ANNIHILATION64_STATE_MAPEDIT_H
