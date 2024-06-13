#pragma once

#include <string>
#include <functional>

#include "state.h"

namespace StateMachine
{
	typedef std::function<bool(void)> CallbackConditionCheck;

	class AbstractTransition
	{
		friend class Machine;

	public:
		AbstractTransition() = default;
		virtual ~AbstractTransition() = default;

		size_t GetID() const { return m_id; }

	protected:
		size_t m_id = 0;
		static size_t _m_nextId;

		const State* m_from = nullptr;
		const State* m_to = nullptr;
	};

	class EventTransition : public AbstractTransition
	{
		friend class Machine;

	public:
		EventTransition() = default;
		EventTransition(size_t event_id,
			const State* from,
			const State* to);

		void Setup(size_t event_id,
			const State* from,
			const State* to);

	private:
		size_t m_eventID = 0;
		bool m_triggered = false;
	};

	class GlobalEventTransition : public AbstractTransition
	{
		friend class Machine;

	public:
		GlobalEventTransition() = default;
		GlobalEventTransition(size_t event_id,
			const State* to);

		void Setup(size_t event_id,
			const State* to);

	private:
		size_t m_eventID = 0;
		bool m_triggered = false;
	};

	class ConditionalTransition : public AbstractTransition
	{
		friend class Machine;

	public:
		ConditionalTransition() = default;
		ConditionalTransition(CallbackConditionCheck conditionCheck,
			const State* from,
			const State* to = nullptr,
			const State* toNeg = nullptr);

		void Setup(CallbackConditionCheck conditionCheck,
			const State* from,
			const State* to = nullptr,
			const State* toNeg = nullptr);

	private:
		const State* m_toNeg = nullptr;
		CallbackConditionCheck m_conditionCheck = nullptr;
	};

	class TimedTransition : public AbstractTransition
	{
		friend class Machine;

	public:
		TimedTransition() = default;
		TimedTransition(uint64_t interval,
			const State* from,
			const State* to);

		void Setup(uint64_t interval,
			const State* from,
			const State* to);

		void Reset();

	private:
		uint64_t m_start = 0;
		uint64_t m_interval = 0;
	};
}