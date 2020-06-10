#include "Tile.h"

Tile::Tile()
{
	rakedDirection = Directions::NONE;
	element = Elements::INACTIVE;
}

Tile::Tile(Elements element)
{
	this->element = element;
	rakedDirection = Directions::NONE;
}

Elements Tile::getElement() const
{
	return element;
}

void Tile::setElement(Elements newElement)
{
	element = newElement;
}

void Tile::rake(Directions direction)
{
	if (element != Elements::INACTIVE)
	{
		if (element == Elements::PERIMETER || element == Elements::USED_PERIMETER)
		{
			element = Elements::USED_PERIMETER;
		}
		else if (element == Elements::UNRAKED_SQUARE)
		{
			element = Elements::RAKED_SQUARE;
		}
		else if (element == Elements::YELLOW_LEAF)
		{
			element = Elements::RAKED_SQUARE;
		}
		else if (element == Elements::ORANGE_LEAF)
		{
			element = Elements::RAKED_SQUARE;
		}
		else if (element == Elements::RED_LEAF)
		{
			element = Elements::RAKED_SQUARE;
		}
		rakedDirection = direction;
	}
}

void Tile::setRakedDirection(Directions direction)
{
	rakedDirection = direction;
}

char Tile::printTile()
{	
	if (element == Elements::RAKED_SQUARE || element == Elements::USED_PERIMETER)
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
	else if (element == Elements::INACTIVE)
	{
		return ' ';
	}
	else if (element == Elements::PERIMETER)
	{
		return '#';
	}
	else if (element == Elements::ROCK)
	{
		return 'O';
	}
	else if (element == Elements::UNRAKED_SQUARE)
	{
		return '.';
	}
	else if (element == Elements::YELLOW_LEAF)
	{
		return 'Y';
	}
	else if (element == Elements::ORANGE_LEAF)
	{
		return 'E';
	}
	else if (element == Elements::RED_LEAF)
	{
		return 'R';
	}
	else if (element == Elements::STATUE)
	{
		return '@';
	}
	else
	{
		return '?';
	}
}

Directions Tile::getRakedDirection() const
{
	return rakedDirection;
}
