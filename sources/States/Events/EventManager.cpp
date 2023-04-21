#include "EventManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

EventManager::EventManager() : m_hasFocus(true) {
	LoadBindings();
}
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

/// <summary>
/// handling sdl events
/// </summary>
/// <param name="l_event"></param>
void EventManager::HandleEvent(SDL_Event& l_event) {
	//TODO: add description
	for (auto& b_itr : m_bindings) {
		Binding* bind = b_itr.second;
		for (auto& e_itr : bind->m_events) {
			EventType sdlEvent = (EventType)l_event.type;
			if (e_itr.first != sdlEvent) { continue; }
			if (sdlEvent == EventType::KeyDown || sdlEvent == EventType::KeyUp) {
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
				switch (sdlEvent) {
					case EventType::MouseWheel :
						bind->m_details.mouseWheelDelta = l_event.wheel.preciseY;
						break;
					default:
						break;
				}
				++(bind->c);
			}
		}
	}
}

void EventManager::Update() {
	if (!m_hasFocus) return;

	for (auto& b_itr : m_bindings) {
		Binding* bind = b_itr.second;
		if (bind->m_events.size() == bind->c) {
			/*auto callItr = m_callbacks.find(bind->m_name);
			if (callItr != m_callbacks.end()) {
				callItr->second(&bind->m_details);
			}*/
			auto stateCallbacks = m_callbacks.find(m_currentState);
			auto otherCallbacks = m_callbacks.find(StateType(0));

			if (stateCallbacks != m_callbacks.end()) {
				auto callItr = stateCallbacks->second.find(bind->m_name);
				if (callItr != stateCallbacks->second.end()) {
					// pass in information about events
					callItr->second(&bind->m_details);
				}
			}

			if (otherCallbacks != m_callbacks.end()) {
				auto callItr = otherCallbacks->second.find(bind->m_name);
				if (callItr != otherCallbacks->second.end()) {
					// pass in information about events
					callItr->second(&bind->m_details);
				}
			}
		}
		bind->c = 0;
		bind->m_details.Clear();
	}
}
void EventManager::LoadBindings() {
	/*
	* TODO(add description)
	**/
	std::string delimiter = ":";
	std::ifstream bindings;
	bindings.open("data/keys.cfg");
	if (!bindings.is_open()) {
		std::cout << "! Failed Loading keys.cfg." << std::endl;
		return;
	}
	std::string line;
	while (std::getline(bindings, line))
	{
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		Binding* bind = new Binding(callbackName);
		while (!keystream.eof())
		{
			std::string keyval;
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos) {
				delete bind;
				bind = nullptr;
				break;
			}
			EventType type = EventType(stoi(keyval.substr(start, end - start)));
			int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			EventInfo eventInfo;
			eventInfo.m_code = code;
			bind->BindEvent(type, eventInfo);
		}
		if (!AddBinding((bind))) { delete bind; }
		bind = nullptr;
	}
	bindings.close();
}

void EventManager::SetCurrentState(const StateType &l_type) {
    m_currentState = l_type;
}
