//
// Created by juicylucy on 4/11/23.
//

#ifndef ANNIHILATION64_STATE_MAPEDIT_H
#define ANNIHILATION64_STATE_MAPEDIT_H
#include "../State Manager/StateManager.h"
#include "../../Map/Map.h"
#include <sqlite3.h>
class State_MapEdit : public BaseState{
public:
    State_MapEdit(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override;
    void Deactivate() override;
    void Update(float dt) override;
    void Draw() override;
    void ReturnToMenu(EventDetails* l_details);
    void SelectUp(EventDetails* l_details);
    void SelectDown(EventDetails* l_details);
    void SelectLeft(EventDetails* l_details);
    void SelectRight(EventDetails* l_details);
    void PressSelected(EventDetails* l_details);
    void SaveMap(EventDetails* l_details);
    void ChaneMapSize(EventDetails* l_details);
    void MapIsntValid();
    void NotAllowed();

    void flip(EventDetails* l_details);
private:
    Map map;
    TTF_Font* menu_font;
    std::string menu_text;
    TTF_Font* info_font;
    std::string info_text;
    int2 pointer;
    bool enter_guard;
    SDL_Color pointer_color;
    std::string index_text;
    sqlite3* DB;
    std::string custom_pass;
    bool written_to_db;
    bool map_is_valid;
    bool is_not_allowed;
};


static bool CompareSDLColors(SDL_Color c,SDL_Color c2) {
    return c.r == c2.r && c.g == c2.g && c.b == c2.b && c.a == c2.a;
}


#endif //ANNIHILATION64_STATE_MAPEDIT_H
