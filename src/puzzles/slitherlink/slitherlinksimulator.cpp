#include "slitherlinksimulator.h"
#include <fstream>
#include <iostream>
#include <set>

#define START_AS_MOVE

unsigned int SlitherlinkSimulator::HashEdge( const Point& p0, const Point& p1 ) const 
{
	unsigned short v0 = (p0.x<<8)|(p0.y&0xFF);
	unsigned short v1 = (p1.x<<8)|(p1.y&0xFF);
#ifndef DIRECTED_GRAPH
	if ( v1 < v0 )
	{
		unsigned int swap = v1;
		v1 = v0;
		v0 = swap;
	}
#endif
	return ((v0<<16)|v1)&(~0x80000000); // top bit has a special meaning (used for starting points) so zero it
}

unsigned int SlitherlinkSimulator::HashStartVert(const Point& p) const
{
	return 0x80000000 | (p.y * (board.GetNX() + 1) + p.x);
}

Point SlitherlinkSimulator::StartNodeFromHash(unsigned int hash) const
{
	hash &= ~0x80000000;
	Point p;
	p.x = hash % (board.GetNX() + 1);
	p.y = hash / (board.GetNX() + 1);
	return p;
}

Point SlitherlinkSimulator::NextFromHash( unsigned int hash ) const 
{
	Point p0, p1;
	p0.x = ((hash&0xFF000000)>>24);
	p0.y = ((hash&0x00FF0000)>>16);
	p1.x = ((hash&0x0000FF00)>>8);
	p1.y = (hash&0x000000FF);
	if ( p0 == curPoint )
		return p1;
	else
		return p0;
}

void SlitherlinkSimulator::StartRun( unsigned int startVal )
{
	// reset board, make first move with start val
	board.Reset();
	// pick random start point based on startVal
#ifndef START_AS_MOVE
	int iStart = startVal%((board.GetNX()+1)*(board.GetNY()+1));
	Point startNode;
	startNode.x = iStart%(board.GetNX()+1);
	startNode.y = iStart/(board.GetNX()+1);
	curPoint = startNode;
	board.InitTour( startNode );
#else
	curPoint.x = curPoint.y = -1;
#endif
}

void SlitherlinkSimulator::GetChoices( std::vector<unsigned int>& choices )
{
#ifndef START_AS_MOVE
	choices.clear();

	Point moves[4];
	int nMoves;
	board.GetMoves(moves, nMoves );
	for ( int i = 0; i < nMoves; i++ )
		choices.push_back( HashEdge( curPoint, moves[i] ) );
#else
	choices.clear();
	if (curPoint.x == -1)
	{
		// this is the first move
		for (int i = 0; i < board.GetNX() + 1; i++)
		{
			for (int j = 0; j < board.GetNY() + 1; j++)
			{
				Point p;
				p.x = i;
				p.y = j;
				choices.push_back(HashStartVert(p));
			}
		}
	}
	else
	{
		// as you were
		Point moves[4];
		int nMoves;
		board.GetMoves(moves, nMoves);
		for (int i = 0; i < nMoves; i++)
			choices.push_back(HashEdge(curPoint, moves[i]));
	}
#endif
}

void SlitherlinkSimulator::SetChoice( unsigned int choice )
{
#ifndef START_AS_MOVE
	Point next = NextFromHash( choice );
	curPoint = next;
	board.MakeMove( next );
#else
	if (choice & 0x80000000)
	{
		// this is the start point
		int iStart = choice & ~0x80000000;
		Point startNode = StartNodeFromHash(choice);
		curPoint = startNode;
		board.InitTour(startNode);
	}
	else
	{
		Point next = NextFromHash(choice);
		curPoint = next;
		board.MakeMove(next);
	}
#endif
}

int SlitherlinkSimulator::GetCost()
{
	return board.ScoreTour();
}

std::string SlitherlinkSimulator::StateAsString()
{
	return board.AsString();
}

Simulator* SlitherlinkSimulator::Copy()
{
	return new SlitherlinkSimulator( board );
}

void SlitherlinkSimulator::ReadSolution(const std::string& solFile)
{
	// solution is in the form of the connected component of cells inside the loop.
	std::ifstream in_file(solFile);
	std::string token;
	std::vector<char> solBoard;
	hasSolution = true;
	while (in_file >> token)
	{
		solBoard.push_back(token[0]);
	}
#if 0
	if (solBoard.size() != board.GetNX() * board.GetNY())
	{
		std::cout << "slitherlink solution/instance size mismatch" << std::endl;
	}
#endif
	int nx = board.GetNX();
	int ny = board.GetNY();

	Point p0, p1;
	std::set<Point> vertices;

	for (int i = 0; i < board.GetNX(); i++)
	{
		for (int j = 0; j < board.GetNY(); j++)
		{
			int here, left, right, top, bottom;
			here = j * nx + i;
			if (i == 0)
				left = -1;
			else
				left = j * nx + i - 1;
			if (i == nx-1)
				right = -1;
			else
				right = j * nx + i + 1;
			if (j == 0)
				bottom = -1;
			else
				bottom = (j-1) * nx + i;
			if (j == ny-1)
				top = -1;
			else
				top = (j + 1) * nx + i;
			if (solBoard[here] == 'x')
			{
				// this is a filled cell, check the surroundings
				if (left == -1 || solBoard[left] != 'x') // add edge to left
				{
					p0.x = i; p1.x = i;
					p0.y = j; p1.y = j + 1;
					solution.insert(HashStartVert(p0));
					solution.insert(HashStartVert(p1));
					solution.insert(HashEdge(p0, p1));
				}
				if (right == -1 || solBoard[right] != 'x') // add edge to right
				{
					p0.x = i+1; p1.x = i+1;
					p0.y = j; p1.y = j + 1;
					solution.insert(HashStartVert(p0));
					solution.insert(HashStartVert(p1));
					solution.insert(HashEdge(p0, p1));
				}
				if (bottom == -1 || solBoard[bottom] != 'x') // add edge to bottom
				{
					p0.x = i; p1.x = i+1;
					p0.y = j; p1.y = j;
					solution.insert(HashStartVert(p0));
					solution.insert(HashStartVert(p1));
					solution.insert(HashEdge(p0, p1));
				}
				if (top == -1 || solBoard[top] != 'x') // add edge to top
				{
					p0.x = i; p1.x = i + 1;
					p0.y = j+1; p1.y = j+1;
					solution.insert(HashStartVert(p0));
					solution.insert(HashStartVert(p1));
					solution.insert(HashEdge(p0, p1));
				}
			}
		}
	}
}

bool SlitherlinkSimulator::ComponentInSolution(unsigned int component)
{
	return solution.find(component) != solution.end();
}

		   
