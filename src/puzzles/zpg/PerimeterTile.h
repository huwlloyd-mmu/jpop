#pragma once
#include "Tile.h"
#include "Directions.h"
#include "Elements.h"
#include "CoOrdinates.h"

class PerimeterTile : public Tile
{
private:
	Directions boardDirection;
	CoOrdinates boardLocation;

public:
	PerimeterTile();
	PerimeterTile(Elements element);
	void setBoardDirection(Directions direction);
	Directions getBoardDirection();
	void setBoardLocation(CoOrdinates location);
	CoOrdinates getBoardLocation();
	char printTile() override;
};