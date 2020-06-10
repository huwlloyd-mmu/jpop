#include "RandomSearch.h"

int RandomSearch::Solve(Simulator *sim, float maxTime, int maxEvals)
{
	SetLimits(maxTime, maxEvals);
	bool solved = false;
	int bestVal = 10000000;

	std::vector<unsigned int> choices;
	int iter = 0;
	while (!solved)
	{
		bool done = false;
		sim->StartRun(0);
		while (!done)
		{
			sim->GetChoices(choices);
			if (choices.size() == 0)
				done = true;
			else if (choices.size() == 1)
				sim->SetChoice(choices[0]);
			else
			{
				std::uniform_int_distribution<int> dist(0, choices.size() - 1);
				int choice = dist(randGen);
				sim->SetChoice(choices[choice]);
			}
		}
		int cost = sim->GetCost();
		if (cost < bestVal)
		{
			bestVal = cost;
			bestSol = sim->StateAsString();
		}
		AddEvaluation();
		if (bestVal == 0)
		{
			solved = true;
			solTime = solutionTimer.Elapsed();
		}
		if ((iter % 1000) == 0)
		{
			bool limited = LimitsExceeded();
			if (chatter)
			{
				std::cout << "iter: " << iter << " time: " << solTime << " evals: " << evals << " best: " << bestVal << std::endl;
			}
			if (limited)
			{
				break;
			}
		}
		++iter;
	}
	return bestVal;
}
