#include "state.h"

namespace StateMachine
{
	size_t State::_m_nextId = 0;

	State::State(std::string name, bool final /*= false*/, CallbackFunction onEnter /*= nullptr*/, CallbackFunction onStay /*= nullptr*/, CallbackFunction onExit /*= nullptr*/)
	{
		Setup(name, final, onEnter, onStay, onExit);
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

	bool operator==(const State* a, const State& b)
	{
		return a->getID() == b.getID();
	}

	bool operator==(const State& a, const State* b)
	{
		return a.getID() == b->getID();
	}
}