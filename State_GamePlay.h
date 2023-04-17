//
// Created by juicylucy on 4/16/23.
//

#ifndef ANNIHILATION64_STATE_GAMEPLAY_H
#define ANNIHILATION64_STATE_GAMEPLAY_H
#include "StateManager.h"
#include <SDL2/SDL.h>
#include "GameData.h"
#include <unordered_map>



class State_GamePlay : public BaseState{
    using keyboard = std::unordered_map<int,bool>;
public:
    State_GamePlay(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override;
    void Deactivate() override;
    void Update(float dt) override;
    void Draw() override;

    void KeyPressed(EventDetails* l_details);
    void KeyReleased(EventDetails* l_details);

private:
    keyboard keys_state;
};


#endif //ANNIHILATION64_STATE_GAMEPLAY_H
