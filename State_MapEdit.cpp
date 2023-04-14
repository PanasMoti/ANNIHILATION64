//
// Created by juicylucy on 4/11/23.
//

#include "State_MapEdit.h"
#include <iostream>
#include <fstream>
#include "config.h"

State_MapEdit::State_MapEdit(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
    custom_pass = random_string(rand()%17 + 1);
}

void State_MapEdit::OnCreate() {
    enter_guard = false; written_to_db = false;
    time_passed = 0.0f;
    pointer = {0,0};
    pointer_color = {255,255,0,255};
    map = Map(16,16);
    menu_font = m_stateMgr->GetContext()->window->loadFont("assets/Doom2016.ttf",160);
    info_font = m_stateMgr->GetContext()->window->loadFont("assets/YosterIsland.ttf",32);
    menu_text = "edit your own level";
    info_text = ">press [Z] tp return to menu\n>use the [ARROWS] to move\n>press [ENTER] to change the selected";
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::MapEdit,"Return_To_Menu",&State_MapEdit::ReturnToMenu,this);
    TTF_SetFontStyle(menu_font,TTF_STYLE_ITALIC);
    evMgr->AddCallback(StateType::MapEdit,"Up_Arrow",&State_MapEdit::SelectUp,this);
    evMgr->AddCallback(StateType::MapEdit,"Down_Arrow",&State_MapEdit::SelectDown,this);
    evMgr->AddCallback(StateType::MapEdit,"Left_Arrow",&State_MapEdit::SelectLeft,this);
    evMgr->AddCallback(StateType::MapEdit,"Right_Arrow",&State_MapEdit::SelectRight,this);
    evMgr->AddCallback(StateType::MapEdit,"MapEdit_Enter_Flip",&State_MapEdit::flip,this);
    evMgr->AddCallback(StateType::MapEdit,"MapEdit_Press_Selected",&State_MapEdit::PressSelected,this);
    evMgr->AddCallback(StateType::MapEdit,"MapEdit_Save",&State_MapEdit::SaveMap,this);
}

void State_MapEdit::OnDestroy() {
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
    evMgr->RemoveCallback(StateType::MapEdit,"MapEdit_Save");
    sqlite3_close(DB);
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
    int c = map.GetWidth();
    int r = map.GetHeight();
    int resx = size.x/c;
    int resy = size.y/r;
    if(written_to_db) {
        window->draw(custom_pass,info_font,center.x,center.y,{255,0,0,255},true);
    } else {
        window->draw(menu_text,menu_font,center.x,center.y - 350,{255,255,255,255},true);
        int padx = -1; int pady = -1;
        for(int i = 0; i < c;i ++) {
            for(int j = 0; j < r; j++) {
                auto cell = map(i,j);
                SDL_Color color = ToColor(cell);
                auto rect = SDL_Rect {
                        o.x + i*resx,o.y + j*resy, resx+padx,resy+pady
                };

                window->draw(rect,color,true);
            }
        }
        SDL_Rect pointer_rect = {
                o.x + pointer.x*resx-2, o.y + pointer.y*resy-2,resx+4,resy+4
        };
        window->draw(pointer_rect,pointer_color,false);
        for(int i = 0; i < 7; i++) {
            auto cell = static_cast<CellType>(i);
            std::string str = "> "+ToString(cell);
            window->draw(str,info_font,{0,32*(i+1)}, ToColor(cell));
        }
    }
    window->draw(info_text,info_font,center.x,2*center.y-100,700,{192,192,192,255},true);

    //    window->draw_guidlines({255,64,255,255});
}

void State_MapEdit::ReturnToMenu(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_MapEdit::SelectUp(EventDetails *l_details) {
    pointer.y-=1;
    if(pointer.y < 0) pointer.y = map.GetHeight()-1;
}

void State_MapEdit::SelectDown(EventDetails *l_details) {
    pointer.y+=1;
    if(pointer.y > map.GetHeight()-1) pointer.y = 0;
}

void State_MapEdit::SelectLeft(EventDetails *l_details) {
    pointer.x-=1;
    if(pointer.x < 0) pointer.x = map.GetWidth()-1;
}

void State_MapEdit::SelectRight(EventDetails *l_details) {
    pointer.x+=1;
    if(pointer.x > map.GetWidth()-1) pointer.x = 0;
}

void State_MapEdit::PressSelected(EventDetails *l_details) {
    if(!enter_guard) return;
    auto tmp = static_cast<int>(map(pointer.x,pointer.y));
    tmp++; if(tmp >= 7) tmp = 0;
    map(pointer.x,pointer.y) = static_cast<CellType>(tmp);}

void State_MapEdit::flip(EventDetails *l_details) {
    enter_guard = true;
}

static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}


void State_MapEdit::SaveMap(EventDetails *l_details) {
    if(written_to_db) return;
    written_to_db = true;
    std::string str;
    for(int i = 0; i < map.GetWidth(); i++) {
        for(int j = 0; j < map.GetHeight(); j++) {
            auto cell = map(i,j);
            int cell_value = static_cast<int>(cell);
            str += std::to_string(cell_value);
        }

    }
    int e;
    do {
        std::string sql("INSERT INTO GAME VALUES('" + custom_pass + "',"
                        + std::to_string(map.GetWidth()) + "," +
                        std::to_string(map.GetHeight()) + ",'" + str +"',3,50);");
        char* errmsg;
        e = sqlite3_exec(DB,sql.c_str(), nullptr,0,&errmsg);
        if(e != SQLITE_OK) custom_pass = random_string(rand()%17 + 1);
    }while(e != SQLITE_OK);
}

void State_MapEdit::Activate() {
    sqlite3_open("../data/storage.db",&DB);
    map.SetSize(map.GetSize());
    written_to_db = false;
    custom_pass = random_string(rand()%17 + 1);
    enter_guard = false;
}

void State_MapEdit::Deactivate() {
    sqlite3_close(DB);
}