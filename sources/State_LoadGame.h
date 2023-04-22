//
// Created by juicylucy on 4/10/23.
//

#ifndef ANNIHILATION64_STATE_LOADGAME_H
#define ANNIHILATION64_STATE_LOADGAME_H
#include "StateManager.h"
#include "import/linalg.h"
#include <sqlite3.h>



class State_LoadGame : public BaseState{
public:
    State_LoadGame(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override;
    void Deactivate() override;
    void Update(float dt) override;
    void Draw() override;

    void ReturnToMenu(EventDetails* l_details);
    void SelectUp(EventDetails* l_details);
    void SelectDown(EventDetails* l_details);
    void SelectLeft(EventDetails* l_details);
    void SelectRight(EventDetails* l_details);
    void PressSelected(EventDetails* l_details);
    void flip(EventDetails* l_details);
    void Backspace(EventDetails* l_details);
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