#include "zpgboard.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

ZPGBoard::ZPGBoard()
{
	yellowLeaves = 0;
	orangeLeaves = 0;
	redLeaves = 0;
	unrakedSquares = 0;
	rakedSquares = 0;
	rows = 0;
	columns = 0;
}

ZPGBoard::ZPGBoard( const std::string& fileName )
{
	yellowLeaves = 0;
	orangeLeaves = 0;
	redLeaves = 0;
	unrakedSquares = 0;
	rakedSquares = 0;
	rows = 0;
	columns = 0;
	std::ifstream ifs(fileName);

	createBoard(ifs);
}

ZPGBoard::~ZPGBoard()
{
	for (std::shared_ptr<PerimeterTile>& pTile : perimeterTiles)
	{
		pTile.reset();
		pTile = nullptr;
	}

	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < columns; y++)
		{
			gameBoard[x][y].reset();
			gameBoard[x][y] = nullptr;
		}
	}

	std::vector<std::shared_ptr<PerimeterTile>>::iterator iterator1 = std::remove(perimeterTiles.begin(), perimeterTiles.end(), nullptr);
	perimeterTiles.erase(iterator1, perimeterTiles.end());

	for (int i = 0; i < rows; i++)
	{
		std::vector<std::shared_ptr<Tile>>::iterator iterator2 = std::remove(gameBoard[i].begin(), gameBoard[i].end(), nullptr);
		gameBoard[i].erase(iterator2, gameBoard[i].end());
	}
}

void ZPGBoard::createBoard(std::ifstream& inputFile)
{
	char nextChar = ' ';
	std::vector<std::shared_ptr<Tile>> row;

	while (inputFile.get(nextChar))
	{
		if (nextChar == '\n')
		{
			gameBoard.push_back(row);
			row.clear();
			continue;
		}
		else if (nextChar == 'X')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::INACTIVE);
			row.push_back(pTile);
		}
		else if (nextChar == '#')
		{
			std::shared_ptr<PerimeterTile> pTile = std::make_shared<PerimeterTile>(Elements::PERIMETER);
			pTile->setBoardLocation({ (int)gameBoard.size() , (int)row.size() });

			row.push_back(pTile);
			perimeterTiles.push_back(pTile);
		}
		else if (nextChar == 'O')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::ROCK);
			row.push_back(pTile);
		}
		else if (nextChar == '.')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::UNRAKED_SQUARE);
			row.push_back(pTile);
			unrakedSquares++;
		}
		else if (nextChar == '@')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::STATUE);
			row.push_back(pTile);
			unrakedSquares++;
			rakedSquares++;
		}
		else if(nextChar == 'Y')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::YELLOW_LEAF);
			row.push_back(pTile);
			unrakedSquares++;
			yellowLeaves++;
		}
		else if (nextChar == 'E')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::ORANGE_LEAF);
			row.push_back(pTile);
			unrakedSquares++;
			orangeLeaves++;
		}
		else if (nextChar == 'R')
		{
			std::shared_ptr<Tile> pTile = std::make_shared<Tile>(Elements::RED_LEAF);
			row.push_back(pTile);
			unrakedSquares++;
			redLeaves++;
		}
		else
		{
			if (!isspace(nextChar))
			{
				std::cout << "Unkown character in input file " << nextChar << std::endl;
			}
		}
	}

	gameBoard.push_back(row);
	row.clear();

	rows = gameBoard.size();
	columns = gameBoard[0].size();
	
	setPerimeterDirections();
}

void ZPGBoard::setPerimeterDirections()
{
	unsigned int size = perimeterTiles.size();
	for (unsigned int i = 0; i < size; i++)
	{
		std::shared_ptr<Tile> pTile = getAdjacent(perimeterTiles[i]->getBoardLocation(), Directions::NORTH);
		if (pTile)
		{
			if (pTile->getElement() != Elements::USED_PERIMETER && pTile->getElement() != Elements::PERIMETER
				&& pTile->getElement() != Elements::INACTIVE)
			{
				perimeterTiles[i]->setBoardDirection(Directions::NORTH);
				continue;
			}
		}

		pTile = getAdjacent(perimeterTiles[i]->getBoardLocation(), Directions::SOUTH);
		if (pTile)
		{
			if (pTile->getElement() != Elements::USED_PERIMETER && pTile->getElement() != Elements::PERIMETER
				&& pTile->getElement() != Elements::INACTIVE)
			{
				perimeterTiles[i]->setBoardDirection(Directions::SOUTH);
				continue;
			}
		}

		pTile = getAdjacent(perimeterTiles[i]->getBoardLocation(), Directions::WEST);
		if (pTile)
		{
			if (pTile->getElement() != Elements::USED_PERIMETER && pTile->getElement() != Elements::PERIMETER
				&& pTile->getElement() != Elements::INACTIVE)
			{
				perimeterTiles[i]->setBoardDirection(Directions::WEST);
				continue;
			}
		}

		pTile = getAdjacent(perimeterTiles[i]->getBoardLocation(), Directions::EAST);
		if (pTile)
		{
			if (pTile->getElement() != Elements::USED_PERIMETER && pTile->getElement() != Elements::PERIMETER
				&& pTile->getElement() != Elements::INACTIVE)
			{
				perimeterTiles[i]->setBoardDirection(Directions::EAST);
				continue;
			}
		}
	}
}

std::string ZPGBoard::printBoard()
{
	std::ostringstream ss;

	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < columns; y++)
		{
			ss << gameBoard[x][y]->printTile();
		}
		ss << std::endl;
	}
	return ss.str();
}

void ZPGBoard::printBoardWithMonk(CoOrdinates monkLocation)
{
	std::cout << "\n";

	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < columns; y++)
		{
			if (x == monkLocation.x && y == monkLocation.y)
			{
				std::cout << '*';
				continue;
			}
			std::cout << gameBoard[x][y]->printTile();
		}
		std::cout << "\n";
	}
}

std::vector<Position> ZPGBoard::getAvailableMoves(Position monkPosition)
{
	std::vector<Position> moves;

	std::shared_ptr<Tile> pStartTile = gameBoard[monkPosition.location.x][monkPosition.location.y];
	std::shared_ptr<Tile> pAdjacentStartTile = getAdjacent(monkPosition.location, monkPosition.direction);

	if (pStartTile)
	{
		if (pStartTile->getElement() == Elements::PERIMETER || pStartTile->getElement() == Elements::USED_PERIMETER
			|| pStartTile->getElement() == Elements::INACTIVE)
		{
			getPerimeterMoves(monkPosition, moves);
		}
		else if (pAdjacentStartTile &&
			(pAdjacentStartTile->getElement() == Elements::UNRAKED_SQUARE
				|| pAdjacentStartTile->getElement() == Elements::PERIMETER
				|| pAdjacentStartTile->getElement() == Elements::USED_PERIMETER
				|| pAdjacentStartTile->getElement() == Elements::YELLOW_LEAF
				|| (pAdjacentStartTile->getElement() == Elements::ORANGE_LEAF && yellowLeaves == 0)
				|| (pAdjacentStartTile->getElement() == Elements::RED_LEAF && yellowLeaves == 0 && orangeLeaves == 0)))
		{
			getInBoardMoveNoChoice(monkPosition, moves);
		}
		else
		{
			Position potentialMove;
			potentialMove.location.x = monkPosition.location.x + 1;
			potentialMove.location.y = monkPosition.location.y;
			potentialMove.direction = Directions::SOUTH;
			getInBoardMoveChoice(potentialMove, moves);

			potentialMove.location.x = monkPosition.location.x - 1;
			potentialMove.direction = Directions::NORTH;
			getInBoardMoveChoice(potentialMove, moves);

			potentialMove.location.x = monkPosition.location.x;
			potentialMove.location.y = monkPosition.location.y + 1;
			potentialMove.direction = Directions::EAST;
			getInBoardMoveChoice(potentialMove, moves);

			potentialMove.location.y = monkPosition.location.y - 1;
			potentialMove.direction = Directions::WEST;
			getInBoardMoveChoice(potentialMove, moves);
		}
	}

	return moves;
}

void ZPGBoard::getPerimeterMoves(Position monkPosition, std::vector<Position>& moves)
{
	unsigned int size = perimeterTiles.size();
	for (unsigned int i = 0; i < size; i++)
	{
		std::shared_ptr<PerimeterTile> pTile = perimeterTiles[i];
		std::shared_ptr<Tile> pAdjacentTile = getAdjacent(pTile->getBoardLocation(), pTile->getBoardDirection());

		if (pTile && pAdjacentTile)
		{
			if (pAdjacentTile->getElement() == Elements::UNRAKED_SQUARE)
			{
				Position newPosition;
				newPosition.location = pTile->getBoardLocation();
				newPosition.direction = pTile->getBoardDirection();
				moves.push_back(newPosition);
			}
			else if (pAdjacentTile->getElement() == Elements::STATUE)
			{
				CoOrdinates statueLocation = pTile->getBoardLocation();

				if (pTile->getBoardDirection() == Directions::NORTH)
				{
					statueLocation.x = statueLocation.x - 1;
				}
				if (pTile->getBoardDirection() == Directions::SOUTH)
				{
					statueLocation.x = statueLocation.x + 1;
				}
				if (pTile->getBoardDirection() == Directions::EAST)
				{
					statueLocation.y = statueLocation.y + 1;
				}
				if (pTile->getBoardDirection() == Directions::WEST)
				{
					statueLocation.y = statueLocation.y - 1;
				}

				pAdjacentTile = getAdjacent(statueLocation, pTile->getBoardDirection());
				if (pAdjacentTile)
				{
					if (pAdjacentTile->getElement() == Elements::UNRAKED_SQUARE)
					{
						Position newPosition;
						newPosition.location = pTile->getBoardLocation();
						newPosition.direction = pTile->getBoardDirection();
						moves.push_back(newPosition);
					}
				}
			}
			else if (pAdjacentTile->getElement() == Elements::YELLOW_LEAF)
			{
				Position newPosition;
				newPosition.location = pTile->getBoardLocation();
				newPosition.direction = pTile->getBoardDirection();
				moves.push_back(newPosition);
			}
			else if (pAdjacentTile->getElement() == Elements::ORANGE_LEAF)
			{
				if (yellowLeaves == 0)
				{
					Position newPosition;
					newPosition.location = pTile->getBoardLocation();
					newPosition.direction = pTile->getBoardDirection();
					moves.push_back(newPosition);
				}
			}
			else if (pAdjacentTile->getElement() == Elements::RED_LEAF)
			{
				if (yellowLeaves == 0 && orangeLeaves == 0)
				{
					Position newPosition;
					newPosition.location = pTile->getBoardLocation();
					newPosition.direction = pTile->getBoardDirection();
					moves.push_back(newPosition);
				}
			}
		}
	}
}

void ZPGBoard::getInBoardMoveNoChoice(Position monkPosition, std::vector<Position>& moves)
{
	Position newPosition;
	newPosition.direction = monkPosition.direction;

	if (monkPosition.direction == Directions::NORTH)
	{
		newPosition.location.x = monkPosition.location.x - 1;
		newPosition.location.y = monkPosition.location.y;
	}
	else if (monkPosition.direction == Directions::SOUTH)
	{
		newPosition.location.x = monkPosition.location.x + 1;
		newPosition.location.y = monkPosition.location.y;
	}
	else if (monkPosition.direction == Directions::EAST)
	{
		newPosition.location.y = monkPosition.location.y + 1;
		newPosition.location.x = monkPosition.location.x;
	}
	else if (monkPosition.direction == Directions::WEST)
	{
		newPosition.location.y = monkPosition.location.y - 1;
		newPosition.location.x = monkPosition.location.x;
	}

	moves.push_back(newPosition);
}

void ZPGBoard::getInBoardMoveChoice(Position potentialMove, std::vector<Position>& moves)
{
	std::shared_ptr<Tile> pTile = gameBoard[potentialMove.location.x][potentialMove.location.y];

	if (pTile)
	{
		if (pTile->getElement() == Elements::UNRAKED_SQUARE	|| pTile->getElement() == Elements::PERIMETER)
		{
			moves.push_back(potentialMove);
		}
		else if (pTile->getElement() == Elements::STATUE)
		{
			CoOrdinates adjacent = { potentialMove.location.x, potentialMove.location.y };
			std::shared_ptr<Tile> pAdjacentTile = getAdjacent(adjacent, potentialMove.direction);

			if (pAdjacentTile)
			{
				if (pAdjacentTile->getElement() == Elements::UNRAKED_SQUARE)
				{
					moves.push_back(potentialMove);
				}
			}
		}
		else if (pTile->getElement() == Elements::YELLOW_LEAF)
		{
			moves.push_back(potentialMove);
		}
		else if (pTile->getElement() == Elements::ORANGE_LEAF)
		{
			if (yellowLeaves == 0)
			{
				moves.push_back(potentialMove);
			}
		}
		else if (pTile->getElement() == Elements::RED_LEAF)
		{
			if (yellowLeaves == 0 && orangeLeaves == 0)
			{
				moves.push_back(potentialMove);
			}
		}
	}
}

std::vector<std::vector<std::shared_ptr<Tile>>> ZPGBoard::getBoard()
{
	return gameBoard;
}

std::shared_ptr<Tile> ZPGBoard::getTile(CoOrdinates coOrdinates)
{
	return gameBoard[coOrdinates.x][coOrdinates.y];
}

void ZPGBoard::rakeTile(Position position)
{
	std::shared_ptr<Tile> pTile = gameBoard[position.location.x][position.location.y];
	if (pTile)
	{
		if (pTile->getElement() == Elements::UNRAKED_SQUARE || pTile->getElement() == Elements::YELLOW_LEAF
			|| pTile->getElement() == Elements::ORANGE_LEAF || pTile->getElement() == Elements::RED_LEAF)
		{
			rakedSquares++;
		}
		gameBoard[position.location.x][position.location.y]->rake(position.direction);
	}
}

void ZPGBoard::moveStatue(Position position)
{
	CoOrdinates targetSquare = position.location;
	if (position.direction == Directions::NORTH)
	{
		targetSquare.x = position.location.x - 1;
	}
	else if (position.direction == Directions::SOUTH)
	{
		targetSquare.x = position.location.x + 1;
	}
	else if (position.direction == Directions::EAST)
	{
		targetSquare.y = position.location.y + 1;
	}
	else if (position.direction == Directions::WEST)
	{
		targetSquare.y = position.location.y - 1;
	}

	if (getAdjacent(targetSquare, position.direction) && getTile(targetSquare))
	{
		getAdjacent(targetSquare, position.direction)->setElement(Elements::STATUE);
		getTile(targetSquare)->setElement(Elements::UNRAKED_SQUARE);
		rakeTile(position);
	}
}

void ZPGBoard::collectLeaf(Position position)
{
	std::shared_ptr<Tile> pTile = getAdjacent(position.location, position.direction);

	if (pTile)
	{
		if (pTile->getElement() == Elements::YELLOW_LEAF)
		{
			yellowLeaves--;
		}
		else if (pTile->getElement() == Elements::ORANGE_LEAF)
		{
			orangeLeaves--;
		}
		else
		{
			redLeaves--;
		}

		rakeTile(position);
	}
}

bool ZPGBoard::checkOnPerimeter(CoOrdinates monkLocation)
{
	std::shared_ptr<Tile> pTile = gameBoard[monkLocation.x][monkLocation.y];

	if (pTile)
	{
		return pTile->getElement() == Elements::PERIMETER || pTile->getElement() == Elements::USED_PERIMETER 
				|| pTile->getElement() == Elements::INACTIVE;
	}
	else
	{
		return false;
	}
}

int ZPGBoard::getRows()
{
	return rows;
}

int ZPGBoard::getColumns()
{
	return columns;
}

float ZPGBoard::getRaked()
{
	return float(rakedSquares) / float(unrakedSquares);
}

std::shared_ptr<Tile> ZPGBoard::getAdjacent(CoOrdinates currentSquare, Directions direction)
{
	if (direction == Directions::NORTH)
	{
		currentSquare.x = currentSquare.x - 1;
	}
	else if (direction == Directions::SOUTH)
	{
		currentSquare.x = currentSquare.x + 1;
	}
	else if (direction == Directions::EAST)
	{
		currentSquare.y = currentSquare.y + 1;
	}
	else if (direction == Directions::WEST)
	{
		currentSquare.y = currentSquare.y - 1;
	}

	if (currentSquare.x < 0 || currentSquare.y < 0 || currentSquare.x >= rows || currentSquare.y >= columns)
	{
		return nullptr;
	}
	else
	{
		return getTile(currentSquare);
	}
}

int ZPGBoard::getRemainingLeaves()
{
	return yellowLeaves + orangeLeaves + redLeaves;
}

void ZPGBoard::Copy( const ZPGBoard &other)
{
	columns = other.columns;
	rows = other.rows;
	unrakedSquares = other.unrakedSquares;
	rakedSquares = other.rakedSquares;
	yellowLeaves = other.yellowLeaves;
	orangeLeaves = other.orangeLeaves;
	redLeaves = other.redLeaves;

	gameBoard.clear();
	perimeterTiles.clear();

	for (int x = 0; x < other.rows; x++)
	{
		std::vector<std::shared_ptr<Tile>> newRow;
		gameBoard.push_back(newRow);

		for (int y = 0; y < other.columns; y++)
		{
			std::shared_ptr<Tile> pTile = other.gameBoard[x][y];

			if (pTile)
			{
				if (pTile->getElement() == Elements::PERIMETER
					|| pTile->getElement() == Elements::USED_PERIMETER)
				{
					std::shared_ptr<PerimeterTile> pNewTile = std::make_shared<PerimeterTile>();

					pNewTile->setElement(pTile->getElement());
					pNewTile->setRakedDirection(pTile->getRakedDirection());
					pNewTile->setBoardLocation({ x,y });

					gameBoard[x].push_back(pNewTile);
					perimeterTiles.push_back(pNewTile);
				}
				else
				{
					std::shared_ptr<Tile> pNewTile = std::make_shared<Tile>();
					pNewTile->setElement(pTile->getElement());
					pNewTile->setRakedDirection(pTile->getRakedDirection());

					gameBoard[x].push_back(pNewTile);
				}
			}
		}
	}
	setPerimeterDirections();
}
