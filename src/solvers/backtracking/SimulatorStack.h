#pragma once
#include <vector>
#include "../../simulator.h"

class SimulatorStack
{
	// wrapper for a simulator which adds a stack of states: choices can be pushed and popped off the stack
	// and the SimulatorStack object keeps the internal simulator state correct by replaying choices 

	Simulator* sim;
	std::vector<unsigned int> choice_stack;
	unsigned int startVal;
public:
	SimulatorStack(Simulator* sim) : sim(sim) {}
	void StartRun(unsigned int start)
	{
		startVal = start;
		sim->StartRun(startVal);
		choice_stack.clear();
	}

	void GetChoices(std::vector<unsigned int>& choices) { sim->GetChoices(choices); }
	void PushChoice(unsigned int choice)
	{
		choice_stack.push_back(choice);
		sim->SetChoice(choice);
	}
	void PopChoice(void)
	{
		choice_stack.pop_back();
		sim->StartRun(startVal);
		for (unsigned int choice : choice_stack)
		{
			sim->SetChoice(choice);
		}
	}
	int GetCost(void) { return sim->GetCost(); }
	std::string StateAsString() { return sim->StateAsString(); }
};