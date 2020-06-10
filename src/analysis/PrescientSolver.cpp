#include "PrescientSolver.h"

int PrescientSolver::Solve(Simulator* sim, float maxTime, int maxEvals)
{
	if (!sim->HasSolution())
	{
		// can't do this without a solution
		return false;
	}
	deltaLogProb.clear();
	accumulatedProb = 0.0f;
	numSteps = 0;
	bool solved = false;

	std::vector<unsigned int> choices;
	sim->StartRun(0); // get a start val - random?
	std::vector<unsigned int> correctChoices;
	std::vector<unsigned int> incorrectChoices;

	while (!solved)
	{
		// get choices
		sim->GetChoices(choices);
		// split between correct and incorrect
		correctChoices.clear();
		incorrectChoices.clear();
		if (choices.size() == 0)
		{
			solved = true;
			break;
		}
		for (auto c : choices)
		{
			if (sim->ComponentInSolution(c))
				correctChoices.push_back(c);
			else
				incorrectChoices.push_back(c);
		}
		if (correctChoices.size() == 0)
		{
			// no correct choices - must be e.g. choosing a start island in nurikabe - anything will do
			// don't need to include this in the probability
			sim->GetChoices(choices);
			std::uniform_int_distribution<unsigned int> dist(0, incorrectChoices.size() - 1);
			unsigned int choice = dist(rng);
			sim->SetChoice(incorrectChoices[choice]);
		}
		else
		{
			// update probability
			float pCorrect = log10( (float)correctChoices.size() / (float)(correctChoices.size() + incorrectChoices.size()));
			accumulatedProb += pCorrect;
			deltaLogProb.push_back(pCorrect);
			// choose a correct choice at random
			std::uniform_int_distribution<unsigned int> dist(0, correctChoices.size() - 1);
			unsigned int choice = dist(rng);
			sim->SetChoice(correctChoices[choice]);

			++numSteps;
		}
	}
	// confirm it's actually solved
	if (sim->GetCost() != 0)
	{
		std::cout << "warning - prescient solver failed to get a solution!" << std::endl;
	}
	// fill in the distance to solution array
	float acc = accumulatedProb;
	for (int i = 0; i < numSteps; i++)
	{
		logProbByStep.push_back(acc);
		acc -= deltaLogProb[i];
	}
	return 0;
}

