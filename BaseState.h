#pragma once

#include "RenderWindow.h"
class StateManager;
class EventManager;

class BaseState
{
	friend class StateManager;
public:
	BaseState(StateManager* l_stateManager) :m_stateMgr(l_stateManager) {}
	virtual ~BaseState() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

	StateManager* GetStateManager() {
		return m_stateMgr;
	}
protected:
    float time_passed;
	StateManager* m_stateMgr;
};

enum class StateType {
	MainMenu = 1,
	GamePlay,
	GameOver,
	MapEdit,
	PasswordInput
};


struct SharedContext {
	
	SharedContext() : window(nullptr), eventMgr(nullptr) {}
	RenderWindow* window;
	EventManager* eventMgr;
};
