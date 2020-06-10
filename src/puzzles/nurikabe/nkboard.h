#pragma once
#include <vector>
#include <string>

namespace Nurikabe
{
	typedef enum
	{
		WHITE,
		BLACK,
	} Color;
	
	typedef struct
	{
		int color;
		int value;
		bool cutFlag;
		int islandId;
	} Cell;

	class NurikabeBoard
	{
		int xSize, ySize;
		Cell *cells = nullptr;
		std::vector<int> numberedCells;
		int islandTotal;
		
		// cut vertex stuff
		typedef struct
		{
			int neighbours[4];
			int nNeighbours;
			bool visited;
			bool cutVertex;
			int low;
			int discovery;
			int parent;
		} Vert;
		void StepTree( Vert *verts, int vert );
		int treeCount;
		Vert *verts;
		// end cut vertex stuff
	public:
		NurikabeBoard(){}
		NurikabeBoard( const char *fileName );
		NurikabeBoard( const NurikabeBoard &other ); 
		
		~NurikabeBoard() {}

		void Copy( const NurikabeBoard &other );
		void FlagCutVerts();
		int GetXSize() const { return xSize; }
		int GetYSize() const { return ySize; }
		int GetNumIslands() const { return (int)numberedCells.size(); }
		int GetNum( int i ) const { return (int)numberedCells[i]; };
		int GetNumCells() const { return xSize*ySize; }
		const std::vector<int>& GetNumberedCells() const { return numberedCells; }
		int GetIslandTotal() const { return islandTotal; }
		const Cell& GetCell( int i, int j ) const { return GetCell(toIndex(i,j));}
		const Cell& GetCell( int index ) const { return cells[index]; }
		void Print() const;
		std::string AsString() const;
		int toIndex(int i, int j ) const { return j*xSize+i; }
		void GetNeighbours( int iCell, std::vector<int> &neighbours );
		void SetCell( int iCell, int islandId ) { cells[iCell].color = WHITE; cells[iCell].islandId = islandId; }
	};
} 
	
