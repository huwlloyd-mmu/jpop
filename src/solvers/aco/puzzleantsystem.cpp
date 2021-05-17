#include "puzzleantsystem.h"
#include <iostream>

void PuzzleAntSystem::ClearPheromone()
{
	pher.clear();
}

float PuzzleAntSystem::PherAdd( int cost )
{
	return 1.0f/ cost;
}

void PuzzleAntSystem::UpdatePheromone()
{
	for (int i : bestChoices)
	{
		float pherVal = Pher(i);
		pherVal = pherVal*(1.0f-rho) + rho*bestPher;
		SetPher(i, pherVal);
	}
}

void PuzzleAntSystem::LocalPheromoneUpdate( unsigned int iChoice )
{
	float pherVal = Pher(iChoice)*0.9f + pher0*0.1f;
	SetPher( iChoice, pherVal );
}

int PuzzleAntSystem::Solve( Simulator *sim, float maxTime, int maxEvals )
{
	SetLimits(maxTime, maxEvals);
	int iter = 0;
	bool solved = false;
	bestPher = 0.0f;
	int curBestAnt = 0;

	// give each ant a copy of the simulator
	for (auto a : antList)
	{
		a->SetSimulator(sim);
	}
	
	while (!solved)
	{
		for (auto a : antList)
		{
			a->InitSolution();
		}
		bool allDone = false;
		while (!allDone)
		{
			// step each ant in turn
			allDone = true;
			for (auto a : antList)
			{
				if ( !a->StepSolution() )
					allDone = false;
			}
		}
		// update pheromone
		int iBest = 0;
		bestVal = 1000000;
		for (unsigned int i = 0; i < antList.size(); i++)
		{
			if (antList[i]->GetCost() < bestVal)
			{
				bestVal = antList[i]->GetCost();
				iBest = i;
			}
			AddEvaluation();
			if (antList[i]->GetCost() == 0)
				SetSolved();
		}

		float pherToAdd = PherAdd(bestVal);

		if (pherToAdd > bestPher )
		{
			// new best
			bestSol = antList[iBest]->GetSolution();
			bestChoices.clear();
			for ( int i : antList[iBest]->GetChoices() )
				bestChoices.push_back(i);
			bestPher = pherToAdd;
			curBestAnt = bestVal;
			if (bestVal == 0)
			{
				solved = true;
				solTime = solutionTimer.Elapsed();
			}
		}
		UpdatePheromone();
		bestPher *= (1.0f - bestEvap);	
		++iter;
		// check limits every 100 iterations
		if ((iter % 100) == 0)
		{
			bool limited = LimitsExceeded();
			if (chatter)
			{
				std::cout << "iter: " << iter << " time: " << solTime << " evals: " << evals << " best: " << curBestAnt << std::endl;
			}
			if ( limited)
			{
				break;
			}
		}
	}
	ClearPheromone();
	return curBestAnt;
}

