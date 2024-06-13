#include "transition.h"

#include <chrono>

namespace StateMachine
{
	size_t AbstractTransition::_m_nextId = 0;

	EventTransition::EventTransition(size_t event_id, const State* from, const State* to)
	{
		Setup(event_id, from, to);
	}

	void EventTransition::Setup(size_t event_id, const State* from, const State* to)
	{
		m_id = _m_nextId++;
		m_eventID = event_id;
		m_from = from;
		m_to = to;
	}

	ConditionalTransition::ConditionalTransition(CallbackConditionCheck conditionCheck, const State* from, const State* to, const State* toNeg)
	{
		Setup(conditionCheck, from, to, toNeg);
	}

	void ConditionalTransition::Setup(CallbackConditionCheck conditionCheck, const State* from, const State* to, const State* toNeg)
	{
		m_id = _m_nextId++;
		m_conditionCheck = conditionCheck;
		m_from = from;
		m_to = to;
		m_toNeg = toNeg;
	}

	TimedTransition::TimedTransition(uint64_t interval, const State* from, const State* to)
	{
		Setup(interval, from, to);
	}

	void TimedTransition::Setup(uint64_t interval, const State* from, const State* to)
	{
		m_id = _m_nextId++;
		m_interval = interval;
		m_from = from;
		m_to = to;
	}

	void TimedTransition::Reset()
	{
		m_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	GlobalEventTransition::GlobalEventTransition(size_t event_id, const State* to)
	{
		Setup(event_id, to);
	}

	void GlobalEventTransition::Setup(size_t event_id, const State* to)
	{
		m_id = _m_nextId++;
		m_eventID = event_id;
		m_to = to;
	}
}
