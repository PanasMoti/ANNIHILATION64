//
// Created by juicylucy on 4/11/23.
//

#ifndef ANNIHILATION64_STATE_MAPEDIT_H
#define ANNIHILATION64_STATE_MAPEDIT_H
#include "StateManager.h"
#include "Map.h"
#include "SDL_Text.h"

class State_MapEdit : public BaseState{
public:
    State_MapEdit(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override {}
    void Deactivate() override {}
    void Update(float dt) override;
    void Draw() override;
    void ReturnToMenu(EventDetails* l_details);
    void SelectUp(EventDetails* l_details);
    void SelectDown(EventDetails* l_details);
    void SelectLeft(EventDetails* l_details);
    void SelectRight(EventDetails* l_details);
    void PressSelected(EventDetails* l_details);
    void flip(EventDetails* l_details);
private:
    Map* map;
    TTF_Font* menu_font;
    SDL_Text menu_text;
    TTF_Font* info_font;
    SDL_Text info_text;
    int2 pointer;
    bool enter_guard;
    SDL_Color pointer_color;
    SDL_Text info2_text;
};


static bool CompareSDLColors(SDL_Color c,SDL_Color c2) {
    return c.r == c2.r && c.g == c2.g && c.b == c2.b && c.a == c2.a;
}


#endif //ANNIHILATION64_STATE_MAPEDIT_H
