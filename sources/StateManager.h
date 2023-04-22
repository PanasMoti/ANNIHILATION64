#pragma once

#include "header_onlys/BaseState.h"
#include "EventManager.h"
#include <utility>
#include <unordered_map>

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
using TypeContainer = std::vector<StateType>;
using StateFactory = std::unordered_map<StateType, std::function<BaseState* (void)>>;


//!@class manages the current state,updates the state,passing information around (example a pointer to the RenderWindow)
class StateManager
{
public:
    /// explicit Constructor, this one also links the different state classes to their StateType
    /// \param l_shared a pointer to the shared context
	explicit StateManager(SharedContext* l_shared);
    /// destructor used to call the destroy all the states and unlink them
	~StateManager();

    /// this is called every frame, it calls the update method of the current state
    /// \param dt delta Time, the time between frames in seconds
	void Update(float dt);
    /// called every frame, calls the draw method of the current state
	void Draw();

    /// Removes the state that was requested to be removed
	void ProcessRequests();

    /// Getter
    /// \return returns a pointer to the Shared Context
	SharedContext* GetContext();
    /// Checks if the state manager has already a state linked
    /// \param l_type the type we want to check for
    /// \return true if the manager has this state , false otherwise
	bool HasState(const StateType& l_type);

    /// changes the current State , also calls the SetCurrentState method of the event manager
    /// this goes through every state it has and if found it deactivates the current state, changes the current state, and activates the
    /// state we switched to.
    /// if the state is not found , it creates it,adds it to container,and switches to it
    /// \param l_type
	void SwitchTo(const StateType& l_type);
    /// adds to ToRemove vector which will be processed in the ProcessRequests method
    /// \param l_type the type we want to remove
	void Remove(const StateType& l_type);
private:
	//Methods
    /// Checks if State already exists, if not , we create a new State, else we don't do nothing
    /// \param l_type the type we want to create
	void CreateState(const StateType& l_type);
    /// goes through the states vector, if it finds it , it calls its Destroy method and deletes it,otherwise it does nothing
    /// \param l_type
	void RemoveState(const StateType& l_type);

    /**
    This templated function is used to register a new state in a state machine.
    It takes a StateType parameter and a class template parameter T.
    It creates a new instance of a state object of type T by passing a pointer to the current object as a parameter.
    The function stores the new state instance in the m_stateFactory map with a key value of l_type.
    This allows the state to be easily accessed and initialized later on when needed.
     @param l_type the type we want to register
     */
	template<class T>
	void RegisterState(const StateType& l_type) {
		m_stateFactory[l_type] = [this]() -> BaseState* {
			return new T(this);
		};
	}

	// Members
	SharedContext* m_shared;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;
};

