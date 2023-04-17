//
// Created by juicylucy on 4/16/23.
//

#include "State_GamePlay.h"
#include <iostream>

const char* arr[] = {
        "Forward_Press",
        "Left_Press",
        "Right_Press",
        "Backward_Press",
// -----------------------------
        "Forward_Release",
        "Left_Release",
        "Right_Release",
        "Backward_Release"
};

State_GamePlay::State_GamePlay(StateManager *l_stateManager) : BaseState(l_stateManager) {
    OnCreate();
}

void State_GamePlay::OnCreate() {
    keys_state[119] = false;
    keys_state[97] = false;
    keys_state[100] = false;
    keys_state[115] = false;
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;
    for(int i = 0; i < 8; i++) {
        if(i < 4) {
            evMgr->AddCallback(StateType::GamePlay,arr[i],&State_GamePlay::KeyPressed,this);
        } else {
            evMgr->AddCallback(StateType::GamePlay,arr[i],&State_GamePlay::KeyReleased,this);
        }
    }
}

void State_GamePlay::OnDestroy() {
    EventManager* evMgr = m_stateMgr->GetContext()->eventMgr;

    for(int i = 0; i < 8; i++) {
        evMgr->RemoveCallback(StateType::GamePlay,arr[i]);
    }
}

void State_GamePlay::Activate() {
    //TODO: process the level into the map;
    std::cout << GameData::self().level << std::endl;
    GameData& gameData = GameData::self();
    Map map;
    std::string level = gameData.level;
    std::string height_str = level;
    if(level.empty()) return;
    std::size_t pos = level.find("MAP = ");
    level = level.substr(pos);
    height_str = height_str.substr(height_str.find("WIDTH = "));
    height_str.erase(height_str.begin() + height_str.find("M"), height_str.end());
    std::string width_str = height_str;
    width_str.erase(std::remove_if(width_str.begin(), width_str.end(), ::isspace),
                    width_str.end());
    height_str = width_str.substr(width_str.find("HEIGHT="));
    width_str.erase(width_str.find("HEIGHT=") + width_str.begin(), width_str.end());
    height_str = height_str.substr(height_str.find("=") + 1);
    width_str = width_str.substr(width_str.find("=") + 1);
    pos = level.find("HP");
    level.erase(level.begin() + pos,level.end());
    level = level.substr(level.find("1"));
    map.SetSize(std::stoi(width_str),std::stoi(height_str));
    gameData.SetMap(map);

}

void State_GamePlay::Deactivate() {

}

void State_GamePlay::Update(float dt) {

}

void State_GamePlay::Draw() {

}

void State_GamePlay::KeyPressed(EventDetails *l_details) {
    int keycode = l_details->keyCode;
    keys_state[keycode] = true;

}

void State_GamePlay::KeyReleased(EventDetails *l_details) {
    int keycode = l_details->keyCode;
    keys_state[keycode] = false;
}
