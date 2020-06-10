#include "PerimeterTile.h"

PerimeterTile::PerimeterTile(Elements element) : Tile(element)
{
	boardDirection = Directions::NONE;
	boardLocation = { 0,0 };
}

PerimeterTile::PerimeterTile() : Tile()
{
	boardDirection = Directions::NONE;
	boardLocation = { 0,0 };
}

void PerimeterTile::setBoardDirection(Directions direction)
{
	boardDirection = direction;
}

Directions PerimeterTile::getBoardDirection()
{
	return boardDirection;
}

void PerimeterTile::setBoardLocation(CoOrdinates location)
{
	boardLocation = location;
}

CoOrdinates PerimeterTile::getBoardLocation()
{
	return boardLocation;
}

char PerimeterTile::printTile()
{
	if (element == Elements::USED_PERIMETER)
	{
		if (rakedDirection == Directions::NORTH)
		{
			return '^';
		}
		else if (rakedDirection == Directions::SOUTH)
		{
			return 'v';
		}
		else if (rakedDirection == Directions::EAST)
		{
			return '>';
		}
		else if (rakedDirection == Directions::WEST)
		{
			return '<';
		}
		else
		{
			return '?';
		}
	}
	else
	{
		return '#';
	}

	/*if (boardDirection == Directions::NORTH)
	{
		return 'N';
	}
	else if (boardDirection == Directions::SOUTH)
	{
		return 'S';
	}
	else if (boardDirection == Directions::EAST)
	{
		return 'E';
	}
	else if (boardDirection == Directions::WEST)
	{
		return 'W';
	}
	else 
	{
		return '?';
	}*/
}