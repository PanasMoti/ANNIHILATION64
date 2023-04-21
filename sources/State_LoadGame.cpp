//
// Created by juicylucy on 4/10/23.
//

#include "State_LoadGame.h"
#include <iostream>
#include "header_onlys/GameData.h"
#include "header_onlys/DataBase_Response.h"
#include "header_onlys/config.h"

State_LoadGame::State_LoadGame(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_LoadGame::OnCreate() {
    this->enter_guard = false; IsFoundInDB = false; showErrorMsg = false;
    letters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    time_passed = 0.0f; selectedX = 0; selectedY = 0; user_font_size = 48;
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Doom2016.ttf",160);
    info_font = m_stateMgr->GetContext()->window->loadFont("assets/YosterIsland.ttf",32);
    user_font = m_stateMgr->GetContext()->window->loadFont("assets/Emulogic.ttf",user_font_size);
    menu_text = "enter a password to load a game";
    keyboard_text = letters;

    info_text = ">press [z] to return to the menu\n>press [BACKSPACE] to delete\n>press [ENTER] to confirm\n>press [SPACE] to submit";
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::LoadGame,"Return_To_Menu",&State_LoadGame::ReturnToMenu,this);
    evMgr->AddCallback(StateType::LoadGame,"Up_Arrow",&State_LoadGame::SelectUp,this);
    evMgr->AddCallback(StateType::LoadGame,"Down_Arrow",&State_LoadGame::SelectDown,this);
    evMgr->AddCallback(StateType::LoadGame,"Left_Arrow",&State_LoadGame::SelectLeft,this);
    evMgr->AddCallback(StateType::LoadGame,"Right_Arrow",&State_LoadGame::SelectRight,this);
    evMgr->AddCallback(StateType::LoadGame, "LoadGame_Press_Selected", &State_LoadGame::PressSelected, this);
    evMgr->AddCallback(StateType::LoadGame,"LoadGame_Enter_Flip",&State_LoadGame::flip,this);
    evMgr->AddCallback(StateType::LoadGame,"LoadGame_Backspace",&State_LoadGame::Backspace,this);
    evMgr->AddCallback(StateType::LoadGame,"LoadGame_Submit",&State_LoadGame::SubmitInput,this);
    password = "";

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
    evMgr->RemoveCallback(StateType::LoadGame,"LoadGame_Submit");
}

void State_LoadGame::Update(float dt) {
    time_passed+=dt;
    if(IsFoundInDB) {
        GameData& gameData = GameData::self();
        gameData.level = DataBase_Response::self().str;

        m_stateMgr->SwitchTo(StateType::GamePlay);
    }
}

void State_LoadGame::ReturnToMenu(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_LoadGame::Draw() {

    RenderWindow* window = m_stateMgr->GetContext()->window;
    int2 center = window->ScreenCenter();
    int w = 1250; int h = 150;
    SDL_Rect rect = {center.x - w/2,center.y - h/2 - 150,w,h};

    window->draw(menu_text,menu_font,center.x,center.y-350,{255,255,255,255},true);
    window->draw(rect,SDL_Color{255,255,255,255},true);
//    window->draw_guidlines({255,64,255,255});
    if(password.empty() || password.length()-1 < max_pass_len-1) {
        SDL_Rect size;
        window->draw(keyboard_text,user_font, center.x, rect.y + 350, user_font_size * 9,{255,255,255,255},true, &size);
        int pw = size.w/9; int ph = size.h/4;
        SDL_Rect selected_char = {size.x+pw*selectedX,size.y+selectedY*ph,pw,ph};
        window->draw(selected_char,{255,255,64,255});
    } else {
        window->draw("press [space] to continue",info_font,center.x,center.y+100,{255,64,64,255},true);
    }
    window->draw(password.empty() ? "enter here" : password,user_font,rect.x+10,rect.y+32,{0,0,0,255},false);
    window->draw(info_text,info_font,center.x,2*center.y-100,700,{192,192,192,255},true);
    if(!IsFoundInDB && showErrorMsg) {
        window->draw("password not found!",info_font,center.x,center.y-200,{255,0,0,255},true);
    }
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
    if(!enter_guard) return;
    showErrorMsg = false;
    if(password.length() > max_pass_len) return;
    password += letters[selectedX + selectedY*9];
}

void State_LoadGame::flip(EventDetails *l_details) {
    this->enter_guard = true;
}

void State_LoadGame::Backspace(EventDetails *l_details) {
    if(password.length() != 0) password.pop_back();
}
void State_LoadGame::Activate() {
    enter_guard = false;
    sqlite3_open(DB_PATH,&DB);
}
void State_LoadGame::Deactivate() {
    sqlite3_close(DB);
}

void State_LoadGame::SubmitInput(EventDetails *l_details) {
    showErrorMsg = true;
    std::string query = "SELECT * FROM GAME WHERE PASSWORD='" + password + "';";
    const std::string* data;
    char *errmsg;
    sqlite3_exec(DB,query.c_str(),callback,(void*)&data,&errmsg);
    if(DataBase_Response::self().str.empty()) {
        IsFoundInDB = false;
    } else {
        IsFoundInDB = true;
    }
//    std::cout << DataBase_Response::self().str << std::endl;
}

