//
// Created by juicylucy on 4/10/23.
//

#include "State_LoadGame.h"

State_LoadGame::State_LoadGame(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_LoadGame::OnCreate() {
    this->toggle = false;
    letters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    time_passed = 0.0f; selectedX = 0; selectedY = 0; user_font_size = 64;
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Doom2016.ttf",160);
    info_font = m_stateMgr->GetContext()->window->loadFont("assets/Emulogic.ttf",32);
    user_font = m_stateMgr->GetContext()->window->loadFont("assets/Emulogic.ttf",user_font_size);
    menu_text = SDL_Text(&menu_font,"enter a password to load a game",SDL_Color{255,255,255,255});
    user_input = SDL_Text(&user_font,letters,SDL_Color{255,255,255,255});
    info_text = SDL_Text(&info_font,"press [Z] to return to the menu",SDL_Color{192,192,192,255});
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::LoadGame,"Return_To_Menu",&State_LoadGame::ReturnToMenu,this);
    evMgr->AddCallback(StateType::LoadGame,"Up_Arrow",&State_LoadGame::SelectUp,this);
    evMgr->AddCallback(StateType::LoadGame,"Down_Arrow",&State_LoadGame::SelectDown,this);
    evMgr->AddCallback(StateType::LoadGame,"Left_Arrow",&State_LoadGame::SelectLeft,this);
    evMgr->AddCallback(StateType::LoadGame,"Right_Arrow",&State_LoadGame::SelectRight,this);
    evMgr->AddCallback(StateType::LoadGame, "LoadGame_Press_Selected", &State_LoadGame::PressSelected, this);
    evMgr->AddCallback(StateType::LoadGame,"LoadGame_Enter_Flip",&State_LoadGame::flip,this);
    evMgr->AddCallback(StateType::LoadGame,"LoadGame_Backspace",&State_LoadGame::Backspace,this);
    password.second = SDL_Text(&user_font," ",SDL_Color{0,0,0,255});
    password.first = ":";
}

void State_LoadGame::OnDestroy() {
    TTF_CloseFont(menu_font);
    TTF_CloseFont(user_font);
    TTF_CloseFont(info_font);
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::LoadGame,"Return_To_Menu");
    evMgr->RemoveCallback(StateType::LoadGame,"Up_Arrow");
    evMgr->RemoveCallback(StateType::LoadGame,"Down_Arrow");
    evMgr->RemoveCallback(StateType::LoadGame,"Left_Arrow");
    evMgr->RemoveCallback(StateType::LoadGame,"Right_Arrow");
    evMgr->RemoveCallback(StateType::LoadGame,"LoadGame_Press_Selected");
    evMgr->RemoveCallback(StateType::LoadGame,"LoadGame_Enter_Flip");
    evMgr->RemoveCallback(StateType::LoadGame,"LoadGame_Backspace");
}

void State_LoadGame::Update(float dt) {
    time_passed+=dt;
    password.second.text = password.first;
}

void State_LoadGame::ReturnToMenu(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_LoadGame::Draw() {

    RenderWindow* window = m_stateMgr->GetContext()->window;
    int2 center = window->ScreenCenter();
    int w = 1250; int h = 150;
    SDL_Rect rect = {center.x - w/2,center.y - h/2 - 150,w,h};

    window->draw(menu_text,center.x,center.y-350);
    window->draw(rect,SDL_Color{255,255,255,255},true);
    window->draw_guidlines({255,64,255,255});
    if(password.first.length()-1 < max_pass_len) {
        SDL_Rect size;
        window->draw(user_input,center.x,rect.y+350,user_font_size*9,&size);
        int pw = size.w/9; int ph = size.h/4;
        SDL_Rect selected_char = {size.x+pw*selectedX,size.y+selectedY*ph,pw,ph};
        window->draw(selected_char,{255,255,64,255});
    } else {
        window->draw(SDL_Text(&info_font,"press [space] to continue",{255,64,64,255}),center.x,center.y+100);
    }
    window->draw_text_uncentered(password.second,rect.x+10,rect.y+32);
    window->draw(info_text,center.x,2*center.y-100);
}

void State_LoadGame::SelectUp(EventDetails *l_details) {
    selectedY--;
    if(selectedY < 0) selectedY = 3;
}

void State_LoadGame::SelectDown(EventDetails *l_details) {
    selectedY++;
    if(selectedY > 3) selectedY = 0;
}

void State_LoadGame::SelectLeft(EventDetails *l_details) {
    selectedX--;
    if(selectedX < 0) selectedX = 8;
}

void State_LoadGame::SelectRight(EventDetails *l_details) {
    selectedX++;
    if(selectedX > 8) selectedX = 0;
}

void State_LoadGame::PressSelected(EventDetails *l_details) {
    if(!toggle) return;
    if(password.first.length() > max_pass_len) return;
    password.first += letters[selectedX + selectedY*9];
}

void State_LoadGame::flip(EventDetails *l_details) {
    this->toggle = true;
}

void State_LoadGame::Backspace(EventDetails *l_details) {
    if(password.first.length() == 1) return;
    password.first.pop_back();
}
