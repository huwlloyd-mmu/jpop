#include "nurikabesimulator.h"
#include "../../timer.h"
#include <iostream>
#include <fstream>

using namespace std;

void NurikabeSimulator::StartRun( unsigned int startVal )
{
	// copy the board
	curBoard.Copy(startBoard);

	int iStart = startVal%curBoard.GetNumIslands();
	for (int i = 0; i < curBoard.GetNumIslands(); i++)
	{
		islandOrder[i] = (iStart + i) % curBoard.GetNumIslands();
	}
	curIslandIndex = 0;
	curIsland.clear();
	const std::vector<int>&nc = curBoard.GetNumberedCells();
	int iCell = nc[islandOrder[curIslandIndex]];
	curIslandId = curBoard.GetCell( iCell ).islandId;
	curIsland.insert( iCell );
}

void NurikabeSimulator::GetChoices( std::vector<unsigned int>& choices )
{
	// find all the available cells around the current island

	// to be available, cell must be black, bordering the current island,
	// not a cut cell for the river, and not bordering any cell with a
	// different island id.
	choices.clear();

	if (curIslandIndex == -1)
	{
		// give choice of islands
		for (int i = 0; i < curBoard.GetNumIslands(); i++)
		{
			// set the top bit, to distinguish this choice from the cell choices
			choices.push_back(0x80000000|i);
		}
		return;
	}

	bool foundOptions = false;

	while ( !foundOptions )
	{
		// first build a list of neighbours for the current island
		set<int> islandNeighbours;
		vector<int> cellNeighbours;
		vector<int> neighboursNeighbours;
		set<int> choiceSet;

		if ( curIsland.size() < curBoard.GetCell(curBoard.GetNumberedCells()[islandOrder[curIslandIndex]]).value )
		{
			for ( int iCell : curIsland )
			{
				curBoard.GetNeighbours( iCell, cellNeighbours );
				for ( int i : cellNeighbours )
				{
					if ( curIsland.find(i) == curIsland.end() && islandNeighbours.find(i) == islandNeighbours.end() )
						islandNeighbours.insert(i);
				}
			}
			// now check all the neighbours for eligibility
			for ( int iCell : islandNeighbours )
			{
				if ( curBoard.GetCell(iCell).color == BLACK && !curBoard.GetCell(iCell).cutFlag )
				{
					// check for neighbouring another island
					curBoard.GetNeighbours( iCell, cellNeighbours );
					bool valid = true;
					for ( int n : cellNeighbours )
					{
						int id = curBoard.GetCell(n).islandId;
						if ( id != -1 && id != curIslandId )
						{
							valid = false;
							break;
						}
					}
					if ( valid )
						choiceSet.insert(iCell);
				}
			}
		}
		if ( choiceSet.size() == 0 )
		{
			// move to a new island and try again
			++curIslandIndex;
			if ( curIslandIndex == curBoard.GetNumIslands() )
			{
				// finish with empty choice vector
				foundOptions = true;
			}
			else
			{
				curIsland.clear();
				int iCell = curBoard.GetNumberedCells()[islandOrder[curIslandIndex]];
				curIslandId = curBoard.GetCell( iCell ).islandId;
				curIsland.insert( iCell );
			}
		}
		else
		{
			foundOptions = true;
			for ( int c : choiceSet )
				choices.push_back( c );
		}
	}
}

void NurikabeSimulator::SetChoice( unsigned int choice )
{
	curBoard.SetCell( choice, curIslandId );
	curBoard.FlagCutVerts();
	curIsland.insert( choice );
}

int NurikabeSimulator::GetCost( void )
{
	// count white cells
	int cost = 0;
	
	int nCells = curBoard.GetNumCells();
	int whiteCells = 0;
	for ( int i = 0; i < nCells; i++ )
	{
		if ( curBoard.GetCell(i).color == WHITE )
			whiteCells++;
	}
	cost = curBoard.GetIslandTotal() - whiteCells;
	// count 4x4 blocks
	for ( int i = 0; i < curBoard.GetXSize()-1; i++ )
	{
		for ( int j = 0; j < curBoard.GetYSize()-1; j++ )
		{
			if ( curBoard.GetCell(i,j).color == BLACK &&
				 curBoard.GetCell(i+1,j).color == BLACK &&
				 curBoard.GetCell(i+1,j+1).color == BLACK &&
				 curBoard.GetCell(i,j+1).color == BLACK )
				++cost;
		}
	}
	return cost;
}

Simulator* NurikabeSimulator::Copy()
{
	return new NurikabeSimulator( startBoard );
}

std::string NurikabeSimulator::StateAsString()
{
	return curBoard.AsString();
}

void NurikabeSimulator::ReadSolution(const std::string& solFile)
{
	ifstream infile( solFile);
	string token;
	int curCell = 0;
	
	while (infile >> token)
	{
		if (token == string("-"))
			solution.insert(curCell);
		curCell++;
	}
	hasSolution = true;
}

bool NurikabeSimulator::HasSolution() const
{
	return hasSolution;
}

bool NurikabeSimulator::ComponentInSolution(unsigned int component)
{
	if (hasSolution)
		return solution.find(component) != solution.end();
	else
		return false;
}
