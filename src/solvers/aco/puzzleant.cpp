#include "puzzleant.h"
#include "puzzleantsystem.h"

void PuzzleAnt::InitSolution()
{
	// start ant at a random position
	unsigned int startVal = (unsigned int)(0xFFFF * parent->random());
	simulator->StartRun(startVal);
	tour.clear();
	done = false;
}

bool PuzzleAnt::StepSolution()
{
	if ( done )
		return done;
	
	simulator->GetChoices(choices);
	if ( choices.size() == 0 )
	{
		// run is finished, nothing left to choose
		done = true;
	}
	else
	{
		// make a choice
		unsigned int thisChoice;
		if ( choices.size() == 1 )
			thisChoice = choices[0]; // optimization for case of only one choice
		else if ( parent->random() < parent->Getq0() )
		{
			// greedy selection
			int bestChoice = -1;
			float bestPher = -1.0f;
			for ( unsigned int choice : choices )
			{
				float pher = parent->Pher(choice);
				if ( pher > bestPher )
				{
					bestPher = pher;
					bestChoice = choice;
				}
			}
			thisChoice = bestChoice;
		}
		else
		{
			// roulette selection
			float sum = 0.0f;
			partialSums.clear();
		  
			for ( unsigned int choice : choices )
			{
				sum += parent->Pher( choice );
				partialSums.push_back( sum );
			}
			float selection = parent->random() * sum;
			int iSel = -1;
			for ( int i = 0; i < partialSums.size(); i++ )
			{ 
				if ( partialSums[i] >= selection )
				{
					iSel = i;
					break;
				}
			}
			thisChoice = choices[iSel];
		}
		simulator->SetChoice( thisChoice );
		tour.push_back(thisChoice);
		parent->LocalPheromoneUpdate(thisChoice);
	}
	return done;
}

