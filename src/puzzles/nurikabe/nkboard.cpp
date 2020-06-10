#include "nkboard.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace Nurikabe;
using namespace std;

NurikabeBoard::NurikabeBoard( const char *fileName )
{
	ifstream inFile(fileName);
	islandTotal = 0;

	inFile >> xSize >> ySize;
	cells = new Cell[xSize*ySize];
	int curCell = 0;
	int islandId = 0;
	while ( curCell < xSize*ySize )
	{
		string inString;
		inFile >> inString;

		if ( inString == string("-") )
		{
			cells[curCell].value = 0;
			cells[curCell].color = BLACK;
			cells[curCell].islandId = -1;
			cells[curCell].cutFlag = false;
		}
		else
		{
			cells[curCell].value = stoi(inString);
			cells[curCell].color = WHITE;
			cells[curCell].islandId = islandId++;
			cells[curCell].cutFlag = false;
			numberedCells.push_back(curCell);
			islandTotal += cells[curCell].value;
		}
		++curCell;
	}
	//printf("found %d numbered cells out of %d\n",(int)numberedCells.size(), xSize*ySize );
	fflush(stdout);
}

string NurikabeBoard::AsString() const
{
	ostringstream outString;
	for ( int j = 0; j < ySize; j++ )
	{
		for ( int i = 0; i < xSize; i++ )
		{
			Cell *cell = cells+toIndex(i,j);
			if ( cell->value != 0 )
				outString << " " << setw(2) << cell->value << " ";
			else if ( cell->color == WHITE )
				outString << " .. ";
			else if ( cell->color == BLACK )
				outString << " ## ";
			else
				outString << " ++ ";
		}
		outString << endl;
	}
	return outString.str();
}

void NurikabeBoard::Print() const
{
	cout << AsString();
}

void NurikabeBoard::StepTree( Vert *verts, int vert )
{
	verts[vert].discovery = treeCount++;
	verts[vert].low = verts[vert].discovery;
	verts[vert].visited = true;

	int children = 0;

	for ( int iNeighbour = 0; iNeighbour < verts[vert].nNeighbours; iNeighbour++ )
	{
		int nextVert = verts[vert].neighbours[iNeighbour];
		
		if ( !verts[nextVert].visited )
		{
			children++;
			verts[ nextVert ].parent = vert;
			StepTree( verts, nextVert );
			verts[vert].low = std::min( verts[vert].low, verts[nextVert].low );
			// check conditions for cut vertex
			if ( !verts[vert].cutVertex && ((verts[vert].parent == -1 && children > 1) || 
											   (verts[vert].parent != -1 && verts[nextVert].low >= verts[vert].discovery) ))
			{
				verts[vert].cutVertex = true;
				cells[vert].cutFlag = true;
			}
		}
		else if ( nextVert != verts[vert].parent )
			verts[vert].low = std::min(verts[vert].low, verts[nextVert].low );
	}
}


void NurikabeBoard::FlagCutVerts()
{
	int startVert = -1;
	verts = new Vert[xSize*ySize];
	
	for ( int j = 0; j < ySize; j++ )
	{
		for ( int i = 0; i < xSize; i++ )
		{
			int index = toIndex(i, j);
			cells[index].cutFlag = 0;
			if (cells[index].color == BLACK )
			{
				startVert = index;
				verts[index].visited = verts[index].cutVertex = false;
				verts[index].parent = -1;
				verts[index].nNeighbours = 0;
				if ( i != 0 && cells[toIndex(i-1,j)].color == BLACK )
					verts[index].neighbours[verts[index].nNeighbours++] = toIndex(i-1,j);
				if ( j != 0 && cells[toIndex(i,j-1)].color == BLACK )
					verts[index].neighbours[verts[index].nNeighbours++] = toIndex(i,j-1);
				if ( i != xSize-1 && cells[toIndex(i+1,j)].color == BLACK )
					verts[index].neighbours[verts[index].nNeighbours++] = toIndex(i+1,j);
				if ( j != ySize-1 && cells[toIndex(i,j+1)].color == BLACK )
					verts[index].neighbours[verts[index].nNeighbours++] = toIndex(i,j+1);
			}
		}
	}
	treeCount = 0;
	StepTree( verts, startVert );
	delete [] verts;
}

void NurikabeBoard::Copy( const NurikabeBoard &other )
{
	xSize = other.xSize;
	ySize = other.ySize;
	if ( cells != nullptr )
		delete [] cells;
	
	cells = new Cell[xSize*ySize];
	islandTotal = other.islandTotal;
	
	for ( int i = 0; i < xSize*ySize; i++ )
	{
		cells[i] = other.cells[i];
	}
	numberedCells.clear();
	for ( int n : other.numberedCells )
		numberedCells.push_back( n );
	islandTotal = other.islandTotal;
}

void NurikabeBoard::GetNeighbours( int iCell, vector<int> &neighbours )
{
	int i, j;
	i = iCell%xSize;
	j = iCell/xSize;
	
	neighbours.clear();
	if ( i > 0 )
		neighbours.push_back( toIndex(i-1, j ));
	if ( j > 0 )
		neighbours.push_back( toIndex(i, j-1 ));
	if ( i < xSize-1 )
		neighbours.push_back( toIndex(i+1, j ));
	if ( j < ySize-1 )
		neighbours.push_back( toIndex(i, j+1 ));
}

