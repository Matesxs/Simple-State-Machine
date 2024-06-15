#pragma once

#include <string>
#include <functional>

#include "state.h"
#include "transition.h"
#include "linked_list.h"

namespace StateMachine
{
	typedef std::function<void(const State*)> StateCallback;
	typedef std::function<void(const State*, const State*)> TransitionCallback;

	class Machine
	{
	public:
		Machine() = default;
		Machine(const State* initialState) :
			m_initialState(initialState)
		{}

		void Init();

		void Add(EventTransition eventTransitions[], size_t size);
		void Add(ConditionalTransition conditionalTransitions[], size_t size);
		void Add(TimedTransition* timedTransition);
		void Add(EventTransition* eventTransition);
		void Add(ConditionalTransition* conditionalTransition);
		void Add(TimedTransition timedTransitions[], size_t size);
		void Add(EventTransition* eventTransitions[], size_t size);
		void Add(ConditionalTransition* conditionalTransitions[], size_t size);
		void Add(TimedTransition* timedTransitions[], size_t size);

		void SetInitialState(const State* state) { m_initialState = state; }

		void SetTransitionCallback(TransitionCallback func) { m_onTransition = func; }
		void SetFinishedCallback(StateCallback func) { m_onFinish = func; }

		void SetOnEnterCallback(StateCallback func) { m_onEnter = func; }
		void SetOnStayCallback(StateCallback func) { m_onStay = func; }
		void SetOnExitCallback(StateCallback func) { m_onExit = func; }

		bool IsInitial() const { if (m_currentState == nullptr) return true; return m_currentState == m_initialState; }
		bool IsFinished() const { if (m_currentState == nullptr) return true; return m_currentState->m_final; }

		const State* GetState() const { return m_currentState; }
		const State* GetPrevState() const { return m_prevState; }

		void Run();
		void RestartState();
		void Stepback();
		void Reset(bool callbacks = false);
		void EventTrigger(size_t eventID);

	private:
		void SetTimedTransitionsStart();
		void Transition(const State* newState);
		void HandleTransitionCallbacks();

	private:
		const State* m_currentState = nullptr;
		const State* m_prevState = nullptr;
		const State* m_initialState = nullptr;

		LinkedList<EventTransition*> m_eventTransitions;
		LinkedList<ConditionalTransition*> m_conditionalTransitions;
		LinkedList<TimedTransition*> m_timedTransitions;

		StateCallback m_onFinish = nullptr;
		TransitionCallback m_onTransition = nullptr;

		StateCallback m_onEnter = nullptr;
		StateCallback m_onStay = nullptr;
		StateCallback m_onExit = nullptr;
	};
}
