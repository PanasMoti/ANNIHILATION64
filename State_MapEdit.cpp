//
// Created by juicylucy on 4/11/23.
//

#include "State_MapEdit.h"

State_MapEdit::State_MapEdit(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_MapEdit::OnCreate() {
    time_passed = 0.0f;
    this->map = new Map(32,32);
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::MapEdit,"Return_To_Menu",&State_MapEdit::ReturnToMenu,this);
}

void State_MapEdit::OnDestroy() {
    delete map;
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::MapEdit,"Return_To_Menu");
}

void State_MapEdit::Update(float dt) {
    time_passed+=dt;
}

void State_MapEdit::Draw() {
    int2 size = {640,640};
    int2 o = {0,0};
    RenderWindow* window = m_stateMgr->GetContext()->window;
    auto center = window->ScreenCenter();
    o = center - size/2;
    int c = map->GetWidth();
    int r = map->GetHeight();
    int resx = size.x/c;
    int resy = size.y/r;
    for(int i = 0; i < c;i ++) {
        for(int j = 0; j < r; j++) {
            auto cell = (*map)(i,j);
            auto color = ToColor(cell);
            auto rect = SDL_Rect {
                o.x + i*resx,o.y + j*resy, resx-1,resy-1
            };
            window->draw(rect,color,true);
        }
    }
    window->draw_guidlines({255,64,255,255});
}

void State_MapEdit::ReturnToMenu(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::MainMenu);
}
