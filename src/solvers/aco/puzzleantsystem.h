#pragma once
#include <vector>
#include <map>
#include <random>
#include "puzzleant.h"
#include "../../solver.h"
#include "../../simulator.h"
#include "../../timer.h"

class PuzzleAntSystem : public Solver
{
	int numAnts;
	float q0;
	float rho;
	float pher0;
	float bestEvap;
	std::string bestSol;
	std::vector<unsigned int> bestChoices;
	
	float bestPher;
	int bestVal;

	std::vector<PuzzleAnt*> antList;
	std::map<unsigned int,float> pher;

	int numCells;
	void InitPheromone();
	void ClearPheromone();
	void UpdatePheromone();
	float PherAdd(int cost);

public:
	PuzzleAntSystem(int numAnts, float q0, float rho, float pher0, float bestEvap) : 
		Solver(), numAnts(numAnts), q0(q0), rho(rho), pher0(pher0), bestEvap(bestEvap)
	{
		for ( int i = 0; i < numAnts; i++ )
			antList.push_back(new PuzzleAnt(this));
	}
	~PuzzleAntSystem()
	{
		for (auto a : antList)
			delete a;
	}
	virtual int Solve(Simulator *sim, float maxTime, int maxEvals ) override;
	virtual float GetSolutionTime() override { return solTime; }
	virtual std::string GetSolution() override { return bestSol; }
	// helpers for ants
	inline float Getq0() { return q0; }
	inline float Pher( unsigned int hash )
	{
		if ( pher.find(hash) != pher.end() )
		{
			return pher[hash];
		}
		else
		{
			return pher0;
		}
	}

	inline void SetPher( unsigned int hash, float val )
	{
		pher[hash] = val;
	}

	void LocalPheromoneUpdate(unsigned int hash);
};
