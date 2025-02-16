#include "backtracking.h"
#include "SimulatorStack.h"
#include <algorithm>

void BacktrackingSearch::StepSol(void)
{
	if (solved || timedOut)
		return;
	// get choices at the current level
	std::vector<unsigned int> choices;
	sim_stack->GetChoices(choices);

	if (choices.size() == 0)
	{
		// reached the end of the line
		AddEvaluation();
		// check time every 1000 evaluations
		int cost = sim_stack->GetCost();
		if (cost < bestVal)
		{
			bestVal = cost;
			bestSol = sim_stack->StateAsString();
		}
		if (cost == 0)
		{
			solved = true;
		}
		if ( evals%1000 == 0)
		{
			bool limited = LimitsExceeded();
			if (chatter)
			{
				std::cout << " time: " << solTime << " evals: " << evals << " best: " << bestVal << std::endl;
			}
			if (limited)
			{
				timedOut = true;
				return;
			}
		}
		return;
	}
	// run through the choices in random order
	std::shuffle(choices.begin(), choices.end(), randGen);

	for (unsigned int c : choices)
	{
		sim_stack->PushChoice(c);
		StepSol();
		sim_stack->PopChoice();
	}
}

int BacktrackingSearch::Solve(Simulator* sim, float timeOut, int maxEvals)
{
	SetLimits(timeOut, maxEvals);
	solved = false;
	maxTime = timeOut;
	timedOut = false;
	evaluations = 0;
	bestVal = 0x7FFFFFFF;
	sim_stack = new SimulatorStack(sim);
	// solve here
	float r = random();
	sim_stack->StartRun(0xFFFF*r);
	StepSol();

	delete sim_stack;

	return bestVal;
}

