//
// Created by juicylucy on 4/9/23.
//

#ifndef ANNIHILATION64_STATE_MAINMENU_H
#define ANNIHILATION64_STATE_MAINMENU_H
#include "StateManager.h"
#include <SDL2/SDL_ttf.h>

class State_MainMenu : public BaseState{
public:
    State_MainMenu(StateManager* l_stateManager);


    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override {}
    void Deactivate() override {}
    void Update(float dt) override;
    void Draw() override;
    void Continue(EventDetails* l_details);
    void SelectUp(EventDetails* l_details);
    void SelectDown(EventDetails* l_details);
private:
    TTF_Font* menu_font;
    TTF_Font* info_font;
    Texture* menu_logo;
    Texture* menu_background;
    std::string options[3];
    int selected;
    SDL_Color selected_color,default_color;
    std::string info;
};


#endif //ANNIHILATION64_STATE_MAINMENU_H
