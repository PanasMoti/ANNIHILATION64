#pragma once
#include <SDL2/SDL_events.h>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include "import/linalg.h"
#include <functional>
#include "RenderWindow.h"
#include "header_onlys/BaseState.h"

using namespace linalg::aliases;

/// an enum class of events, currently this only has events from the SDL_EVENT MACROS but this can be expended in the future to
/// include custom events
/// @note: currently we only really use the KeyDown,KeyUp and Closed Event. this is because we don't use the mouse in the game,
/// as the game meant to mimic 90's arcade games, this can be changed in the future so its here anyways
enum class EventType {
	KeyDown = SDL_KEYDOWN, //768
	KeyUp = SDL_KEYUP, // 769
	MButtonDown = SDL_MOUSEBUTTONDOWN, // 1025
	MButtonUp = SDL_MOUSEBUTTONUP, // 1026
	Closed = SDL_QUIT, // 256
	MouseWheel = SDL_MOUSEWHEEL //1027
};


///@brief a struct that is used to store these groups of events for each binding
/// we know that in order to bind to a key, we need both the event type and the code for the key that
/// we're interested in. Some events we'll be working with only need to havea type stored,
/// in which cases we can simply store an integer value of 0 with the type.
struct EventInfo
{
    ///@brief Constructor for Events that doesn't need to store the key
	EventInfo() { m_code = 0; }
    ///@brief Constructor fo Events that do need to store the key
	EventInfo(int l_event) { m_code = l_event; }
    ///@property
    /// in order to leave room for future expantion we use a union to hold the keycode
	/// since it is a nameless union we can still access it like EventInfo.m_code
    union
	{
		int m_code;
	};
};

///@typedef here set up the data type that we're going to be using to hold the event information
using Events = std::vector<std::pair<EventType, EventInfo>>;


/// since we are going to need to share the event information with code that uses this class
/// we will set a data type (struct) which will helps us with it
/// this is the EventDetails struct which holds the information
struct EventDetails {
    ///@brief simple constructor that takes in a constant refrence to std::string which represents the
    /// name of the binding, this constructor sets the name property to the string and calls the Clear function which init
    /// the rest of the properties
	EventDetails(const std::string& bindName) : name(bindName) {
		Clear();
	}
    ///@property the name of the binding, for example "Window_Close"
	std::string name;
    ///@property int2 that represents the size of the window
	int2 size;
    ///@property int2 that represents the coordinates of the mouse in
	int2 mouse;
    ///@property int that represents the amount the mouse scrolled
	int mouseWheelDelta;
    ///@property a single Key Code
	int keyCode; // Single Key Code

    ///@brief a function to clear the EventDetails
	void Clear() {
		size = { 0,0 };
		mouse = { 0,0 };
		mouseWheelDelta = 0;
		keyCode = 0;
	}
};


/// a struct that holds all the event information
struct Binding {
    /// constructor
    /// \param l_name constant reference to std::string which is the name of the action
    /// this sets the m_name property, calls the EventDetails Constructor for m_details with the parameter, and sets c to 0
	Binding(const std::string& l_name) : m_name(l_name), m_details(l_name), c(0) {}

    /// adds To the Events vector
    /// \param l_type event type to bind and add to the Events vector
    /// \param l_info event info to bind and add to the Events vector -- this has a default value
	void BindEvent(EventType l_type, EventInfo l_info = EventInfo()) {
		m_events.emplace_back(l_type, l_info);
	}
    ///@property The Events vector
	Events m_events;
    ///@property the name of the binding
	std::string m_name;
    ///@property count of the events that are "happening"
    int c;
    ///@property the event details for this binding
	EventDetails m_details;
};


///@typedef defining a container for the bindings, we use std::unordered_map for our bindings
/// so that there will only be 1 binding per action,since its an associative container and the action
/// name string is the key for that container
using Bindings = std::unordered_map<std::string, Binding*>; // quick typedef

///@typedef defining a container for our callback functions
/// this data type is an unorder_map with string as its key and pointers to functions of the form [ void Func(EventDetails) ]
/// as the values, this way every action can have a function associated with it and call it
using CallbackContainer = std::unordered_map<std::string,
	std::function<void(EventDetails*)>>;

///@typedef defining a container of type unordered_map with StateType being its key and the callbackcontainer as the value
/// that way we can group callbacks by the statetype and thus every state can have whatever functions they want with whatever name
/// and not have them clash
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

/***
 * this class is the event manager,this holds ALL the information relating to events
 * aka all the previous data types we made
 * and this one deals with to logic of them
 * this holds the bindings, the callbacks and manages them currectly
 */
class EventManager
{
public:
    ///@publicsection
    ///@brief Constructor
	EventManager();
    ///@brief Destructor
	~EventManager();


    /// this function adds a binding pointer to the bindings vector
    /// \param l_binding a pointer to a Binding
    /// \return false if the binding already exists in the bindings vector and true otherwise
	bool AddBinding(Binding* l_binding);
    /// this functions search the bindings vector and removes it from storage
    /// \param l_name a string that is the name of the Action
    /// \return false if a binding with that name doesn't even exists and true otherwise
	bool RemoveBinding(std::string l_name);

    /// Setter
    /// \param l_focus are we in focuse
	void SetFocus(const bool& l_focus);

	// Needs to be defined in the header!
    /// a function that adds a callback to the container
    /// due to this function being a generic one, it MUST be defined in the header file
    /// \tparam T a place holder for the type of class that implements the function
    /// \param l_state the state that uses this callback
    /// \param l_name the name of the binding
    /// \param l_func a pointer to the function of the call back
    /// \param l_instance an instance of the T class that implements the function
    /// \return true if the CallBack was Added successfuly to the vector, false otherwise
	template<typename T>
	bool AddCallback(StateType l_state, const std::string& l_name,
		void(T::* l_func)(EventDetails*), T* l_instance)
	{
		auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first;
		auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
		return itr->second.emplace(l_name, temp).second;

	}
    /// the opposite of the AddCallBack function, this function Removes a callback from the vector,
    /// this search the vector for a callback of the state provided and with the name provided
    /// we define this in the header for symmetry reasons
    /// \param l_state the state of the callback to remove
    /// \param l_name the name of the callback to remove
    /// \return true if removed successfully and false otherwise
	bool RemoveCallback(StateType l_state, const std::string& l_name) {
		auto itr = m_callbacks.find(l_state);
		if (itr == m_callbacks.end()) { return false; }
		auto itr2 = itr->second.find(l_name);
		if (itr2 == itr->second.end()) { return false; }
		itr->second.erase(l_name);
		return true;
	}

    /// this function matches the SDL_Event with the corresponding EventType and loops thorugh all our bindings and updates
    /// the ones with right EventType and passes the Key Code
    /// \param l_event a reference to an SDL_Event object that was pulled from SDL_PollEvents
	void HandleEvent(SDL_Event& l_event);
    /// a function to update the Containers
	void Update();

    /// a function to get the Mouse Position relative to the window
    /// \param l_wind a pointer to a RenderWindow object
    /// \return an int2 with the coordinates of the mouse relative to the window
	int2 GetMousePosition(RenderWindow* l_wind = nullptr) {
		if (l_wind) {
			return l_wind->GetMousePosition();
		}
		int2 temp;
		SDL_GetGlobalMouseState(&temp.x, &temp.y);
		return temp;
	}
    /// Setter
    /// \param l_type a constant reference to a StateType object, which is the type we want to change to
    void SetCurrentState(const StateType& l_type);

private:
    ///@privatesection
    /// a function that loads our bindings from our keys.cfg file
	void LoadBindings();

    ///@property our bindings
	Bindings m_bindings;
    ///@property our callbacks
	Callbacks m_callbacks;
    ///@property a boolean that represents if our window is in focus, this way if our user is in a different tab, their key presses wont
    /// be applied
	bool m_hasFocus;
    ///@property the type of state that is currently active
	StateType m_currentState;
};

