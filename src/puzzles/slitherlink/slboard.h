#ifndef _SLBOARD_H_
#define _SLBOARD_H_

#include <string>
#include <cstring>
#include <vector>

namespace Slitherlink
{
	struct Point
	{
		int x, y;
		Point( int x, int y ) : x(x), y(y) {}
		Point() : x(0), y(0) {}
		bool operator==( const Point&other ) const
		{
			return x == other.x && y == other.y;
		}
	};

   class SLBoard
   {
	   // slitherlink board
   private:
	   int *cellValues = nullptr;
	   int *cellValues0 = nullptr;
	   bool *nodesVisited = nullptr; 
	   int nx, ny; // dimensions

	   void LoadFromFile( const std::string &fileName );
	   bool CanMove( const Point &start, const Point& end ) const;
	   int GetCellValue0(int i, int j) const { return cellValues0[j*nx+i]; } // used by Print
	   
	   std::vector<Point> tour;
   public:
	   SLBoard() {};
	   SLBoard( const std::string &fileName );
	   SLBoard( const SLBoard& other );
	   ~SLBoard();
	   void Copy( const SLBoard &other );
	   void GetMoves( Point *moves, int &nMoves );
	   void MakeMove( const Point& end );
	   void InitTour( const Point& startPoint );
	   int ScoreTour() const;
	   std::string AsString() const;
	   void Print() const;
	   void Reset();
	   int GetCellValue(int i, int j) const { return cellValues[j*nx+i]; } // there are nx * ny cells
	   void SetCellValue(int i, int j, int val) { cellValues[j*nx+i]=val; }
	   bool GetNodeVisited(int i, int j) const { return nodesVisited[j*(nx+1)+i]; } // there are (nx+1) * (ny+1) nodes
	   void SetNodeVisited(int i, int j, bool visited ){ nodesVisited[j*(nx+1)+i]=visited;}
	   int GetNX() const { return nx; }
	   int GetNY() const { return ny; }
   };
}
#endif
