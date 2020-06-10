#pragma once
#include "../../simulator.h"
#include <string>

class PuzzleAntSystem;

class PuzzleAnt
{
	PuzzleAntSystem *parent;	// parent ant system
	Simulator *simulator;       // puzzle simulator - gives the ant a series of choices

	std::vector<unsigned int> choices; // working array to hold the current options
	std::vector<float> partialSums; // working array used for roulette selection
	std::vector<unsigned int> tour;   // record of the choices made in the current run
	bool done;                  // finished a run
public:	
PuzzleAnt(PuzzleAntSystem *parent) : parent(parent), simulator(nullptr), done(false) {}
	
	void SetSimulator( Simulator *sim )
	{
		if ( simulator != nullptr )
			delete simulator;
		simulator = sim->Copy();
	}
	
	void InitSolution();
	bool StepSolution();
	int GetCost() { return simulator->GetCost(); }
	std::string GetSolution() { return simulator->StateAsString(); }
	const std::vector<unsigned int>& GetChoices() { return tour; }
};
