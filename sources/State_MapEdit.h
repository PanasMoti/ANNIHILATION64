//
// Created by juicylucy on 4/11/23.
//

#ifndef ANNIHILATION64_STATE_MAPEDIT_H
#define ANNIHILATION64_STATE_MAPEDIT_H
#include "StateManager.h"
#include "Map.h"
#include <sqlite3.h>
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

    void SelectUp(EventDetails* l_details);
    void SelectDown(EventDetails* l_details);
    void SelectLeft(EventDetails* l_details);
    void SelectRight(EventDetails* l_details);
    void PressSelected(EventDetails* l_details);
    void SaveMap(EventDetails* l_details);
    void ChaneMapSize(EventDetails* l_details);
    void MapIsntValid();
    void NotAllowed();

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
