//
// Created by juicylucy on 4/11/23.
//

#ifndef ANNIHILATION64_STATE_MAPEDIT_H
#define ANNIHILATION64_STATE_MAPEDIT_H
#include "StateManager.h"
#include "Map.h"


class State_MapEdit : public BaseState{
public:
    State_MapEdit(StateManager* l_stateManager);

    void OnCreate() override;
    void OnDestroy() override;
    void Activate() override {}
    void Deactivate() override {}
    void Update(float dt) override;
    void Draw() override;
    void ReturnToMenu(EventDetails* l_details);
private:
    Map* map;

};


#endif //ANNIHILATION64_STATE_MAPEDIT_H
