#pragma once

#include "RenderWindow.h"
#include "header_onlys/config.h"
#include "EventManager.h"
#include "StateManager.h"


///@brief a struct that measurse the time between frames ( deltaTime ) that is later used to update in the same speed regardless of framerate
struct Clock {
    //!@property unsigned integer of 32 bit that stores the time of the last tick
    uint32_t last_tick_time = 0;
    //!@property unsigned integer of 32 bit that stores the delta time
    uint32_t delta = 0;
    ///@brief a function that is to be called every frame,this function calculates the delta time
    void tick()
    {
        uint32_t tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        last_tick_time = tick_time;
    }
}; // Struct Clock



///@brief This class is our "High Level" object of our code
/// what i mean is that in our main.cpp file, in the main.cpp function we only have to write the lines that make up the basic
/// logic of an application -> 1.) create the application 2.) check if it should close 3.) if not , update variables, draw to the screen , process late updates and loop
/// 4.) if it should close destroy the application
class Game
{
public:
    ///@publicsection
    ///@brief a costructor that initalize all the members , aka creating the window, load the binding , etc...
	Game();
    /// a function to match the global callback Window_Close which as the name suggests closes the game
    /// \param l_details this isn't really used but since a callback has to have this parameter
	void close(EventDetails* l_details = nullptr);
    ///@property a boolean that stores if the game should close
	bool shouldClose;
    ///@brief a function that renders the game
	void render();
    ///@brief a function that updates all the variables
	void update();
    ///@brief a function for proccessing state_manager updates
	void lateUpdate();
    /// Getter
    /// @return the value of m_isFocused
	bool IsFocused() const;
    ///@brief a function to close the application
    void destroy();
private:
    ///@privatesection
    ///@property a clock so we can use deltaTime and measure Time
    Clock clock;
    ///@property a pointer to a RenderWindow , which deals with opening a window, drawing to the renderer, displaying the renderer ,etc...
	RenderWindow* window;
    ///@property an Event Manager that handles all the events
	EventManager m_eventManager;
    ///@property a bool variable that stores if the application is in focus
	bool m_isFocused;
    ///@property shared device context aka the window and state manager
	SharedContext m_context;
    ///@property our state manager that deals with what should be displayed when
	StateManager m_stateManager;
};

