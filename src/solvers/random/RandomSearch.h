#pragma once
#include <random>
#include <iostream>
#include "../../solver.h"
#include "../../simulator.h"
#include "../../timer.h"

class RandomSearch : public Solver
{
	std::string bestSol;
public:
	RandomSearch() : Solver()
	{
	}
	virtual int Solve(Simulator *sim, float maxTime, int maxEvals) override;
	virtual float GetSolutionTime() override { return solTime; }
	virtual std::string GetSolution() override { return bestSol; }
};