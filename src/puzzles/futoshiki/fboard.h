#pragma once
#include "../sudoku/valueset.h"
#include <string>
#include <vector>
using namespace std;

class FBoard
{
public:
	// futoshiki board
	FBoard(){};
	FBoard(const string &fileNameOrPuzzleString, bool fromFile = true);
	FBoard(const FBoard &other);
	~FBoard();

	string AsString(bool useNumbers=false, bool showUnfixed=false) const;

	int   FixedCellCount(void) const;
	int   InfeasibleCellCount(void) const;
	void  SetCellFixed(int i, const ValueSet &c, bool topLevel=false);
	void  SetCellEmpty(int i);
	const ValueSet &GetCell(int i) const;

	int  GetNumUnits() const;
	int  GetNumRels() const;
	void Copy(const FBoard &other);
	int  CellCount(void) const;
	bool CheckSolution(const FBoard& other) const;
	// helpers
	int RowCell(int iRow, int iCell) const;
	int ColCell(int iCol, int iCell) const;
	int RowForCell(int iCell) const;
	int ColForCell(int iCell) const;

private:
	ValueSet *cells = nullptr;
	bool     *fixed = nullptr;
	bool     *empty = nullptr;
	int      *grels = nullptr;
	int      *lrels = nullptr;

	int numUnits;      // number of units (rows, columns)
	int numCells;      // number of cells
	int numRels;       // number of relations
	int numFixedCells; // number of cells with uniquely determined value
	int numInfeasible; // number of cells with no possibilities.
	void ConstrainCell(int i);
	void ConstrainRowCol(int i);
	std::string ReadFile(const std::string &fileName);
	char RelString(int iCell, bool inRow) const;
	void PropagateCellRels(int iCell);
	bool CheckAll() const;
	bool CheckRels() const;
	bool CheckCellConstraints(int i) const;
	bool CheckCellRels(int i) const;
	bool CheckNumFixed() const;
	bool CheckNumInfeasible() const;
};

