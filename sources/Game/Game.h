#pragma once

#include "../Render Window/RenderWindow.h"
#include "../util/config.h"
#include "../States/Events/EventManager.h"
#include "../States/State Manager/StateManager.h"

struct Clock {
    uint32_t last_tick_time = 0;
    uint32_t delta = 0;
    void tick()
    {
        uint32_t tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        last_tick_time = tick_time;
    }
};

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
    Clock clock;
	RenderWindow* window;
	EventManager m_eventManager;
	bool m_isFocused;
	SharedContext m_context;
	StateManager m_stateManager;
};

