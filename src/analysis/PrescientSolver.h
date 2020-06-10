#pragma once
#include <random>
#include <iostream>
#include "../solver.h"
#include "../simulator.h"

class PrescientSolver : public Solver
{
	std::string solution;
	int numSteps;
	std::vector<float> deltaLogProb;
	std::vector<float> logProbByStep;
	float accumulatedProb;
	std::mt19937 rng; // random number generator
public:
	PrescientSolver() : Solver()
	{
		rng = std::mt19937(std::random_device()());
	}

	virtual int Solve(Simulator* sim, float maxTime, int maxEvals) override;
	virtual float GetSolutionTime() override { return 0.0f; } // not used 
	virtual std::string GetSolution() override { return solution; }

	int GetNumSteps() const { return numSteps;  }
	float GetLogProb() const { return accumulatedProb; }
	const std::vector<float>& GetLogProbByStep() const { return logProbByStep; }
};