#include "puzzlegeneticalgorithm.h"
#include <iostream>

void PuzzleGeneticAlgorithm::PrintStats()
{
	// print out some stats
	int minCost, maxCost, totCost;
	minCost = 1000000; maxCost = totCost = 0;
	for (auto p : population)
	{
		if (p->GetCost() < minCost)
			minCost = p->GetCost();
		if (p->GetCost() > maxCost)
			maxCost = p->GetCost();
		totCost += p->GetCost();
	}

	std::cout << "solution lengths (min, max): " << minSolLen << " " << maxSolLen << std::endl;
	std::cout << "population costs (min, mean, max) " << minCost << " " << (float)totCost/population.size() << " " << maxCost << std::endl;
}

unsigned int PuzzleGeneticAlgorithm::TournamentSelection(unsigned int tSize)
{
	std::vector<unsigned int> tournamentEntries;
	for (unsigned int i = 0; i < tSize; i++)
	{
		tournamentEntries.push_back((int)floor(random()*population.size())%population.size());
	}

	unsigned int winner = tournamentEntries[0];

	for (unsigned int i = 1; i < tSize; i++)
	{
		if (population[tournamentEntries[i]]->GetCost() < population[winner]->GetCost())
		{
			winner = tournamentEntries[i];
		}
	}

	return winner;
}

void PuzzleGeneticAlgorithm::PrintPopulation()
{
	for (int i = 0; i < population.size(); i++)
	{
		std::cout << population[i]->GetSolution() << std::endl << std::endl;
	}
}

int PuzzleGeneticAlgorithm::Solve(Simulator * sim, float maxTime, int maxEvals)
{
	SetLimits(maxTime, maxEvals);
	int iter = 0;
	bool solved = false;
	int curBestChromosome = 0;
	
	int bestVal = 1000000;
	minSolLen = 10000000;
	maxSolLen = 0;

	for (auto p : population)
	{
		p->SetSimulator(sim);
	}

	while (!solved)
	{
		for (auto p : population)
		{
			p->InitSolution();
			if (p->UsedLength() > maxSolLen)
				maxSolLen = p->UsedLength();
			if (p->UsedLength() < minSolLen)
				minSolLen = p->UsedLength();

			AddEvaluation();
			if (p->GetCost() == 0)
				SetSolved();
		}

		//PrintPopulation();

		int iBest = 0;
		for (unsigned int i = 0; i < population.size(); i++)
		{
			if (population[i]->GetCost() < bestVal)
			{
				bestVal = population[i]->GetCost();
				iBest = i;
				bestSol = population[iBest]->GetSolution();
			}
		}

		// check timer every 100 iterations
		if ( (iter % 100) == 0)
		{
			bool limited = LimitsExceeded();
			if (chatter)
			{
				std::cout << "iter: " << iter << " time: " << solTime << " evals: " << evals << " best: " << bestVal << std::endl;
				PrintStats();
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
		else
		{
			std::vector<Chromosome*> children;

			while (children.size() < populationSize)
			{
				if (random() < crossoverRate)
				{
					int parent1 = TournamentSelection(tournamentSize);
					int parent2 = TournamentSelection(tournamentSize);
					float crossoverPoint = random();

					Chromosome* child1 = NewChromosome();
					child1->SetSimulator(sim);
					child1->GenerateCrossoverSolution(population[parent1], population[parent2], crossoverPoint);
					child1->MutateSolution(mutationRate);
					children.push_back(child1);
					Chromosome* child2 = NewChromosome();
					child2->SetSimulator(sim);
					child2->GenerateCrossoverSolution(population[parent2], population[parent1], crossoverPoint);
					child2->MutateSolution(mutationRate);
					children.push_back(child2);
				}
				else
				{
					Chromosome* child = NewChromosome();
					child-> SetSimulator(sim);
					int parent = TournamentSelection(tournamentSize);
					child->InheritSolution(population[parent]);
					child->MutateSolution(mutationRate);
					children.push_back(child);
				}
			}
			for (auto p : population) 
			{
				delete p;
			}
			population.swap(children);
		}

		++iter;
	}

	return bestVal;
}
