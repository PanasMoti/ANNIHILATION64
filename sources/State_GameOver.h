//
// Created by juicylucy on 4/21/23.
//

#ifndef ANNIHILATION64_STATE_GAMEOVER_H
#define ANNIHILATION64_STATE_GAMEOVER_H
#include "StateManager.h"
#include "gfx/Texture.h"
#include "gfx/Sprite.h"
#include "RenderWindow.h"

class State_GameOver : public BaseState{
public:
    State_GameOver(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override;
    void Deactivate() override;
    void Update(float dt) override;
    void Draw() override;

    void ReturnToMenu(EventDetails* l_details);

private:
    Sprite* skull;
    Texture* you_died_img;
    TTF_Font* info_font;
    std::string info_text;

};


#endif //ANNIHILATION64_STATE_GAMEOVER_H
