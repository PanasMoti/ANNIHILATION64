#include "StateManager.h"
#include "Main Menu/State_MainMenu.h"
#include "Load Game/State_LoadGame.h"
#include <iostream>
#include "Map Editor/State_MapEdit.h"
#include "Gameplay/State_GamePlay.h"
#include "Game Over/State_GameOver.h"
StateManager::StateManager(SharedContext* l_shared) : m_shared(l_shared)
{
    RegisterState<State_MainMenu>(StateType::MainMenu);
    RegisterState<State_LoadGame>(StateType::LoadGame);
    RegisterState<State_MapEdit>(StateType::MapEdit);
    RegisterState<State_GamePlay>(StateType::GamePlay);
    RegisterState<State_GameOver>(StateType::GameOver);

}

StateManager::~StateManager() {
	for (auto& itr : m_states) {
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Draw() {
	if (m_states.empty()) { return; }
	m_states.back().second->Draw();
}



void StateManager::Update(float dt) {
	if (m_states.empty()) { return; }
	m_states.back().second->Update(dt);
}

SharedContext* StateManager::GetContext() {
	return m_shared;
}

bool StateManager::HasState(const StateType& l_type) {
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
		if (itr->first == l_type) {
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
			if (removed == m_toRemove.end()) { return true; }
			return false;
		}
	}
	return false;
}

void StateManager::Remove(const StateType& l_type) {
	m_toRemove.push_back(l_type);
}

void StateManager::ProcessRequests()
{
	while (m_toRemove.begin() != m_toRemove.end()) {
		RemoveState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& l_type) {
	m_shared->eventMgr->SetCurrentState(l_type);
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
		if (itr->first == l_type) {
			m_states.back().second->Deactivate();
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			m_states.erase(itr);
			m_states.emplace_back(tmp_type, tmp_state);
			tmp_state->Activate();
			return;
		}
	}
    // state with l_type wasn't found.
    if(!m_states.empty()) {m_states.back().second->Deactivate();}
    CreateState(l_type);
    m_states.back().second->Activate();
}

void StateManager::CreateState(const StateType& l_type) {
	auto newState = m_stateFactory.find(l_type);
	if (newState == m_stateFactory.end()) { return; }
	BaseState* state = newState->second();
	m_states.emplace_back(l_type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& l_type) {
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
		if (itr->first == l_type) {
			itr->second->OnDestroy();
			delete itr->second;
			m_states.erase(itr);
			return;
		}
	}
}