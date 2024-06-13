#include "state.h"

namespace StateMachine
{
	size_t State::_m_nextId = 0;

	State::State(std::string name, bool final /*= false*/, CallbackFunction onEnter /*= nullptr*/, CallbackFunction onStay /*= nullptr*/, CallbackFunction onExit /*= nullptr*/)
	{
		Setup(name, final, onEnter, m_onStay, onExit);
	}

	void State::Setup(std::string name, bool final, CallbackFunction onEnter, CallbackFunction onStay, CallbackFunction onExit)
	{
		m_id = _m_nextId++;
		m_name = name;
		m_final = final;
		m_onEnter = onEnter;
		m_onStay = onStay;
		m_onExit = onExit;
	}
}