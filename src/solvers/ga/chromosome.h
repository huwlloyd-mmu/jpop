#pragma once
#include "../../simulator.h"
#include <bitset>
#include <string>
#include <iostream>

class Solver;

enum ChromosomeType
{
	CT_BITMAP,
	CT_INT
};

//template <typename T>
class Chromosome
{
protected:
	Solver *parent;
	Simulator *simulator;
	int length;
	int usedLength;
	bool done;
	int cost;
	std::string stateAsString;

public:
	Chromosome(Solver *parent, int length) : parent(parent), length(length), simulator(nullptr), done(false) {}
	virtual ~Chromosome() {}
	void SetSimulator(Simulator *sim)
	{
		// chromosomes should be able to share a simulator.
		simulator = sim;
	}
	int GetCost() { return cost; }
	int UsedLength() { return usedLength; }
	std::string GetSolution() { return stateAsString; }

	virtual void InitSolution() = 0;
	virtual void GenerateRandomSolution() = 0;
	virtual std::string GetRepresentation() = 0;
	virtual void InheritSolution(Chromosome* parent1) = 0;
	virtual void GenerateCrossoverSolution(Chromosome* parent1, Chromosome* parent2, float crossoverPoint) = 0;
	virtual void MutateSolution(float mutationRate) = 0;

};