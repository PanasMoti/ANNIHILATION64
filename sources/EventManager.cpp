#include "EventManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

/// a simple constructor
EventManager::EventManager() : m_hasFocus(true) {
	LoadBindings();
}
/// a destructor, since our bindings uses pointers aka on heap variables, we need to free them after our use
/// this function goes through all of the bindings and deletes the pointer to Binding, then it sets it to nullptr so we
/// wont get access error
EventManager::~EventManager() {
	for (auto& itr : m_bindings) {
		delete itr.second;
		itr.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding* l_binding) {
	if (m_bindings.find(l_binding->m_name) != m_bindings.end()) {
		return false;
	}
	return m_bindings.emplace(l_binding->m_name, l_binding).second;
}
bool EventManager::RemoveBinding(std::string l_name) {
	auto itr = m_bindings.find(l_name);
	if (itr == m_bindings.end()) return false;
	delete itr->second;
	m_bindings.erase(itr);
	return true;
}

void EventManager::SetFocus(const bool& l_focus)
{
	m_hasFocus = l_focus;
}


void EventManager::HandleEvent(SDL_Event& l_event) {
	/***
	 * this function handles the which event goes where and which callback/binding will be called appropriately
	 */
	for (auto& b_itr : m_bindings) {
        // loop through every binding
		Binding* bind = b_itr.second; // a pointer for convince
		for (auto& e_itr : bind->m_events) {
            // loop through every event in the binding pointer
			EventType sdlEvent = static_cast<EventType>(l_event.type); // casting the event from an SDL_Event to EventType
			if (e_itr.first != sdlEvent) { continue; } // if the event doesn't exist in our enum, there is no reason for further checking, we can skip to the processing part
			if (sdlEvent == EventType::KeyDown || sdlEvent == EventType::KeyUp) {
                // if it's a key related event, store the keycode
				if (e_itr.second.m_code == l_event.key.keysym.sym) {
					// matching event / keystroke.
					// increase amount.
					if (bind->m_details.keyCode != -1) {
						bind->m_details.keyCode = e_itr.second.m_code;
					}
					++(bind->c);
					break;
				}
			}
			else if (sdlEvent == EventType::MButtonDown || sdlEvent == EventType::MButtonUp) {
                // if this is mouse_event , store the mouse position as well

				// matching event / keystroke.
				// increase amount
				bind->m_details.mouse.x = l_event.button.x;
				bind->m_details.mouse.y = l_event.button.y;
				if (bind->m_details.keyCode != -1) {
					bind->m_details.keyCode = e_itr.second.m_code;
				}
				++(bind->c);
				break;
			}
			else {
				// no need for additional checking
                // processing events that aren't convertible
				switch (sdlEvent) {
					case EventType::MouseWheel :
                        // adding the mouse delta
						bind->m_details.mouseWheelDelta = l_event.wheel.preciseY;
						break;
					default:
                        // reserved for future expansion
						break;
				}
				++(bind->c); // increase the amount of events happenings
			}
		}
	}
}

void EventManager::Update() {
	if (!m_hasFocus) return; // if the user is in a different window -> do not process any event/keystroke

    //loop through all of our bindings
	for (auto& b_itr : m_bindings) {
		Binding* bind = b_itr.second; // for shorter writing
		if (bind->m_events.size() == bind->c) {
		    // if the amounts of events in the event container matches to the amount that happenings
            // if that's the case we simply locate our callback in the callbacks container and invoke it
			auto stateCallbacks = m_callbacks.find(m_currentState); // the callback container corresponding to the current state
			auto otherCallbacks = m_callbacks.find(StateType(0)); // StateType(0) isn't a valid state since our enum starts from 1,this means a global callback which will be executed regardless of the current state
			// in our code the only time we use this is for the Window_close binding

            if (stateCallbacks != m_callbacks.end()) { // if callbacks of this state exists at all
				auto callItr = stateCallbacks->second.find(bind->m_name); // find the correct binding
				if (callItr != stateCallbacks->second.end()) { // checking if the binding exists at all
					// pass in information about events
					callItr->second(&bind->m_details);
				}
			}

			if (otherCallbacks != m_callbacks.end()) { // if global callbacks exists at all
				auto callItr = otherCallbacks->second.find(bind->m_name); // find the correct binding
				if (callItr != otherCallbacks->second.end()) { // checking if the binding exists at all
					// pass in information about events
					callItr->second(&bind->m_details);
				}
			}
        }
        // all actions were executed so there are no longer any actions happenings
		bind->c = 0;
		bind->m_details.Clear();
	}
}
void EventManager::LoadBindings() {
	/*
	* this function loads all of our bindings from the file "keys.cfg" which exists in the data directory
	 * this function deals with string manipulation as in the file bindings are written as the following:
	 *  [BINDING NAME] [EVENT TYPE]:[KEY CODE]
	 *  each line represents a single binding
	**/
	std::string delimiter = ":";
	std::ifstream bindings;
	bindings.open("data/keys.cfg");
	if (!bindings.is_open()) {
        // checking if we managed to open the file at all
		std::cerr << "! Failed Loading keys.cfg." << std::endl;
		return;
	}
	std::string line;
	while (std::getline(bindings, line)) // go through the file line by line
	{
		std::stringstream keystream(line); // stringstream is a c++ data structure that allows performing insertions,extractions and other operators
        // it can be used to parse input and converting strings to numbers and vice versa
        // the name of the callback
		std::string callbackName; // this string holds the [BINDING NAME] part of the line
		keystream >> callbackName; // we insert it from the file to this variable
		Binding* bind = new Binding(callbackName); // Create a new binding
		while (!keystream.eof()) // while we aren't in the end of the string stream
		{
            // the string stream automaticly breaks the string down by spaces
			std::string keyval; // a string that will hold the [EVENTTYPE]:[KEYCODE] part of the line
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter); // we find where is the : character -> aka where the string splits into [EVENT TYPE] and
            // [KEY CODE]
			if (end == std::string::npos) { // if there isn't the : character -> the line is invalid
                // remove the bind
				delete bind;
				bind = nullptr;
				break; // skip to next line
			}
            // take the substring of everything before the delimiter : and convert it to StateType and store it in a variable
			EventType type = EventType(stoi(keyval.substr(start, end - start))); // converting to int and then to EventType
            // take the substring of everything AFTER the delimiter : and convert it to int and store it as keycode
			int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			EventInfo eventInfo; // create a new instance of EventInfo
			eventInfo.m_code = code;
			bind->BindEvent(type, eventInfo); // bind the event
		}
		if (!AddBinding((bind))) { delete bind; } // Add the binding to the vector, if the binding already exists , delete the new binding we created here
		bind = nullptr;
	} // while getline
	bindings.close(); // close the file after finishing
}

void EventManager::SetCurrentState(const StateType &l_type) {
    m_currentState = l_type;
}
