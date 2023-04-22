#pragma once

#include "../RenderWindow.h"
class StateManager;
class EventManager;



///@class a basic barebones virtual class of a State
class BaseState
{
    ///! forward declaration of the class StateManager that will be implemented later on
    ///! this is to avoid recursive definitions because the StateManager class header file will need to include
    ///! the BaseState header file
	friend class StateManager;
public:
	BaseState(StateManager* l_stateManager) :m_stateMgr(l_stateManager) {}

    // virtual functions that every derived class will have to implement
    //! @brief virtual destructor
    virtual ~BaseState() {}
    //! @brief a function that will be called upon creating an object of a derived class
	virtual void OnCreate() = 0;
    //! @brief a function that will be called upon deleting an object of a derived class
	virtual void OnDestroy() = 0;

    //! @brief a function that will be used when the derived state is being used
	virtual void Activate() = 0;
    //! @brief a function that will be used when the derived state is no longer being used
	virtual void Deactivate() = 0;

    //! @brief a function that will be used to update variables every frame
    //! @param dt: floating point number representing the time since the last frame ( DeltaTime ), used to make the game work
    //! independent of the frame rate
	virtual void Update(float dt) = 0;
    //! @brief a function that will be called to draw objects of the state to the screen
	virtual void Draw() = 0;

    //!@brief Getter function for the m_stateMgr member of the BaseState Class
    //! @return a pointer to a StateManager member
	StateManager* GetStateManager() {
		return m_stateMgr;
	}
protected:
    float time_passed;
	StateManager* m_stateMgr;
};


//!@brief enum class that represents the types of states our game has
//!@note their values start from 1, 0 will represent no state
enum class StateType {
	MainMenu = 1,
	GamePlay,
	GameOver,
	MapEdit,
	LoadGame
};


///@brief struct used to keep the device context that will be used by our different states
///this struct holds a pointer to some of our most commonly used classes or "devices". Because there's more than one
/// it's quite useful to define a simple struct that will keep around pointers to the window class and the event manager
struct SharedContext {
	//!@brief simple constructor that will initalize the members as nullptr
	SharedContext() : window(nullptr), eventMgr(nullptr) {}
	RenderWindow* window;
	EventManager* eventMgr;
};
