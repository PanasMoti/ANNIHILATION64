//
// Created by juicylucy on 4/21/23.
//

#include "State_GameOver.h"
#include "header_onlys/Colors.h"
#include "header_onlys/GameData.h"

bool did_player_lost = false;
float2 flicker_cooldown = {0.0f,1.0f};
State_GameOver::State_GameOver(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_GameOver::OnCreate() {
    RenderWindow* window = m_stateMgr->GetContext()->window;
    skull = new Sprite("assets/skull",window->GetRenderer());
    you_died_img = new Texture(window->loadTexture("assets/game_over.png"));
    info_font = window->loadFont("assets/YosterIsland.ttf");
    hud_font = window->loadFont("assets/fonts/Doom2016.ttf",200);
    info_text = "press [Z] to return to the menu\n"
                "press [ESC] to quit the game";
    skull->time_between_frames = 0.2f;
//    skull->SetFPS(20);
    skull->SetSize(450,450);
    TTF_SetFontWrappedAlign(info_font,TTF_WRAPPED_ALIGN_CENTER);
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->AddCallback(StateType::GameOver,"Return_To_Menu",&State_GameOver::ReturnToMenu,this);
}

void State_GameOver::OnDestroy() {
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    evMgr->RemoveCallback(StateType::GameOver,"Return_To_Menu");
}

void State_GameOver::Activate() {
    skull->ToggleLoop();
    flicker = false;
    did_player_lost = GameData::self().player.hp == 0;
}

void State_GameOver::Deactivate() {
    skull->ToggleLoop();
}

void State_GameOver::Update(float dt) {
    skull->StartPlaying();
    skull->Update(dt);
    flicker_cooldown.x+=dt;
}

void State_GameOver::Draw() {
    RenderWindow* window = m_stateMgr->GetContext()->window;
    auto center = window->ScreenCenter();
    window->draw(*you_died_img,center.x,100,{true,false});
    window->draw(info_text,info_font,center.x,2*center.y-100,400,clWHITE,true);
    window->draw(skull,center.x,center.y,skull->GetCurrentFrameIndex(),{true,true});
    if(flicker_cooldown.x >= flicker_cooldown.y) {
        flicker_cooldown.x = 0;
        flicker = !flicker;
    }
    if(flicker) {
        if(did_player_lost) {
            window->draw("YOU LOST!",hud_font,center.x,center.y,clRED,true);
        } else {
            window->draw("YOU WON!",hud_font,center.x,center.y,clGREEN,true);
        }
    } else {
        window->draw(std::string("SCORE : ") + std::to_string(GameData::self().score),hud_font,center.x,center.y+250,clCYAN,true);
    }
}

void State_GameOver::ReturnToMenu(EventDetails *l_details) {
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

