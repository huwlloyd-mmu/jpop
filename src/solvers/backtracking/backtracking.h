#pragma once
#include <iostream>
#include <string>
#include "../../solver.h"
#include "../../simulator.h"
#include "../../timer.h"
#include "SimulatorStack.h"

class BacktrackingSearch : public Solver
{
	std::string bestSol;
	int bestVal;
	int evaluations;
	bool solved;
	bool timedOut;
	SimulatorStack *sim_stack;
	void StepSol(void);
public:

	BacktrackingSearch() {}
	virtual ~BacktrackingSearch() {}

	virtual int Solve(Simulator* sim, float maxTime, int maxEvals ) override;
	virtual float GetSolutionTime() override { return solTime; }
	virtual std::string GetSolution() override { return bestSol; }

	// not yet implemented - not used by this solver
	// better architecturing solution than this?
	inline float random() { return 0.0f; }
};