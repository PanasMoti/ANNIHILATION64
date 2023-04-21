//
// Created by juicylucy on 4/9/23.
//

#include "State_MainMenu.h"
#include <iostream>
#include <sqlite3.h>
#include "../../util/DataBase_Response.h"
#include "../../util/GameData.h"
#include "../../util/config.h"

void State_MainMenu::OnCreate() {
    time_passed = 0.0f;
    selected = 0;
    selected_color = SDL_Color {255,255,0,255};
    default_color = SDL_Color {255,64,64,255};
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Doom2016.ttf",160);
    info_font = m_stateMgr->GetContext()->window->loadFont("assets/YosterIsland.ttf", 32);
    options[0] = "new game"; options[1] = "load game"; options[2] = "custom game";
    info = ">press [ESC] to quit\n>press [ENTER] to confirm\n>use the [UP] and [DOWN] arrows to move your select";
    menu_logo = new Texture(m_stateMgr->GetContext()->window->loadTexture("assets/logo.png"));
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::MainMenu,"MainMenu_Continue",&State_MainMenu::Continue,this);
    evMgr->AddCallback(StateType::MainMenu,"Up_Arrow",&State_MainMenu::SelectUp,this);
    evMgr->AddCallback(StateType::MainMenu,"Down_Arrow",&State_MainMenu::SelectDown,this);
    this->menu_logo->Scale(1.5f,1.5f);

}

void State_MainMenu::OnDestroy() {
    TTF_CloseFont(menu_font);
    TTF_CloseFont(info_font);
    menu_logo->Unload();
    delete menu_logo;
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::MainMenu,"MainMenu_Continue");
    evMgr->RemoveCallback(StateType::MainMenu,"Up_Arrow");
    evMgr->RemoveCallback(StateType::MainMenu,"Down_Arrow");

}


void State_MainMenu::Update(float dt) {
    time_passed += dt;

}
void State_MainMenu::Draw() {
    RenderWindow* window = m_stateMgr->GetContext()->window;
    int2 p = window->ScreenCenter();
    window->draw(*menu_logo,p.x - menu_logo->GetWidth()/2,-10);

    for(int i = 0; i < 3; i++) {
        window->draw((i==selected)? "\\" + options[i] + "/" : options[i],
                     menu_font,
                     p.x,p.y-150*(3-i) + 300,
                     (i==selected)?
                        selected_color : default_color,
                     true);
    }
    window->draw(info,info_font,p.x,2*p.y-100,700,{255,255,255,255},true);
}

void State_MainMenu::Continue(EventDetails *l_details) {

    switch (selected) {
        case 0: {
            DataBase_Response &response = DataBase_Response::self();
            sqlite3 *DB;
            sqlite3_open(DB_PATH, &DB);
            const char *query = "SELECT * FROM GAME WHERE PASSWORD = 'NEWGAME';";
            const char *data;
            char *errmsg;
            sqlite3_exec(DB, query, callback, (void *) data, &errmsg);
            sqlite3_close(DB);
            GameData &gameData = GameData::self();
            gameData.level = response.str;
            gameData.is_new_game = true;
            m_stateMgr->SwitchTo(StateType::GamePlay);
        }
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

void State_MainMenu::Activate() {

}
void State_MainMenu::Deactivate() {

}