//
// Created by juicylucy on 4/9/23.
//

#include "State_MainMenu.h"
#include <iostream>

void State_MainMenu::OnCreate() {
    time_passed = 0.0f;
    selected = 0;
    selected_color = SDL_Color {255,255,0,255};
    default_color = SDL_Color {255,64,64,255};
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Doom2016.ttf",160);
    info_font = m_stateMgr->GetContext()->window->loadFont("assets/Emulogic.ttf", 32);
    text[0] = SDL_Text(&menu_font,"new game",default_color);
    text[1] = SDL_Text(&menu_font,"load game",default_color);
    text[2] = SDL_Text(&menu_font,"custom game", default_color);
    info = SDL_Text(&info_font, "press [ESC] to quit", SDL_Color{255, 255, 255, 255});
    menu_logo = new Texture(m_stateMgr->GetContext()->window->loadTexture("assets/logo.png"));
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::MainMenu,"MainMenu_Continue",&State_MainMenu::Continue,this);
    evMgr->AddCallback(StateType::MainMenu,"Up_Arrow",&State_MainMenu::SelectUp,this);
    evMgr->AddCallback(StateType::MainMenu,"Down_Arrow",&State_MainMenu::SelectDown,this);
    this->menu_logo->Scale(2.2f);
    menu_background = new Texture(m_stateMgr->GetContext()->window->loadTexture("assets/background.png"));
}

void State_MainMenu::OnDestroy() {
    TTF_CloseFont(menu_font);
    TTF_CloseFont(info_font);
    menu_logo->Unload(); menu_background->Unload();
    delete menu_logo; delete menu_background;
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::MainMenu,"MainMenu_Continue");
    evMgr->RemoveCallback(StateType::MainMenu,"Up_Arrow");
    evMgr->RemoveCallback(StateType::MainMenu,"Down_Arrow");
}

void State_MainMenu::Update(float dt) {
    time_passed += dt;
    for(int i = 0; i < 3; i++) {
        if(i == selected) {
            text[i].color = selected_color;
        } else {
            text[i].color = default_color;
        }
    }
}
void State_MainMenu::Draw() {
    RenderWindow* window = m_stateMgr->GetContext()->window;
    int2 p = window->ScreenCenter();
//    window->draw(*menu_background);
    window->draw(*menu_logo,p.x - menu_logo->GetWidth()/2,-10);
    for(int i = 0; i < 3; i++) {
        window->draw(text[i],p.x,p.y-150*(3-i) + 300);
    }
    window->draw(info,p.x,2*p.y-100);
    window->draw_guidlines({255,64,255,255});
}

void State_MainMenu::Continue(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::GamePlay);
    switch (selected) {
        case 0:
            m_stateMgr->SwitchTo(StateType::GamePlay);
            break;
        case 1:
            m_stateMgr->SwitchTo(StateType::LoadGame);
            break;
        case 2:
            m_stateMgr->SwitchTo(StateType::MapEdit);
            break;
        default:
            std::cout << "something went horribly wrong!" << std::endl;
            break;
    }
}

State_MainMenu::State_MainMenu(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_MainMenu::SelectUp(EventDetails *l_details) {
    selected--;
    if(selected < 0) selected = 2;
}

void State_MainMenu::SelectDown(EventDetails *l_details) {
    selected++;
    if(selected > 2) selected = 0;

}

