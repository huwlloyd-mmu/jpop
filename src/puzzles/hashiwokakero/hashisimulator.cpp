#include "hashisimulator.h"
#include <fstream>
#include <iostream>
#include <algorithm>
void HashiSimulator::StartRun( unsigned int startVal ) 
{
	if (edgeOrder.size() == 0)
	{
		for (int i = 0; i < startBoard.GetNumEdgeSites(); i++)
			edgeOrder.push_back(i);
	}

	curBoard.Copy(startBoard);
	std::uniform_int_distribution<int> dist(0, startBoard.GetNumEdgeSites() - 1);
	curEdge = startVal % startBoard.GetNumEdgeSites();
	numEdgesVisited = 0;
//	std::random_shuffle(edgeOrder.begin(), edgeOrder.end());
}

void HashiSimulator::GetChoices(std::vector<unsigned int>& choices)
{
	choices.clear();
	if (numEdgesVisited == curBoard.GetNumEdgeSites())
	{
		// finished, return no choices
		return;
	}
	++numEdgesVisited;
	if (curBoard.GetEdgeFixed(edgeOrder[curEdge]))
	{
		// return only the fixed value of this edge
		unsigned int choice = edgeOrder[curEdge] * 3 + curBoard.GetEdgeValue(edgeOrder[curEdge]);
		choices.push_back(choice);
	}
	else
	{
		for (unsigned int i = 0; i <= curBoard.GetEdgeCapacity(edgeOrder[curEdge]); i++)
		{
			unsigned int choice = edgeOrder[curEdge] * 3 + i;
			choices.push_back(choice);
		}
//		std::random_shuffle(choices.begin(), choices.end());
	}
	++curEdge;
	if (curEdge == curBoard.GetNumEdgeSites())
	{
		// wrap around
		curEdge = 0;
	}
	return;
}

void HashiSimulator::SetChoice(unsigned int choice)
{
	int iEdge = choice / 3;
	int iVal = choice % 3;
	curBoard.SetEdgeSite(iEdge, iVal);
}

int HashiSimulator::GetCost(void)
{
	return curBoard.Cost();
}

Simulator* HashiSimulator::Copy()
{
	return new HashiSimulator(startBoard);
}

std::string HashiSimulator::StateAsString()
{
	return curBoard.AsString();
}

void HashiSimulator::ReadSolution(const std::string& solFile)
{
	std::ifstream in_file(solFile);
	std::string token;
	std::vector<char> board;
	hasSolution = true;
	while (in_file >> token)
	{
		board.push_back(token[0]);
	}
	if (board.size() != startBoard.GetNX() * startBoard.GetNY())
	{
		std::cout << "hashi solution/instance size mismatch" << std::endl;
	}
	// check the order (x,y) the instance is read
	// raster scan order - y = 0 line followed by y=1 line etc.
	// so point i, j in the grid this list will be edges[j*ny+i]

	// now run through edge list in the start board checking what values to assign to the edges
	int ny = startBoard.GetNY();
	int nx = startBoard.GetNX();
	for (int i = 0; i < startBoard.GetNumEdgeSites(); i++)
	{
		int x0, y0, x1, y1;
		startBoard.GetEdge(i, x0, y0, x1, y1);
		int occupancy = 0;
		if (y0 == y1)
		{
			char c = board[y0 * nx + x0 + 1];
			if (c == '1')
				occupancy = 1;
			else if (c == '2')
				occupancy = 2;
		}
		else
		{
			char c = board[(y0+1) * nx + x0];
			if (c == 'a')
				occupancy = 1;
			else if (c == 'b')
				occupancy = 2;
		}
		int component = i * 3 + occupancy;
		solution.insert(component);
	}
}

bool HashiSimulator::ComponentInSolution(unsigned int component)
{
	if (hasSolution)
		return solution.find(component) != solution.end();
	else
		return false;
}
