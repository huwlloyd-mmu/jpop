#pragma once
#include <string>
#include <random>
#include "timer.h"
class Simulator;

class Solver
{
	// base class for solvers
	// includes helpers for managing timeout and evaluation limit
	bool timeLimited;
	bool evalLimited;
protected:
	Timer solutionTimer;
	float solTime;
	float maxTime;
	int maxEvals;
	std::mt19937 randGen;
	std::uniform_real_distribution<float> randomDist;
	bool chatter = true;
	int evals;
	int solvedEvals = -1; // evaluation on which the instance was solved

	void SetLimits(float timeLimit, int evaluationLimit )
	{
		timeLimited = evalLimited = false;
		evals = 0;
		solvedEvals = -1;
		solutionTimer.Reset();
		// solvers should call this at the beginning of a solution run
		if (timeLimit != 0.0f)
		{
			timeLimited = true;
			maxTime = timeLimit;
		}
		if (evaluationLimit != 0)
		{
			evalLimited = true;
			maxEvals = evaluationLimit;
		}
	}
	void SetSolved() { if ( solvedEvals == -1 ) solvedEvals = evals; }
	void AddEvaluation(int nEval = 1) { evals += nEval;  }

	bool LimitsExceeded()
	{
		solTime = solutionTimer.Elapsed();
		if ((timeLimited && solTime > maxTime) || (evalLimited && evals > maxEvals))
			return true;
		return false;
	}
public:
	Solver() 
	{
		randomDist = std::uniform_real_distribution<float>(0.0f, 1.0f);
		std::random_device rd;
		randGen = std::mt19937(rd());
	}
	virtual int Solve(Simulator *sim, float maxTime, int maxEvals ) = 0;
	virtual float GetSolutionTime() = 0;
	virtual std::string GetSolution() = 0;
	int GetNumEvals() const { if (solvedEvals != -1) return solvedEvals; else return evals; }
	void Mute(bool bMute = true) { chatter = !bMute; }
	virtual ~Solver() {};
	inline float random() { return randomDist(randGen); }
};