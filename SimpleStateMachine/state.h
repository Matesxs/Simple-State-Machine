#pragma once

#include <string>
#include <functional>

namespace StateMachine
{
	typedef std::function<void(void)> CallbackFunction;

	class State
	{
		friend class Machine;

	public:
		State() = default;
		State(std::string name,
			bool final = false,
			CallbackFunction onEnter = nullptr,
			CallbackFunction onStay = nullptr,
			CallbackFunction onExit = nullptr);

		void Setup(std::string name,
			bool final = false,
			CallbackFunction onEnter = nullptr,
			CallbackFunction onStay = nullptr,
			CallbackFunction onExit = nullptr);

		size_t getID() const { return m_id; }
		std::string getName() const { return m_name; }

	public:
		bool operator==(const State& other) const { return m_id == other.m_id; }

	private:
		size_t m_id = 0;
		static size_t _m_nextId;

		std::string m_name = "";
		bool m_final = false;
		CallbackFunction m_onEnter = nullptr;
		CallbackFunction m_onStay = nullptr;
		CallbackFunction m_onExit = nullptr;
	};

	bool operator==(const State* a, const State& b);
	bool operator==(const State& a, const State* b);
}