#include "Game.h"

Game::Game() :m_stateManager(&m_context) {
	// making sure we init everything correctly
	// ------------------------------------------
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	assert(IMG_Init(IMG_INIT_PNG) != 0);
	assert(TTF_Init() == 0);
	// ------------------------------------------


	window = new RenderWindow("ANNIHILATION64", 1280, 720);
	shouldClose = false;
	m_isFocused = true;
	m_eventManager.AddCallback(StateType::MainMenu,"Window_close", &Game::close, this);
	m_context.window = window;
	m_context.eventMgr = &m_eventManager;
	m_stateManager.SwitchTo(StateType::MainMenu);
}

Game::~Game() {

	delete this->window;

	// Making sure we close everything
	// -----------------------------------
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	// -----------------------------------
}

void Game::render() {
	window->clear_screen();
	m_stateManager.Draw();
	window->display();
}

void Game::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_WINDOWEVENT_FOCUS_LOST) {
			m_isFocused = false;
			m_eventManager.SetFocus(false);
		}
		else if (event.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
			m_isFocused = true;
			m_eventManager.SetFocus(true);
		}
		m_eventManager.HandleEvent(event);
	}
	m_eventManager.Update();
	m_stateManager.Update();
}

void Game::lateUpdate() {
	m_stateManager.ProcessRequests();
}

bool Game::IsFocused()
{
	return m_isFocused;
}

void Game::close(EventDetails* l_details) {
	this->shouldClose = true;
}