#pragma once
#include <vector>
#include <map>
#include <random>
#include <iostream>
#include "../ga/IntChromosome.h"
#include "../../solver.h"
#include "../../simulator.h"
#include "../../timer.h"

class SimulatedAnnealing : public Solver
{

	float maxTemp;
	float tempChange;
	int chromosomeLength;

	std::string bestSol;

	Chromosome* currentSol;


public:
	SimulatedAnnealing(float maxTemp, float tempChange, int chromosomeLength)
		: maxTemp(maxTemp), tempChange(tempChange), chromosomeLength(chromosomeLength)
	{
		currentSol = new IntChromosome(this, chromosomeLength);
	}

	~SimulatedAnnealing()
	{
		delete currentSol;
	}

	IntChromosome* CreateNeighbour(Chromosome* current, Simulator* sim);
	bool ShouldAccept(Chromosome* candidate, Chromosome* current, float temperature);

	virtual int Solve(Simulator *sim, float maxTime, int maxEvals) override;
	virtual float GetSolutionTime() override { return solTime; }
	virtual std::string GetSolution() override { return bestSol; }
};


