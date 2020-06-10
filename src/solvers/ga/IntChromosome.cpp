#include "IntChromosome.h"
#include "puzzlegeneticalgorithm.h"

uint16_t IntChromosome::GetSolutionVal(int i)
{
	if (i > (int)solution.size() - 1)
		PadToLength(i + 1);
	return solution[i];
}

void IntChromosome::PadToLength(int newLength)
{
	int iStart = solution.size();
	for (int i = iStart; i < newLength; i++)
	{
		uint16_t val = (uint16_t)(parent->random() * 0xFFFF);
		solution.push_back(val);
	}
	length = newLength;
}

void IntChromosome::InitSolution()
{
	int i = 0;
	unsigned int startVal = 0;
	if (simulator->NeedsStartVal())
	{
		startVal = GetSolutionVal(0);
		i = 1;
	}
	simulator->StartRun(startVal);
	std::vector<unsigned int> choices;
	bool done = false;
	do
	{
		simulator->GetChoices(choices);
		int nChoices = choices.size();
		if (nChoices == 0)
			done = true;
		else
		{
			if (nChoices == 1)
			{
				simulator->SetChoice(choices[0]);
			}
			else
			{
				simulator->SetChoice(choices[GetSolutionVal(i) % nChoices]);
				++i;
			}
		}
	} while (!done);
	usedLength = i;
	stateAsString = simulator->StateAsString();
	cost = simulator->GetCost();
}


// simple 1-point crossover
void IntChromosome::GenerateCrossoverSolution(Chromosome* p1, Chromosome* p2, float fCrossoverPoint)
{
	IntChromosome *parent1 = (IntChromosome*)p1;
	IntChromosome *parent2 = (IntChromosome*)p2;

	if (parent1->length > parent2->length)
		parent2->PadToLength(parent1->length);
	else if (parent2->length > parent1->length)
		parent1->PadToLength(parent2->length);

	PadToLength(parent1->length);
	int crossoverPoint = (int)floor(fCrossoverPoint * parent1->length);
	for (int i = 0; i < crossoverPoint; i++)
	{
		solution[i] = parent1->solution[i];
	}
	for (int i = crossoverPoint; i < length; i++)
	{
		solution[i] = parent2->solution[i];
	}
}

void IntChromosome::InheritSolution(Chromosome* parent1)
{
	solution.clear();
	for (auto v : ((IntChromosome*)parent1)->solution)
		solution.push_back(v);
	length = solution.size();
}

void IntChromosome::MutateSolution(float mutationRate)
{
	for (int i = 0; i < length; i++)
	{
		if (parent->random() < mutationRate)
		{
			solution[i] = parent->random()*0xFFFF;
		}
	}
}

void IntChromosome::MutateOne()
{
	int pointToChange = floor(parent->random() * length);
	solution[pointToChange] = parent->random() * 0xFFFF;
}

void IntChromosome::GenerateRandomSolution()
{
	// clear the chromosome - padding adds randoms as required
	length = 0;
	solution.clear();
}

