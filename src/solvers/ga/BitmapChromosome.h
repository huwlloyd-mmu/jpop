#pragma once
#include "chromosome.h"

class BitmapChromosome : public Chromosome
{
	// to accommodate variable number of bits, this is set arbitrarily high
	// avoid using solution.size() when iterating and instead use 0 to bitsize
	std::bitset<1024> solution;
	bool done;
	std::vector<unsigned int> choices; // working array to hold the current options
	int chromosomePosition;
	bool StepSolution();

public:
	BitmapChromosome(Solver *parent, int length) : Chromosome(parent, length) {}
	virtual ~BitmapChromosome() {};
	virtual void InitSolution();
	virtual void GenerateRandomSolution();
	virtual std::string GetRepresentation() { return solution.to_string().substr(solution.size() - length, length); }
	virtual void InheritSolution(Chromosome* parent1);
	virtual void GenerateCrossoverSolution(Chromosome* parent1, Chromosome* parent2, float crossoverPoint);
	virtual void MutateSolution(float mutationRate);
	virtual void MutateOne();
};
