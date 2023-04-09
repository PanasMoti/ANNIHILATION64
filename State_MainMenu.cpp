//
// Created by juicylucy on 4/9/23.
//

#include "State_MainMenu.h"
#include <iostream>

void State_MainMenu::OnCreate() {
    time_passed = 0.0f;
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Emulogic.ttf",32);
    text = SDL_Text(&menu_font,"Menu!",SDL_Color{255,255,0,255});
    menu_logo = new Texture(m_stateMgr->GetContext()->window->loadTexture("assets/logo.png"));
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::MainMenu,"MainMenu_Continue",&State_MainMenu::Continue,this);
    this->menu_logo->Scale(2.2f);
    menu_background = new Texture(m_stateMgr->GetContext()->window->loadTexture("assets/background.png"));
}

void State_MainMenu::OnDestroy() {
    TTF_CloseFont(menu_font);
    menu_logo->Unload(); menu_background->Unload();
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::MainMenu,"MainMenu_Continue");
}

void State_MainMenu::Update(float dt) {
    time_passed += dt;
}
void State_MainMenu::Draw() {
    RenderWindow* window = m_stateMgr->GetContext()->window;
    int2 p = window->ScreenCenter();
    window->draw(*menu_background,0,0);
    window->draw(*menu_logo,p.x - menu_logo->GetWidth()/2,-10);
    window->draw(text,p.x,p.y);
}

void State_MainMenu::Continue(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::GamePlay);
}

State_MainMenu::State_MainMenu(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

