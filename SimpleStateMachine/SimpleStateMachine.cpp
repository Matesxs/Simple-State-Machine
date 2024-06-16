#include <cstdio>
#include <chrono>
#include <thread>
#include <Windows.h>

#include "state_machine.h"

size_t loopCounter = 0;

void loopCountCallback()
{
	printf("Loop count increment\r\n");
	loopCounter++;
}

bool finalTransitionCheckCallback()
{
	if (loopCounter >= 5) return true;
	return false;
}

StateMachine::State states[] = {
	{ "1", false, []() { printf("Starting new loooooooop\r\n"); } },
	{ "2", false },
	{ "3", false, []() { printf("Finally in 3 :D\r\n"); }, []() { printf("Stay in state 3\r\n"); std::this_thread::sleep_for(std::chrono::milliseconds(200)); }},
	{ "4", false },
	{ "5", false, loopCountCallback },
	{ "f", true },
	{ "ff", true },
};

StateMachine::TimedTransition timedTransitions[] = {
	{ 5000, &states[0], &states[1] },
	{ 2000, &states[1], &states[2] },
	{ 1000, &states[2], &states[3] },
	{ 4000, &states[3], &states[4] },
	{ 8000, &states[4], &states[0] },
};

StateMachine::ConditionalTransition condititionalTransitions[] = {
	{ finalTransitionCheckCallback, &states[4], &states[5] },
};

StateMachine::EventTransition eventTransitions[] = {
	{ 1, &states[0], &states[5] },
	{ 1, &states[1], &states[5] },
	{ 1, &states[2], &states[5] },
	{ 1, &states[3], &states[5] },
	{ 1, &states[4], &states[5] },
	{ 2, nullptr, &states[2] },
	{ 3, nullptr, &states[6] },
};

int main()
{
	/*StateMachine::Machine stateMachine;

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
	stateMachine.Add(timedTransitions, sizeof(timedTransitions) / sizeof(StateMachine::TimedTransition));
	stateMachine.Add(condititionalTransitions, sizeof(condititionalTransitions) / sizeof(StateMachine::ConditionalTransition));
	stateMachine.Add(eventTransitions, sizeof(eventTransitions) / sizeof(StateMachine::EventTransition));

	stateMachine.Init();

	for (size_t i = 0; i < 2; i++)
	{
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

			if (GetKeyState('B') & 0x8000)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(50));

				while (GetKeyState('B') & 0x8000)
					std::this_thread::sleep_for(std::chrono::milliseconds(5));

				printf("Triggering event 2\r\n");
				stateMachine.EventTrigger(2);
			}

			if (GetKeyState('C') & 0x8000)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(50));

				while (GetKeyState('C') & 0x8000)
					std::this_thread::sleep_for(std::chrono::milliseconds(5));

				printf("Triggering event 3\r\n");
				stateMachine.EventTrigger(3);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			stateMachine.Run();
		}

		stateMachine.Reset();
	}*/

	StateMachine::State state1("test2");
	StateMachine::State state2("test2");

	StateMachine::State* state1Ptr = &state1;
	StateMachine::State* state2Ptr = &state2;

	printf("Objects compare:\r\n");
	printf("Same object: %d\r\n", state1 == state1);
	printf("Different object: %d\r\n", state1 == state2);

	printf("\r\nCompare pointers:\r\n");
	printf("Same pointer: %d\r\n", state1Ptr == state1Ptr);
	printf("Different pointer: %d\r\n", state1Ptr == state2Ptr);

	printf("\r\nMixed compare:\r\n");
	printf("Object and pointer of same object: %d\r\n", state1 == state1Ptr);
	printf("Pointer and object of the same object: %d\r\n", state1Ptr == state1);
	printf("Object and pointer of different object: %d\r\n", state1 == state2Ptr);
	printf("Pointer and object of the different object: %d\r\n", state1Ptr == state2);

	return 0;
}
