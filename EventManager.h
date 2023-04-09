#pragma once
#include <SDL2/SDL_events.h>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include "linalg.h"
#include <functional>
#include "RenderWindow.h"
#include "BaseState.h"

using namespace linalg::aliases;


enum class EventType {
	KeyDown = SDL_KEYDOWN, //768
	KeyUp = SDL_KEYUP, // 769
	MButtonDown = SDL_MOUSEBUTTONDOWN, // 1025
	MButtonUp = SDL_MOUSEBUTTONUP, // 1026
	Closed = SDL_QUIT, // 256
	MouseWheel = SDL_MOUSEWHEEL //1027
};

struct EventInfo
{
	EventInfo() { m_code = 0; }
	EventInfo(int l_event) { m_code = l_event; }
	union
	{
		int m_code;
	};
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventDetails {
	EventDetails(const std::string& bindName) : name(bindName) {
		Clear();
	}
	std::string name;
	int2 size;
	uint32_t textEntered; // idk why uint32 
	int2 mouse;
	int mouseWheelDelta;
	int keyCode; // Single Key Code

	void Clear() {
		size = { 0,0 };
		textEntered = 0;
		mouse = { 0,0 };
		mouseWheelDelta = 0;
		keyCode = 0;
	}
};

struct Binding {
	Binding(const std::string& l_name) : m_name(l_name), m_details(l_name), c(0) {}

	void BindEvent(EventType l_type, EventInfo l_info = EventInfo()) {
		m_events.emplace_back(l_type, l_info);
	}

	Events m_events;
	std::string m_name;
	int c; // count of the events that are "happening"
	EventDetails m_details;
};

using Bindings = std::unordered_map<std::string, Binding*>; // quick typedef

using CallbackContainer = std::unordered_map<std::string,
	std::function<void(EventDetails*)>>;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;


class EventManager
{
public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding* l_binding);
	bool RemoveBinding(std::string l_name);

	void SetFocus(const bool& l_focus);

	// Needs to be defined in the header!
	template<typename T>
	bool AddCallback(StateType l_state, const std::string& l_name,
		void(T::* l_func)(EventDetails*), T* l_instance)
	{
		auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first;
		auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
		return itr->second.emplace(l_name, temp).second;

	}
	bool RemoveCallback(StateType l_state, const std::string& l_name) {
		auto itr = m_callbacks.find(l_state);
		if (itr == m_callbacks.end()) { return false; }
		auto itr2 = itr->second.find(l_name);
		if (itr2 == itr->second.end()) { return false; }
		itr->second.erase(l_name);
		return true;
	}

	void HandleEvent(SDL_Event& l_event);
	void Update();

	int2 GetMousePosition(RenderWindow* l_wind = nullptr) {
		if (l_wind) {
			return l_wind->GetMousePosition();
		}
		int2 temp;
		SDL_GetGlobalMouseState(&temp.x, &temp.y);
		return temp;
	}
    void SetCurrentState(const StateType& l_type);

private:
	void LoadBindings();

	Bindings m_bindings;
	Callbacks m_callbacks;
	bool m_hasFocus;
	StateType m_currentState;
};

