#pragma once
#include <vector>
#include <map>
#include <random>
#include <iostream>
#include "chromosome.h"
#include "BitmapChromosome.h"
#include "IntChromosome.h"
#include "../../solver.h"
#include "../../simulator.h"
#include "../../timer.h"

class PuzzleGeneticAlgorithm : public Solver
{
	int chromosomeLength;
	int populationSize;
	int tournamentSize;
	float mutationRate;
	float crossoverRate;

	std::string bestSol;

	std::vector<Chromosome*> population;

	ChromosomeType cType;

	// stats
	int maxSolLen;
	int minSolLen;
	void PrintStats();
public:
	PuzzleGeneticAlgorithm(int chromosomeLength, int populationSize, int tournamentSize, float crossoverRate, float mutationRate, ChromosomeType cType=CT_BITMAP )
		: Solver(), chromosomeLength(chromosomeLength), populationSize(populationSize), tournamentSize(tournamentSize), crossoverRate(crossoverRate), mutationRate(mutationRate), cType(cType)
	{
		for (int i = 0; i < populationSize; i++)
		{
			population.push_back( NewChromosome() );
			population[i]->GenerateRandomSolution();
		}
	}

	~PuzzleGeneticAlgorithm()
	{
		for (auto c : population)
			delete c;
	}

	Chromosome *NewChromosome(void)
	{
		if (cType == CT_BITMAP)
			return new BitmapChromosome( this, chromosomeLength);
		else if (cType == CT_INT)
			return new IntChromosome(this, chromosomeLength);
		else
			return nullptr;
	}	

	virtual int Solve(Simulator *sim, float maxTime, int maxEvals ) override;
	virtual float GetSolutionTime() override { return solTime; }
	virtual std::string GetSolution() override { return bestSol; }

	virtual void PrintPopulation();

	virtual unsigned int TournamentSelection(unsigned int tSize);

};
