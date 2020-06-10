#include "sudokusimulator.h"
#include <fstream>
void SudokuSimulator::StartRun( unsigned int startVal )
{
	failCells = 0;
	cellsVisited = 0;
	curCell = startVal % startBoard.CellCount();
	curBoard.Copy(startBoard);
}

void SudokuSimulator::GetChoices( std::vector<unsigned int>& choices )
{
	choices.clear();
	bool choiceFound = false;
	while (!choiceFound)
	{
		cellsVisited++;
		if ( cellsVisited > curBoard.CellCount() )
		{
			// run out of cells
			return;
		}
		else if ( curBoard.GetCell(curCell).Empty() )
		{
			failCells++;
		}
		else if ( !curBoard.GetCell(curCell).Fixed() )
		{
			// there are choices
			ValueSet choice = ValueSet(curBoard.GetNumUnits(), (uint64_t)1);			
			for (int i = 0; i < curBoard.GetNumUnits(); i++)
			{
				if (curBoard.GetCell(curCell).Contains(choice))
				{
					choices.push_back( curCell*curBoard.GetNumUnits() + i ); 
				}
				choice <<= 1;
			}
			choiceFound = true;
		}
		else
		{
			// give back the only choice
			choices.push_back(curCell*curBoard.GetNumUnits() + curBoard.GetCell(curCell).Index());
			choiceFound = true;
		}
		curCell++;
		// wrap around
		if ( curCell == curBoard.CellCount() )
			curCell = 0;
	}
	return;
}

void SudokuSimulator::SetChoice( unsigned int choice ) 
{
	unsigned int iCell = choice/curBoard.GetNumUnits();
	unsigned int iValue = choice%curBoard.GetNumUnits();
	ValueSet val( curBoard.GetNumUnits(), (int64_t)1<<iValue );
	if ( !curBoard.GetCell(iCell).Fixed() )
		curBoard.SetCell( iCell, val );
}

int SudokuSimulator::GetCost( void ) 
{
	// fail cells is the number of empty cells found while constructing
	// an attempted solution
	return failCells;
}

Simulator* SudokuSimulator::Copy() 
{
	return new SudokuSimulator( startBoard );
}

std::string SudokuSimulator::StateAsString() 
{
	return curBoard.AsString();
};

void SudokuSimulator::ReadSolution(const std::string& solFile)
{
	ifstream infile(solFile);
	string token;
	int curCell = 0;

	while (infile >> token)
	{
		int val = std::stoi(token);
		unsigned int component = curCell* startBoard.GetNumUnits() + val-1;
		solution.insert(component);
		curCell++;
	}
	hasSolution = true;
	if (curCell != startBoard.CellCount())
	{
		std::cout << "warning: sudoku solution cell count does not match instance" << std::endl;
	}
}

bool SudokuSimulator::ComponentInSolution(unsigned int component)
{
	if (hasSolution)
		return solution.find(component) != solution.end();
	else
		return false;
}


