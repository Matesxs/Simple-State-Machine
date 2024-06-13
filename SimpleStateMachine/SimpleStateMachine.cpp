#include <cstdio>
#include <chrono>
#include <thread>
#include <Windows.h>

#include "state_machine.h"

StateMachine::State states[] = {
    { "init", false },
    { "1", false },
    { "2", false },
    { "final", true },
    { "fail", true },
};

StateMachine::ConditionalTransition condititionalTransitions[] = {
    { []() { return true; } , &states[0], &states[1], &states[2]},
};

StateMachine::EventTransition eventTransitions[] = {
    { 1, &states[0], &states[3] },
    { 1, &states[1], &states[3] },
    { 1, &states[2], &states[3] },
};

StateMachine::TimedTransition timedTransitions[] = {
    { 10000, &states[1], &states[4] },
    { 10000, &states[2], &states[4] },
};

int main()
{
    StateMachine::Machine stateMachine;

    stateMachine.SetTransitionCallback([](const StateMachine::State* from, const StateMachine::State* to)
        {
            printf("Transitioning from %s to %s\r\n", from->getName().c_str(), to->getName().c_str()); 
        });
    stateMachine.SetFinishedCallback([](const StateMachine::State*)
        {
            printf("State machine finished\r\n");
        });
    stateMachine.SetOnEnterCallback([](const StateMachine::State* state)
        {
            printf("On Enter %s\r\n", state->getName().c_str());
        });
    stateMachine.SetOnExitCallback([](const StateMachine::State* state)
        {
            printf("On Exit %s\r\n", state->getName().c_str());
        });

    stateMachine.SetInitialState(&states[0]);
    stateMachine.Add(condititionalTransitions, sizeof(condititionalTransitions) / sizeof(StateMachine::ConditionalTransition));
    stateMachine.Add(eventTransitions, sizeof(eventTransitions) / sizeof(StateMachine::EventTransition));
    stateMachine.Add(timedTransitions, sizeof(timedTransitions) / sizeof(StateMachine::TimedTransition));

    stateMachine.Init();

	while (!stateMachine.IsFinished())
	{
		if (GetKeyState('A') & 0x8000)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

			while (GetKeyState('A') & 0x8000)
				std::this_thread::sleep_for(std::chrono::milliseconds(5));

			printf("Triggering event 1\r\n");
			stateMachine.EventTrigger(1);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		stateMachine.Run();
	}

    return 0;
}
