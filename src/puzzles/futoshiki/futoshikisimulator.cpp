#include "futoshikisimulator.h"
#include <fstream>

void FutoshikiSimulator::StartRun( unsigned int startVal )
{
	failCells = 0;
	cellsVisited = 0;
	curCell = startVal % startBoard.CellCount();
	curBoard.Copy(startBoard);
}

void FutoshikiSimulator::GetChoices( std::vector<unsigned int>& choices )
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

void FutoshikiSimulator::SetChoice(unsigned int choice)
{
	unsigned int iCell = choice/curBoard.GetNumUnits();
	unsigned int iValue = choice%curBoard.GetNumUnits();
	ValueSet val( curBoard.GetNumUnits(), (int64_t)1<<iValue );
	if (!curBoard.GetCell(iCell).Fixed())
		curBoard.SetCellFixed(iCell, val, true);
}

int FutoshikiSimulator::GetCost( void )
{
	// fail cells is the number of empty cells found while constructing
	// an attempted solution
	return failCells;
}

Simulator* FutoshikiSimulator::Copy()
{
	return new FutoshikiSimulator( startBoard );
}

std::string FutoshikiSimulator::StateAsString()
{
	return curBoard.AsString();
};

void FutoshikiSimulator::ReadSolution(const std::string& solFile)
{
	ifstream infile(solFile);
	string token;
	int curCell = 0;

	while (infile >> token)
	{
		int val = std::stoi(token);
		unsigned int component = curCell * startBoard.GetNumUnits() + val - 1;
		solution.insert(component);
		curCell++;
	}
	hasSolution = true;
	if (curCell != startBoard.CellCount())
	{
		std::cout << "warning: futoshiki solution cell count does not match instance" << std::endl;
	}
}

bool FutoshikiSimulator::ComponentInSolution(unsigned int component)
{
	if (hasSolution)
		return solution.find(component) != solution.end();
	else
		return false;
}

