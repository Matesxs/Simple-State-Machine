#include "state_machine.h"

#include <chrono>

namespace StateMachine
{
	void Machine::Init()
	{
		Reset(false);
	}

	void Machine::Add(EventTransition eventTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(&eventTransitions[i]);
	}

	void Machine::Add(GlobalEventTransition globalEventTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(&globalEventTransitions[i]);
	}

	void Machine::Add(ConditionalTransition conditionalTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(&conditionalTransitions[i]);
	}

	void Machine::Add(TimedTransition timedTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(&timedTransitions[i]);
	}

	void Machine::Add(EventTransition* eventTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(eventTransitions[i]);
	}

	void Machine::Add(GlobalEventTransition* globalEventTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(globalEventTransitions[i]);
	}

	void Machine::Add(ConditionalTransition* conditionalTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(conditionalTransitions[i]);
	}

	void Machine::Add(TimedTransition* timedTransitions[], size_t size)
	{
		for (size_t i = 0; i < size; i++)
			Add(timedTransitions[i]);
	}

	void Machine::Add(EventTransition* eventTransition)
	{
		if (eventTransition->m_from != nullptr &&
			eventTransition->m_to != nullptr &&
			!eventTransition->m_from->m_final)
		{
			m_eventTransitions.Add(eventTransition);
		}
	}

	void Machine::Add(GlobalEventTransition* globalEventTransition)
	{
		if (globalEventTransition->m_to != nullptr)
			m_globalEventTransitions.Add(globalEventTransition);
	}

	void Machine::Add(ConditionalTransition* conditionalTransition)
	{
		if (conditionalTransition->m_from != nullptr &&
			(conditionalTransition->m_to != nullptr || conditionalTransition->m_toNeg != nullptr) &&
			!conditionalTransition->m_from->m_final &&
			conditionalTransition->m_conditionCheck != nullptr)
		{
			m_conditionalTransitions.Add(conditionalTransition);
		}
	}

	void Machine::Add(TimedTransition* timedTransition)
	{
		if (timedTransition->m_from != nullptr &&
			timedTransition->m_to != nullptr &&
			!timedTransition->m_from->m_final)
		{
			m_timedTransitions.Add(timedTransition);
		}
	}

	void Machine::Run()
	{
		if (IsFinished()) return;

		for (auto& transition : m_globalEventTransitions)
		{
			if (transition->m_triggered &&
				transition->m_to != m_currentState)
			{
				Transition(transition->m_to);
				return;
			}
		}

		for (auto& transition : m_eventTransitions)
		{
			if (transition->m_triggered &&
				transition->m_from != m_currentState &&
				transition->m_to != m_currentState)
			{
				Transition(transition->m_to);
				return;
			}
		}

		for (auto& transition : m_conditionalTransitions)
		{
			if (transition->m_from != m_currentState) continue;

			if (transition->m_conditionCheck())
			{
				if (transition->m_to == nullptr) continue;

				Transition(transition->m_to);
				return;
			}
			else
			{
				if (transition->m_toNeg == nullptr) continue;
				
				Transition(transition->m_toNeg);
				return;
			}
		}

		uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		for (auto& transition : m_timedTransitions)
		{
			if (transition->m_from == m_currentState && currentTime - transition->m_start > transition->m_interval)
			{
				Transition(transition->m_to);
				return;
			}
		}

		// Not transitioned
		if (m_onStay != nullptr)
			m_onStay(m_currentState);

		if (m_currentState->m_onStay != nullptr)
			m_currentState->m_onStay();
	}

	void Machine::RestartState()
	{
		if (m_currentState == nullptr) return;
		HandleTransitionCallbacks();
	}

	void Machine::Stepback()
	{
		if (m_prevState == nullptr) return;

		m_currentState = m_prevState;
		m_prevState = nullptr;

		HandleTransitionCallbacks();
	}

	void Machine::Reset(bool callbacks /*= true*/)
	{
		m_currentState = m_initialState;
		m_prevState = nullptr;

		if (callbacks)
			HandleTransitionCallbacks();
	}

	void Machine::EventTrigger(size_t eventID)
	{
		for (auto& eventTransition : m_globalEventTransitions)
		{
			if (eventTransition->m_eventID == eventID)
			{
				eventTransition->m_triggered = true;
				break;
			}
		}

		for (auto& eventTransition : m_eventTransitions)
		{
			if (eventTransition->m_eventID == eventID && eventTransition->m_from == m_currentState)
			{
				eventTransition->m_triggered = true;
				break;
			}
		}
	}

	void Machine::SetTimedTransitionsStart()
	{
		uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		for (auto& transition : m_timedTransitions)
		{
			if (transition->m_from == m_currentState)
				transition->m_start = currentTime;
		}
	}

	void Machine::Transition(const State* newState)
	{
		m_prevState = m_currentState;
		m_currentState = newState;

		HandleTransitionCallbacks();
	}

	void Machine::HandleTransitionCallbacks()
	{
		// Clear triggered events
		for (auto& transition : m_eventTransitions)
			transition->m_triggered = false;

		// Clear triggered events
		for (auto& transition : m_globalEventTransitions)
			transition->m_triggered = false;

		if (m_onTransition != nullptr)
			m_onTransition(m_prevState, m_currentState);

		if (m_onExit != nullptr)
			m_onExit(m_prevState);

		if (m_prevState != nullptr && m_prevState->m_onExit != nullptr)
			m_prevState->m_onExit();

		if (m_onEnter != nullptr)
			m_onEnter(m_currentState);

		if (m_currentState != nullptr && m_currentState->m_onEnter != nullptr)
			m_currentState->m_onEnter();

		if (m_onFinish != nullptr && IsFinished())
			m_onFinish(m_currentState);
		
		SetTimedTransitionsStart();
	}

}
