#include "Game.h"
#include "header_onlys/config.h"

Game::Game() :m_stateManager(&m_context)  // passing a reference to the context to the statemanager
{
	// making sure we init everything correctly
	// ------------------------------------------
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	assert(IMG_Init(IMG_INIT_PNG) != 0);
	assert(TTF_Init() == 0);
	// ------------------------------------------


	window = new RenderWindow(TITLE, WIDTH, HEIGHT); // creating a new instance of RenderWindow on the heap
    // which creates a new SDL_Window and SDL_Renderer and the buffer which will be used later on for the gameplay state
	shouldClose = false;
	m_isFocused = true;
    // adding the global callback so whenever the user presses escape the game will close regardless of current state
	m_eventManager.AddCallback(StateType(0),"Window_close", &Game::close, this);
	m_context.window = window; // pass the pointer to the window to the shared context
	m_context.eventMgr = &m_eventManager; // pass a reference to the event manager to the shared context
	m_stateManager.SwitchTo(StateType::MainMenu); // switching to the main menu state which is the default state and the first this the user will see when opening the game
}


void Game::render() {
    // 1.) clear the screen
	window->clear_screen();
    // 2.) write to the gpu
	m_stateManager.Draw();
    // 3.) swap the buffer in the gpu
	window->display();
}

void Game::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) { // poll all the events
		if (event.type == SDL_WINDOWEVENT_FOCUS_LOST) { // if we lost focus update the m_isFocused variable and update it in the event manager
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED) { // if we gained focus update the variable and the event manager
			m_isFocused = true;
			m_eventManager.SetFocus(true);
		}
		m_eventManager.HandleEvent(event); // pass the event to the eventManager to handle it
	}
	m_eventManager.Update(); // update the event manager
    clock.tick(); // calculate deltaTime
    m_stateManager.Update(clock.delta*0.001); // pass the deltaTime(converted to seconds from ms) to stateManager which will pass it to the current state
}

void Game::lateUpdate() {
	m_stateManager.ProcessRequests(); // process any AddState / RemoveState etc functions
}

bool Game::IsFocused() const
{
	return m_isFocused;
}

void Game::close(EventDetails* l_details) {
	this->shouldClose = true;
}

void Game::destroy() {
    this->window->destroy(); // close the window
    delete this->window; // delete the object
    SDL_Quit(); // quit SDL
    IMG_Quit(); // quit IMG
    TTF_Quit(); // quit TTF
    this->m_stateManager.Remove(StateType(0)); // remove the global State aka all states
}

