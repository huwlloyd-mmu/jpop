#include "BitmapChromosome.h"
#include "puzzlegeneticalgorithm.h"

void BitmapChromosome::InitSolution()
{
	unsigned int startVal = 0;
	chromosomePosition = solution.size() - length;
	usedLength = 0;
	if (simulator->NeedsStartVal())
	{
		// pull off 16 bits for the start value
		size_t bitsNeeded = 16;

		std::bitset<1024> choice(solution.to_string().substr(chromosomePosition, bitsNeeded));
		startVal = choice.to_ullong();

		chromosomePosition += bitsNeeded;
		usedLength += bitsNeeded;
		if (chromosomePosition >= solution.size())
		{
			// run out of bits, so this solution is forced to be done
			std::cerr << "Solution does not have enough bits to make all choices, try increasing number of bits." << std::endl;
			done = true;
		}
	}
	simulator->StartRun(startVal);
	done = false;
	do
		done = StepSolution();
	while (!done);
	stateAsString = simulator->StateAsString();
	cost = simulator->GetCost();
}

// simple 1-point crossover
void BitmapChromosome::GenerateCrossoverSolution(Chromosome* p1, Chromosome* p2, float fCrossoverPoint)
{
	//std::cout << "Parent 1 solution: " << parent1->solution << std::endl;
	//std::cout << "Parent 2 solution: " << parent2->solution << std::endl;
	//std::cout << "Crossover point: " << crossoverPoint << std::endl;
	int crossoverPoint = (int)floor(fCrossoverPoint * length);
	BitmapChromosome *parent1 = (BitmapChromosome*)p1;
	BitmapChromosome *parent2 = (BitmapChromosome*)p2;
	crossoverPoint += solution.size() - length;

	//std::cout << "Adjusted crossover point: " << crossoverPoint << std::endl;
	for (int i = 0; i < crossoverPoint; i++)
	{
		solution[i] = parent1->solution[i];
	}
	for (int i = crossoverPoint; i < solution.size(); i++)
	{
		solution[i] = parent2->solution[i];
	}

	//std::cout << "New solution: " << solution << std::endl << std::endl;
}

void BitmapChromosome::InheritSolution(Chromosome* parent1)
{
	solution = ((BitmapChromosome*)parent1)->solution;
}

void BitmapChromosome::MutateSolution(float mutationRate)
{
	for (int i = 0; i < solution.size(); i++)
	{
		if (parent->random() < mutationRate)
		{	
			solution[i] = !solution[i];
		}
	}
}

void BitmapChromosome::MutateOne()
{
	int pointToChange = floor(parent->random()*solution.size());
	std::cout << pointToChange << std::endl;
	solution[pointToChange] = !solution[pointToChange];
}

void BitmapChromosome::GenerateRandomSolution()
{
	for (int i = 0; i < solution.size(); i++)
	{
		solution[i] = parent->random() <= 0.5;
	}
}

static int CalculateBitsNeeded(int potentialChoices) 
{
	return ceil(log(potentialChoices) / log(2));
};

bool BitmapChromosome::StepSolution()
{
	if (done)
		return done;

	simulator->GetChoices(choices);
	if (choices.size() == 0)
	{
		// run is finished, nothing left to choose
		done = true;
	}
	else
	{
		// make a choice
		unsigned int thisChoice;
		if (choices.size() == 1)
			thisChoice = choices[0]; // optimization for case of only one choice
		else
		{
			size_t bitsNeeded = CalculateBitsNeeded(choices.size());
			
			std::bitset<1024> choice(solution.to_string().substr(chromosomePosition, bitsNeeded));
			thisChoice = choices[choice.to_ullong()%choices.size()];

			chromosomePosition += bitsNeeded;
			usedLength += bitsNeeded;
			if (chromosomePosition >= solution.size())
			{
				// run out of bits, so this solution is forced to be done
				std::cerr << "Solution does not have enough bits to make all choices, try increasing number of bits." << std::endl;
				done = true;
			}
		}
		simulator->SetChoice(thisChoice);

	}
	return done;
}
