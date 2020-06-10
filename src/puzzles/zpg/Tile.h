#pragma once
#include "Elements.h"
#include "Directions.h"

class Tile
{
protected:
	Elements element;
	Directions rakedDirection;

public:
	Tile();
	Tile(Elements element);
	Elements getElement() const;
	void setElement(Elements newElement);
	void rake(Directions direction);
	void setRakedDirection(Directions direction);
	Directions getRakedDirection() const;
	virtual char printTile();
};