#include "zpgsimulator.h"
#include <iostream>

ZPGSimulator::ZPGSimulator(const ZPGBoard& instance) 
{ 
	startBoard.Copy(instance); 
	rng = mt19937(random_device{}()); 
	totalMoves = 0;
	monkPosition.location.x = 0;
	monkPosition.location.y = 0;
	monkPosition.direction = Directions::NONE;
}

void ZPGSimulator::StartRun(unsigned int startVal)
{
	curBoard.Copy(startBoard);
	totalMoves = 0;
	monkPosition.location.x = 0;
	monkPosition.location.y = 0;
	monkPosition.direction = Directions::NONE;
}

#define AUTO_CHOICE 0
void ZPGSimulator::GetChoices(std::vector<unsigned int>& choices)
{
#if AUTO_CHOICE
	do
	{
#endif
		std::vector<Position> moves = curBoard.getAvailableMoves(monkPosition);
		choices.clear();

		if (curBoard.getRaked() == 1)
		{
			return;
		}

		int ny = curBoard.getColumns();
		int nx = curBoard.getRows();
		int ndir = (int)Directions::DIR_SIZE;
		for (Position p : moves)
		{
			int hash = ndir * nx*ny*(ny * monkPosition.location.x + monkPosition.location.y) + ndir * (ny * p.location.x + p.location.y) + (int)p.direction;
			choices.push_back(hash);
		}
#if AUTO_CHOICE
		if (choices.size() == 1)
			SetChoice(choices[0]);
	} while (choices.size() == 1);
#endif
}

void ZPGSimulator::SetChoice(unsigned int choice)
{
	int ny = curBoard.getColumns();
	int nx = curBoard.getRows();
	int ndir = (int)Directions::DIR_SIZE;

	choice = choice % (nx*ny*ndir);
	int y = (choice/ndir) % ny;
	int x = (choice/ndir) / ny;
	Position pos;
	pos.location.x = x;
	pos.location.y = y;
	pos.direction = (Directions)(choice % ndir);
	MakeMove(pos);
}

void ZPGSimulator::MakeMove(Position selectedMove)
{
	std::shared_ptr<Tile> pTile = curBoard.getTile(monkPosition.location);

	if (pTile)
	{
		if (pTile->getElement() == Elements::PERIMETER || pTile->getElement() == Elements::USED_PERIMETER
			|| pTile->getElement() == Elements::INACTIVE)
		{
			perimeterMove(pTile, selectedMove);
			inBoardMove(curBoard.getAdjacent(selectedMove.location, monkPosition.direction), { selectedMove.location, monkPosition.direction });
		}
		else
		{
			monkPosition.direction = selectedMove.direction;
			inBoardMove(curBoard.getTile(selectedMove.location), monkPosition);
			monkPosition = selectedMove;
		}
	}
}

void ZPGSimulator::perimeterMove(std::shared_ptr<Tile> pTile, Position move)
{
	if (pTile)
	{
		++totalMoves;
		curBoard.rakeTile(monkPosition);

		monkPosition = move;

		if (move.direction == Directions::NORTH)
		{
			monkPosition.location.x = monkPosition.location.x - 1;
		}
		else if (move.direction == Directions::SOUTH)
		{
			monkPosition.location.x = monkPosition.location.x + 1;
		}
		else if (move.direction == Directions::WEST)
		{
			monkPosition.location.y = monkPosition.location.y - 1;
		}
		else if (move.direction == Directions::EAST)
		{
			monkPosition.location.y = monkPosition.location.y + 1;
		}
	}
}

void ZPGSimulator::inBoardMove(std::shared_ptr<Tile> pTile, Position move)
{
	if (pTile)
	{
		if (pTile->getElement() == Elements::STATUE)
		{
			curBoard.moveStatue(move);
		}
		else if (pTile->getElement() == Elements::YELLOW_LEAF || pTile->getElement() == Elements::ORANGE_LEAF
			     || pTile->getElement() == Elements::RED_LEAF)
		{
			curBoard.collectLeaf(move);
		}
		else
		{
			curBoard.rakeTile(move);
		}
	}
}

int ZPGSimulator::GetCost(void)
{
	int cells = curBoard.getRows() * curBoard.getColumns();
	int cost = (int)(cells*(1.0f - curBoard.getRaked()))*100  + totalMoves + curBoard.getRemainingLeaves();
	return cost;
}

Simulator* ZPGSimulator::Copy()
{
	return new ZPGSimulator(startBoard);
}

std::string ZPGSimulator::StateAsString()
{
	return curBoard.printBoard();
}

bool ZPGSimulator::NeedsStartVal() 
{ 
	return false; 
}

void ZPGSimulator::ReadSolution(const std::string& solFile)
{
}

bool ZPGSimulator::HasSolution() const
{
	return false;
}

bool ZPGSimulator::ComponentInSolution(unsigned int component)
{
 	return true;
}
