#include "simulatedannealing.h"
#include <iostream>
#include <math.h>

IntChromosome* SimulatedAnnealing::CreateNeighbour(Chromosome *current, Simulator *sim)
{
	IntChromosome* neighbour = new IntChromosome(this, chromosomeLength);
	neighbour->SetSimulator(sim);
	neighbour->InheritSolution(current);
	neighbour->MutateOne();
	neighbour->InitSolution();
	AddEvaluation();
	if (neighbour->GetCost() == 0)
		SetSolved();
	return neighbour;
}

bool SimulatedAnnealing::ShouldAccept(Chromosome* candidate, Chromosome* current, float temperature)
{
	if (candidate->GetCost() < current->GetCost())
	{
		return true;
	}

	return (exp((current->GetCost() - candidate->GetCost()) / temperature) > random());
}

int SimulatedAnnealing::Solve(Simulator *sim, float maxTime, int maxEvals)
{
	SetLimits(maxTime, maxEvals);
	int iter = 0;
	bool solved = false;

	currentSol->SetSimulator(sim);
	currentSol->InitSolution();
	AddEvaluation();

	int bestVal = currentSol->GetCost();
	float temperature = maxTemp;

	while (!solved)
	{
		IntChromosome* candidate = CreateNeighbour(currentSol, sim);
		temperature *= tempChange;
		if (ShouldAccept(candidate, currentSol, temperature))
		{
			delete currentSol;
			currentSol = candidate;
		}
		else
		{
			delete candidate;
		}

		if (currentSol->GetCost() < bestVal)
		{
			bestVal = currentSol->GetCost();
			bestSol = currentSol->GetSolution();
		}

		if ((iter % 100) == 0)
		{
			bool limited = LimitsExceeded();
			if (chatter)
			{
				std::cout << "iter: " << iter << " time: " << solTime << " evals: " << evals << " temp " << temperature << " best: " << bestVal << std::endl;
			}
			if (limited)
			{
				break;
			}
		}

		if (bestVal == 0)
		{
			solved = true;
		}

		++iter;
	}
	return bestVal;
}