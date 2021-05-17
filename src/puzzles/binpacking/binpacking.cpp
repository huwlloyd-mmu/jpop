#include "binpacking.h"
#include <cstring>

void BinPackingSimulator::StartRun(unsigned int startVal)
{
	std::memset(tabu, 0, sizeof(uint32_t) * sizes.size());
	solution.clear();
	curBin = 0;
	nBins = 1;
}

void BinPackingSimulator::GetChoices(std::vector<unsigned int>& choices)
{
	choices.clear();
	size_t step = solution.size();
	for (uint32_t i = 0; i < sizes.size(); i++)
	{
		if (!tabu[i])
		{
			choices.push_back((step << 16) | i);
		}
	}
}

void BinPackingSimulator::SetChoice(unsigned int choice)
{
	choice &= 0xFFFF;
	uint32_t thisSize = sizes[choice];
	solution.push_back(choice);
	tabu[choice] = 1;
	if (binSize - curBin < thisSize)
	{
		nBins++;
		curBin = binSize-thisSize;
	}
	else
	{
		curBin += thisSize;
	}
}

int BinPackingSimulator::GetCost(void)
{
	return nBins - lowerLimit;
}

Simulator* BinPackingSimulator::Copy()
{
	BinPackingSimulator* newSim = new BinPackingSimulator(sizes, binSize, name, bestKnown);
	return static_cast<Simulator*>(newSim);
}

std::string BinPackingSimulator::StateAsString()
{
	return string();
}

