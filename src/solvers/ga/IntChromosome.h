#pragma once
#include "chromosome.h"
#include <cstdint>
#include <cstring>

class IntChromosome : public Chromosome
{
	std::vector<uint16_t> solution;
	bool done;
	std::vector<unsigned int> choices; // working array to hold the current options
	int chromosomePosition;
	bool StepSolution();

	uint16_t GetSolutionVal(int i);
	void PadToLength(int length);
public:
	IntChromosome(Solver *parent, int length) : Chromosome(parent, length) {}
	virtual ~IntChromosome() {}
	virtual void InitSolution();
	virtual void GenerateRandomSolution();
	virtual std::string GetRepresentation() { return std::string(); }
	virtual void InheritSolution(Chromosome* parent1);
	virtual void GenerateCrossoverSolution(Chromosome* parent1, Chromosome* parent2, float crossoverPoint);
	virtual void MutateSolution(float mutationRate);
	virtual void MutateOne();

};
