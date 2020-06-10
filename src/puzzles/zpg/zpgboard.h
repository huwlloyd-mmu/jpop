#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include "Tile.h"
#include "PerimeterTile.h"
#include "CoOrdinates.h"
#include "Position.h"

class ZPGBoard
{
private:
	int rows;
	int columns;
	std::vector<std::vector<std::shared_ptr<Tile>>> gameBoard;
	std::vector<std::shared_ptr<PerimeterTile>> perimeterTiles;
	int unrakedSquares;
	int rakedSquares;
	int yellowLeaves;
	int orangeLeaves;
	int redLeaves;

public:
	ZPGBoard();
	~ZPGBoard();
	ZPGBoard(const std::string &fileName);
	void createBoard(std::ifstream& inputFile);
	std::string printBoard();
	std::vector<std::vector<std::shared_ptr<Tile>>> getBoard();
	std::shared_ptr<Tile> getTile(CoOrdinates coOrdinates);
	void rakeTile(Position position);
	void moveStatue(Position position);
	void collectLeaf(Position position);
	int getRows();
	int getColumns();
	std::vector<Position> getAvailableMoves(Position monkPosition);
	void getPerimeterMoves(Position monkPosition, std::vector<Position>& moves);
	void getInBoardMoveChoice(Position potentialMove, std::vector<Position>& moves);
	void getInBoardMoveNoChoice(Position monkPosition, std::vector<Position>& moves);
	bool checkOnPerimeter(CoOrdinates monkLocation);
	float getRaked();
	std::shared_ptr<Tile> getAdjacent(CoOrdinates currentSquare, Directions direction);
	int getRemainingLeaves();
	void setPerimeterDirections();
	void Copy(const ZPGBoard &other);
	//bool equal(ZPGBoard board);
	void printBoardWithMonk(CoOrdinates monkLocation);
};