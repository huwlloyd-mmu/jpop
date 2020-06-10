#include "slboard.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace Slitherlink;

SLBoard::SLBoard( const std::string& fileName )
{
    // read board from text file
	std::ifstream inFile;
	inFile.open( fileName );

	if (inFile.is_open() )
	{
		std::vector<std::string> lines;
		std::string line;
		while (std::getline( inFile, line ) )
		{
			// strip carriage returns and spaces
			line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
			line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
			lines.push_back(line);
		}
		inFile.close();

		nx = (int)lines[0].length(); // length of a line
		ny = (int)lines.size();      // number of lines

		cellValues = new int[nx*ny];
		cellValues0 = new int[nx*ny];
		nodesVisited = new bool[(nx+1)*(ny+1)];

		for (int j=0; j<ny; j++)
		{
			for (int i=0; i<nx; i++)
			{
				char c=lines[j][i];
				if (c == '.' || c == '-')
					SetCellValue(i, j, -1);
				else
				{
					if ( c < '0' || c > '9' )
					{
						std::cout << "found dodgy character: " << c <<std::endl;
					}
					SetCellValue(i, j, (int)(c-'0'));
				}
			}
		}
		std::memset( nodesVisited, 0, (nx+1)*(ny+1)*sizeof(bool));
		std::memcpy( cellValues0, cellValues, nx*ny*sizeof(int));
	}
}

void SLBoard::MakeMove( const Point& end )
{
	Point start = tour.back();
	tour.push_back(end);
	SetNodeVisited(end.x, end.y, true);
	
	// open the first node again after a few moves
	if (tour.size() == 4)
		SetNodeVisited(tour[0].x, tour[0].y, false);
	// decrement the counter in the adjacent cells
	if ( start.y == end.y ) // horizontal edge
	{
		int cellx = std::min(start.x, end.x);
		int y = start.y;
		if ( y < ny && GetCellValue(cellx,y) != -1 )
			SetCellValue(cellx, y, GetCellValue(cellx, y)-1 );
		if ( y > 0 && GetCellValue(cellx, y-1) != -1 )
			SetCellValue( cellx, y-1, GetCellValue(cellx, y-1)-1 );
	}
	else
	{
		int celly = std::min(start.y, end.y);
		int x = start.x;
		if ( x < nx && GetCellValue(x, celly) != -1 )
			SetCellValue( x, celly, GetCellValue(x, celly)-1 );
		if ( x > 0 && GetCellValue(x-1, celly) != -1 )
			SetCellValue( x-1, celly, GetCellValue(x-1, celly)-1 );
	}
}

bool SLBoard::CanMove( const Point& start, const Point& end ) const
{

	if ( end.y > ny || end.x > nx || end.y < 0 || end.x < 0 ) // off the board
		return false;

	if ( GetNodeVisited(end.x, end.y) ) // already visited
		return false;

	// now check neighbouring cells to the edge we want to traverse. If any have a
	// zero value, the move is disallowed
	if ( start.y == end.y ) // horizontal edge
	{
		int cellx = std::min(start.x, end.x);
		int y = start.y;
		if ( y < ny && GetCellValue(cellx,y) == 0 )
			return false;
		if ( y > 0 && GetCellValue(cellx,y-1) == 0 )
			return false;
	}
	else
	{
		int celly = std::min(start.y, end.y);
		int x = start.x;
		if ( x < nx && GetCellValue(x,celly) == 0 )
			return false;
		if ( x > 0 && GetCellValue(x-1,celly) == 0 )
			return false;
	}

	return true;
}

void SLBoard::GetMoves( Point* moves, int &nMoves )
{
	nMoves=0;
	// check for closed loop in which case offer no moves
	if (tour.size() > 1 && tour[0] == tour.back())
	{
		return;
	}
	Point start = tour.back();
	if ( CanMove( start, Point( start.x-1, start.y ) ))
		 moves[nMoves++] = Point(start.x-1, start.y);
	if ( CanMove( start, Point( start.x+1, start.y ) ))
		 moves[nMoves++] = Point(start.x+1, start.y);
	if ( CanMove( start, Point( start.x, start.y-1 ) ))
		 moves[nMoves++] = Point(start.x, start.y-1);
	if ( CanMove( start, Point( start.x, start.y+1 ) ))
		 moves[nMoves++] = Point(start.x, start.y+1);
#if 0
	// if there are no moves, set the start point to not visited and try again
	if ( nMoves == 0 && GetNodeVisited( tour[0].x, tour[0].y ) )
	{
		SetNodeVisited( tour[0].x, tour[0].y, false );
		GetMoves(moves, nMoves);
	}
#endif
}

int SLBoard::ScoreTour() const
{
	// add up all the cell values
	int total = 0;
	for ( int i = 0; i < nx; i++ )
	{
		for ( int j = 0; j < ny; j++ )
		{
			if ( GetCellValue(i, j) != -1 )
				total += GetCellValue(i, j);
		}
	}
	// penalty if loop is not closed
	if ( !(tour[0] == tour.back()) )
		total += 100;
	return total;
}

void SLBoard::InitTour( const Point& start )
{
	tour.clear();
	tour.push_back(start);
	// restore cell values
	std::memcpy( cellValues, cellValues0, nx*ny*sizeof(int));
	SetNodeVisited(start.x, start.y, true);
}

SLBoard::SLBoard( const SLBoard& other )
{
	Copy(other);
}

void SLBoard::Reset()
{
	std::memcpy( cellValues, cellValues0, nx*ny*sizeof(int));	
	std::memset( nodesVisited, 0, (nx+1)*(ny+1)*sizeof(bool));
}

void SLBoard::Copy( const SLBoard& other )
{
	// if not already alllocated or the sizes are different, allocate the memory, otherwise recycle
	if ( cellValues == nullptr || nx != other.nx || ny != other.ny )
	{
		if ( cellValues != nullptr )
		{
			delete [] cellValues;
			delete [] cellValues0;
			delete [] nodesVisited;
		}
		cellValues = new int[other.nx*other.ny];
		cellValues0 = new int[other.nx*other.ny];
		nodesVisited = new bool[(other.ny+1)*(other.nx+1)];
	}
	nx = other.nx;
	ny = other.ny;
	std::memcpy( cellValues, other.cellValues, nx*ny*sizeof(int));
	std::memcpy( cellValues0, other.cellValues0, nx*ny*sizeof(int));
	std::memcpy( nodesVisited, other.nodesVisited, (nx+1)*(ny+1)*sizeof(bool));


}

std::string SLBoard::AsString() const
{
	bool *hlines = new bool[(nx+1)*(ny+1)];
	bool *vlines = new bool[(nx+1)*(ny+1)];
	std::memset( hlines, 0, (nx+1)*(ny+1)*sizeof(bool) );
	std::memset( vlines, 0, (nx+1)*(ny+1)*sizeof(bool) );
	std::ostringstream outString;

	if ( tour.size() > 1 )
	{
		for ( int i = 0; i < tour.size()-1; i++ )
		{
			Point t0, t1;
			t0 = tour[i];
			t1 = tour[i+1];
			if ( t0.x == t1.x )
			{
				// vertical edge
				int x, y;
				x = t0.x;
				y = std::min(t0.y, t1.y);
				vlines[x*(ny+1)+y] = true;
			}
			else
			{
				// horizontal edge
				int x, y;
				y = t0.y;
				x = std::min(t0.x, t1.x);
				hlines[x*(ny+1)+y] = true;
			}
		}
	}
	for ( int j = 0; j < ny; j++ )
	{
		// top edge
		for ( int i = 0; i < nx; i++ )
		{
			outString << ".";
			if ( hlines[i*(ny+1)+j] )
				outString << "_";
		    else
				outString << " ";
		}
		// right edge
		outString << "." << std::endl;
		for ( int i = 0; i < nx; i++ )
		{
			if ( vlines[i*(ny+1)+j] )
				outString << "|";
			else
				outString << " ";
			outString << (GetCellValue0(i,j) == -1 ? " " : std::to_string(GetCellValue0(i,j)));
		}
		if ( vlines[nx*(ny+1)+j] )
			outString << "|";
		outString << std::endl;
	}
	// bottom edge
	for ( int i = 0; i < nx; i++ )
	{
		outString << ".";
		if ( hlines[i*(ny+1)+ny] )
			outString << "_";
		else
			outString << " ";
	}
	delete[] hlines;
	delete[] vlines;
	outString << "." << std::endl;
	std::string retVal = outString.str();
	return retVal;
}

void SLBoard::Print() const
{
	std::cout << AsString();
}

SLBoard::~SLBoard()
{
	delete [] cellValues;
	delete [] cellValues0;
	delete [] nodesVisited;
}
