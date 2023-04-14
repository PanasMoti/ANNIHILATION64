#pragma once

#include "RenderWindow.h"
#include "config.h"
#include "EventManager.h"
#include "StateManager.h"

class Game
{
public:
	Game();
	~Game();
	void close(EventDetails* l_details = nullptr);
	bool shouldClose;
	void render();
	void update();
	void lateUpdate();
	bool IsFocused();
    void destroy();
private:
	RenderWindow* window;
	EventManager m_eventManager;
	bool m_isFocused;
	SharedContext m_context;
	StateManager m_stateManager;
};

