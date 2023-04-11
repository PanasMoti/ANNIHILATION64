//
// Created by juicylucy on 4/11/23.
//

#include "State_MapEdit.h"
#include <iostream>
State_MapEdit::State_MapEdit(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}


void State_MapEdit::OnCreate() {
    enter_guard = false;
    time_passed = 0.0f;
    pointer = {0,0};
    pointer_color = {255,255,0,255};
    this->map = new Map(16,16);
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Doom2016.ttf",160);
    info_font = m_stateMgr->GetContext()->window->loadFont("assets/YosterIsland.ttf",32);
    menu_text = SDL_Text(&menu_font,"edit your own level",SDL_Color{255,255,255,255});
    info_text = SDL_Text(&info_font,"press [Z] to return to the menu",SDL_Color{192,192,192,255});
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::MapEdit,"Return_To_Menu",&State_MapEdit::ReturnToMenu,this);
    TTF_SetFontStyle(menu_font,TTF_STYLE_ITALIC);
    evMgr->AddCallback(StateType::MapEdit,"Up_Arrow",&State_MapEdit::SelectUp,this);
    evMgr->AddCallback(StateType::MapEdit,"Down_Arrow",&State_MapEdit::SelectDown,this);
    evMgr->AddCallback(StateType::MapEdit,"Left_Arrow",&State_MapEdit::SelectLeft,this);
    evMgr->AddCallback(StateType::MapEdit,"Right_Arrow",&State_MapEdit::SelectRight,this);
    evMgr->AddCallback(StateType::MapEdit,"MapEdit_Enter_Flip",&State_MapEdit::flip,this);
    evMgr->AddCallback(StateType::MapEdit,"MapEdit_Press_Selected",&State_MapEdit::PressSelected,this);
}

void State_MapEdit::OnDestroy() {
    delete map;
    TTF_CloseFont(menu_font);
    TTF_CloseFont(info_font);
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::MapEdit,"Return_To_Menu");
    evMgr->RemoveCallback(StateType::MapEdit,"Up_Arrow");
    evMgr->RemoveCallback(StateType::MapEdit,"Down_Arrow");
    evMgr->RemoveCallback(StateType::MapEdit,"Left_Arrow");
    evMgr->RemoveCallback(StateType::MapEdit,"Right_Arrow");
    evMgr->RemoveCallback(StateType::MapEdit,"MapEdit_Press_Selected");
    evMgr->RemoveCallback(StateType::MapEdit,"MapEdit_Enter_Flip");
}

void State_MapEdit::Update(float dt) {
    time_passed+=dt;

}

void State_MapEdit::Draw() {
    int2 size = {480,480};
    int2 o = {0,0};
    RenderWindow* window = m_stateMgr->GetContext()->window;
    auto center = window->ScreenCenter();
    o = center - size/2;
    int c = map->GetWidth();
    int r = map->GetHeight();
    int resx = size.x/c;
    int resy = size.y/r;
    window->draw(menu_text,center.x,center.y - 350);
    int padx = -1; int pady = -1;
    for(int i = 0; i < c;i ++) {
        for(int j = 0; j < r; j++) {
            auto cell = (*map)(i,j);
            SDL_Color color = ToColor(cell);
            auto rect = SDL_Rect {
                o.x + i*resx,o.y + j*resy, resx+padx,resy+pady
            };
            bool fill = cell != CellType::Empty;
            window->draw(rect,color,fill);
        }
    }
    SDL_Rect pointer_rect = {
            o.x + pointer.x*resx-2, o.y + pointer.y*resy-2,resx+4,resy+4
    };
    window->draw(pointer_rect,pointer_color,false);
    for(int i = 0; i < 7; i++) {
        auto cell = static_cast<CellType>(i);
        std::string str = "> "+ToString(cell);
        info2_text = SDL_Text(&info_font,str.c_str(), ToColor(cell));
        window->draw_text_uncentered(info2_text,0,32*(i+1));
    }
    window->draw(info_text,center.x,2*center.y-100);
    window->draw_guidlines({255,64,255,255});
}

void State_MapEdit::ReturnToMenu(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_MapEdit::SelectUp(EventDetails *l_details) {
    pointer.y-=1;
    if(pointer.y < 0) pointer.y = map->GetHeight()-1;
}

void State_MapEdit::SelectDown(EventDetails *l_details) {
    pointer.y+=1;
    if(pointer.y > map->GetHeight()-1) pointer.y = 0;
}

void State_MapEdit::SelectLeft(EventDetails *l_details) {
    pointer.x-=1;
    if(pointer.x < 0) pointer.x = map->GetWidth()-1;
}

void State_MapEdit::SelectRight(EventDetails *l_details) {
    pointer.x+=1;
    if(pointer.x > map->GetWidth()-1) pointer.x = 0;
}

void State_MapEdit::PressSelected(EventDetails *l_details) {
    if(!enter_guard) return;
    if((*map)(pointer.x,pointer.y) == CellType::LevelEnd) {
        (*map)(pointer.x,pointer.y) = CellType::Empty;
    } else (*map)(pointer.x,pointer.y) = static_cast<CellType>(static_cast<int>((*map)(pointer.x,pointer.y))+1);

}

void State_MapEdit::flip(EventDetails *l_details) {
    enter_guard = true;
}

