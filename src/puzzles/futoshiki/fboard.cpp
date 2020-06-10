#include "fboard.h"
#include <iostream>
#include <iomanip>
#include <inttypes.h>
#include <string>
#include <sstream>
#include <fstream>
#define NDEBUG 1
#include <cassert>
//
// description of a futoshiki board, with functions for setting cells and propagating constraints
//
FBoard::FBoard(const string &fileOrPuzzleString, bool fromFile) {
	string puzzleString;

	if (fromFile)
		puzzleString = ReadFile(fileOrPuzzleString);
	else
		puzzleString = fileOrPuzzleString;

	stringstream psstm;
	psstm.str(puzzleString);
	char semicolon;
	psstm >> numUnits >> semicolon;

	if (numUnits <= 0 || numUnits > 64) {
		cerr << "wrong number of cells for a futoshiki board!" << std::endl;
		numUnits = 0;
	}

	numCells   = numUnits * numUnits;
	cells      = new ValueSet[numCells];
	fixed      = new bool[numCells];
	empty      = new bool[numCells];
	int maxVal = numUnits;

	for (int i = 0; i < numCells; i++) {
		cells[i].Init(maxVal);
		fixed[i] = false;
		empty[i] = false;
	}

	// set all possibilities for all cells
	for ( int i = 0; i < numCells; i++ )
		cells[i] = ~cells[i];

	// set the known cells one by one
	numInfeasible = 0;
	numFixedCells = 0;
	for (int i = 0; i < numCells; i++) {
		char pschar;
		psstm >> pschar;
		if (pschar != '.') {
			int value;
			if (numUnits <= 9) {
				value = (int)(pschar - '0');
			} else if (numUnits <= 9) {
				if (pschar >= '0' && pschar <= '9')
					value = 1+(int)(pschar - '0');
				else
					value = 11 + (int)(pschar - 'a');
			} else {
				value = 1+(int)(pschar - 'a');
			}
			SetCellFixed(i, ValueSet(maxVal, (int64_t)1 << (value-1)), true);
		}
	}

	// get the relations
	psstm >> semicolon;
	psstm >> numRels;
	psstm >> semicolon;
	grels = new int[numRels];
	lrels = new int[numRels];
	char gt;
	for (int i=0; i<numRels; i++)
		psstm >> grels[i] >> gt >> lrels[i] >> semicolon;

	//propagate the relations (this cannot be done during earlier SetCellFixed calls)
	for (int i=0; i<numRels; i++)
		PropagateCellRels(grels[i]);
	assert(CheckAll());
}

FBoard::FBoard(const FBoard &other) {
	Copy(other);
}

void FBoard::Copy(const FBoard& other) {
	numUnits = other.numUnits;
	numCells = other.numCells;
	numRels  = other.numRels;

	if (cells == nullptr)
		cells = new ValueSet[numCells];
	if (fixed == nullptr)
		fixed = new bool[numCells];
	if (empty == nullptr)
		empty = new bool[numCells];

	int maxVal = numUnits;

	for (int i=0; i<numCells; i++) {
		cells[i] = other.cells[i];
		fixed[i] = other.fixed[i];
		assert(cells[i].Fixed() == fixed[i]);
		empty[i] = other.empty[i];
		assert(cells[i].Empty() == empty[i]);
	}

	numFixedCells = other.FixedCellCount();
	assert(CheckNumFixed());
	numInfeasible = other.InfeasibleCellCount();
	assert(CheckNumInfeasible());

	if (grels == nullptr)
		grels = new int[numRels];
	if (lrels == nullptr)
		lrels = new int[numRels];

	for (int i=0; i<numRels; i++) {
		grels[i] = other.grels[i];
		lrels[i] = other.lrels[i];
	}
	assert(CheckAll());
}

FBoard::~FBoard() {
	if (cells != nullptr) delete [] cells;
	if (fixed != nullptr) delete [] fixed;
	if (empty != nullptr) delete [] empty;
	if (grels != nullptr) delete [] grels;
	if (lrels != nullptr) delete [] lrels;
}

std::string FBoard::ReadFile(const std::string &fileName) {
 	ifstream inFile;
	inFile.open(fileName);
	if (inFile.is_open()) {
		// input grid
		int numUnits;
		inFile >> numUnits;
		int numCells = numUnits*numUnits;
		//inFile >> idum;
		char *puzString = new char[numCells + 1];
		for (int i = 0; i < numCells; i++) {
			int val;
			inFile >> val;
			if (val == -1)
				puzString[i] = '.';
			else if (numUnits <= 9)
				puzString[i] = '1' + (val - 1);
			else if (numUnits <= 16)
				if (val < 11)
					puzString[i] = '0' + val - 1;
				else
					puzString[i] = 'a' + val - 11;
			else
				puzString[i] = 'a' + val - 1;
		}
		puzString[numCells] = 0;

		// input relations
		int numRels;
		inFile >> numRels;
		int *grels = new int[numRels];
		int *lrels = new int[numRels];
		char gt;
		for (int i = 0; i < numRels; i++)
			inFile >> grels[i] >> gt >> lrels[i];

		// output grid & relations
		stringstream retVal;
		retVal << numUnits << ";" << string(puzString) << ";";
		retVal << numRels << ";";
		for (int i=0; i<numRels; i++)
			retVal << grels[i] << ">" << lrels[i] << ";";

		// tidy up
		inFile.close();
		delete[] puzString;
		delete[] grels;
		delete[] lrels;

		return retVal.str();
	} else {
		cerr << "could not open file: " << fileName << endl;
		return string();
	}
}

int FBoard::RowCell(int iRow, int iCell) const {
	// returns cell index of the iCell'th cell in the iRow'th row
	return iRow * numUnits + iCell;
}

int FBoard::ColCell(int iCol, int iCell) const {
	// returns cell index of the iCell'th cell in the iCol'th column
	return iCell * numUnits + iCol;
}

int FBoard::RowForCell(int iCell) const {
	// returns index of the row which contains cell iCell 
	return iCell / numUnits;
}

int FBoard::ColForCell(int iCell) const {
	// returns index of the column which contains cell iCell 
	return iCell % numUnits;
}

string FBoard::AsString(bool useNumbers, bool showUnfixed ) const {
	/*
	  Form a human-readable string from the board, using either numbers (1..numUnits) or a single character
	  per cell (9x9 - 1-9, 16x16 0-f, 25x25 - a-y). If showUnfixed is true, show the possibilities in
	  an unfixed cell (otherwise represent it is as '.'). If numUnits is 16 or more and showUnfixed is true,
	  force useNumbers to false for readability.
	 */

	if (grels == nullptr || lrels == nullptr)
		return "<under construction>";

	if ( showUnfixed )
		useNumbers = false;

	stringstream puzString;
	string alphabet;

	if ( !useNumbers ) {
		if ( numUnits <= 9 )
			alphabet = string("123456789");
		else if (numUnits <= 16 )
			alphabet = string("0123456789abcdef");
		else
			alphabet = string("abcdefghijklmnopqrstuvwxy");
	}

	vector<string> cellStrings;
	unsigned int maxLen = 0;
	for (int i=0; i<numCells; i++) {
		string cellContents;
		if (!useNumbers) {
			if (!showUnfixed && !cells[i].Fixed())
				cellContents = string(".");
			else
				cellContents = cells[i].toString(alphabet);
		} else
			cellContents = to_string(cells[i].Index() + 1);
		if (cellContents.size() > maxLen )
			maxLen = (int)cellContents.size();
		cellStrings.push_back(cellContents);
	}
	int pitch = maxLen+1;
	for (int row=0; row<numUnits; row++) {
		for (int col=0; col<numUnits; col++) {
			int iCell=row*numUnits+col;
			puzString << setw(pitch) << cellStrings[iCell] << setw(pitch) << RelString(iCell, true);
			if (col == numUnits-1)
				if ( iCell != numCells-1 )
					puzString << endl;
		}
		for (int col=0; col<numUnits; col++) {
			int iCell=row*numUnits+col;
			if (row != numUnits-1) {
				puzString << setw(pitch) << RelString(iCell, false);
				if (col != numUnits-1)
					puzString << setw(pitch) << " ";
				else if (col == numUnits-1)
					puzString << endl;
			}
		}
	}
	return puzString.str();
}

char FBoard::RelString(int iCell, bool inRow) const {
	// what char should follow (inRow) or be below (!inRow) this cell to represent any relations?
	for (int i=0; i<numRels; i++) {
		int loIdx = grels[i] < lrels[i] ? grels[i] : lrels[i];
		int hiIdx = grels[i] > lrels[i] ? grels[i] : lrels[i];
		if (inRow && loIdx == iCell && hiIdx == iCell+1)
			return loIdx == grels[i] ? '>' : '<';
		else if (!inRow && loIdx == iCell && hiIdx == iCell+numUnits)
			return loIdx == grels[i] ? 'v' : '^';
	}
	return ' ';
}

void FBoard::ConstrainCell(int i) {
	assert(i>=0 && i<numCells);

	if (cells[i].Empty() || cells[i].Fixed())
		return; // already empty or set

	ValueSet start = cells[i]; // cache cell
	
	int iCol, iRow;
	iCol = ColForCell(i);
	iRow = RowForCell(i);

	// set of all fixed cells in row, column
	ValueSet colFixed(numUnits), rowFixed(numUnits);
    // set of all open values in row, column not including this cell
	ValueSet colAll(numUnits), rowAll(numUnits);

	for (int j=0; j<numUnits; j++) {
		int k;
		k = ColCell(iCol, j);
		if (k != i) {
			if (cells[k].Fixed())
				colFixed += cells[k];
			colAll += cells[k];
		}
		k = RowCell(iRow, j);
		if (k != i) {
			if (cells[k].Fixed())
				rowFixed += cells[k];
			rowAll += cells[k];
		}
	}
	ValueSet fixedCellsConstraint = ~(rowFixed + colFixed);

	if (fixedCellsConstraint.Fixed()) {
		if (cells[i].Contains(fixedCellsConstraint))
			SetCellFixed(i, fixedCellsConstraint); // only one possibility left
		else
			SetCellEmpty(i);
	} else {
		// eliminate all the values already taken in this cell's units
		cells[i] ^= fixedCellsConstraint;
		if (cells[i].Fixed())
			SetCellFixed(i, cells[i]);
		// are any of the remaining values for this cell in the only possible
		// place in a unit? If so, set the cell to that value
		ValueSet rowConstraint = cells[i]-rowAll;
		ValueSet colConstraint = cells[i]-colAll;
		if (rowConstraint.Fixed())
			SetCellFixed(i, rowConstraint);
		else if (colConstraint.Fixed())
			SetCellFixed(i, colConstraint);
	}
	if (cells[i].Empty())
		SetCellEmpty(i);
	if (!(start-cells[i]).Empty())  // changed
		PropagateCellRels(i);
}

void FBoard::ConstrainRowCol(int i) {
	assert(i>=0 && i<numCells);
	// propagate the constraints
	int iCol = ColForCell(i);
	int iRow = RowForCell(i);

	for (int j=0; j<numUnits; j++) {
		int k;
		k = ColCell(iCol, j);
		if (k != i)
		    ConstrainCell(k);
		k = RowCell(iRow, j);
		if (k != i)
			ConstrainCell(k);
	}
}

void FBoard::SetCellFixed(int i, const ValueSet &c, bool topLevel) {
	assert(i>=0 && i<numCells);
	assert(c.Fixed());
	assert(cells[i].Contains(c));

	// set the cell
	cells[i] = c;

	// check for & fix constraint violations
	if (!CheckCellConstraints(i)) {
		SetCellEmpty(i);
		if (fixed[i]) { // no longer fixed
			fixed[i] = false;
			numFixedCells--;
		}
	} else { // constraints ok
		if (!fixed[i]) { // not set already
			fixed[i] = true;
			numFixedCells++;
		}
		assert(fixed[i]);
		assert(cells[i].Fixed());

		// propagate the constraints
		ConstrainRowCol(i);

		// propagate the relations
		PropagateCellRels(i);
	}
	assert(topLevel ? CheckAll() : true);
}

void FBoard::SetCellEmpty(int i) {
	assert(i>=0 && i<numCells);

	// empty the cell
	if (!empty[i]) { // not already empty
		cells[i].Clear();
		empty[i] = true;
		numInfeasible++;
	}
	assert(empty[i]);
	assert(cells[i].Empty());

	if (fixed[i]) { // no longer fixed
		fixed[i] = false;
		numFixedCells--;
	}
	assert(!fixed[i]);
}

void FBoard::PropagateCellRels(int iCell) {
	// propagate relations for cell iCell
	if (grels == nullptr || lrels == nullptr)
		return;

	for (int i=0; i<numRels; i++) {
		if (iCell == grels[i] || iCell == lrels[i]) {
			ValueSet& greater = cells[grels[i]];
			ValueSet& lesser  = cells[lrels[i]];
			if (greater.Empty() || lesser.Empty())
				continue;
			// highest in greater Cell must be greater than the lowest in lesser Cell
			if (greater.Highest() <= lesser.Lowest()) {
				// relation is broken
				SetCellEmpty(grels[i]);
				SetCellEmpty(lrels[i]);
				continue;
			}
			if (greater.Fixed() && lesser.Fixed()) // no changes can be made
				continue;

			bool changed        = false;
			bool greaterChanged = false;
			bool lesserChanged  = false;
			do {
				changed = false;
				// lowest number in greater Cell must be greater than lowest in lesser Cell
				int gl = greater.Lowest();
				if (gl <= lesser.Lowest()) {
					greater.IdxRemove(gl);
					greaterChanged = true;
					changed        = true;
					if (greater.Empty()) {
						SetCellEmpty(grels[i]);
						break;
					}
				}
				// highest number in lesser Cell must be smaller than highest in greater Cell
				int lh = lesser.Highest();
				if (lh >= greater.Highest()){
					lesser.IdxRemove(lh);
					changed       = true;
					lesserChanged = true;
					if (lesser.Empty()) {
						SetCellEmpty(lrels[i]);
						break;
					}
				}
			} while (changed);
			if (greaterChanged && greater.Fixed())
				SetCellFixed(grels[i], greater);
			if (lesserChanged && lesser.Fixed())
				SetCellFixed(lrels[i], lesser);
			if (lesserChanged) // is this enough for propagation?
				PropagateCellRels(lrels[i]);
		}
	}
}

const ValueSet& FBoard::GetCell(int i) const {
	return cells[i];
}

int FBoard::FixedCellCount(void) const {
	return numFixedCells;
}

int FBoard::InfeasibleCellCount(void) const {
	return numInfeasible;
}

int FBoard::CellCount(void) const {
	return numCells;
}

int FBoard::GetNumUnits(void) const {
	return numUnits;
}

int FBoard::GetNumRels(void) const {
	return numRels;
}

bool FBoard::CheckAll() const {
	for (int i=0; i<numCells; i++)
		if (!CheckCellConstraints(i))
			return false;
	if (!CheckRels() || !CheckNumFixed() || !CheckNumInfeasible())
		return false;
	return true;
}

bool FBoard::CheckRels() const {
	if (grels == nullptr || lrels == nullptr)
		return true;
	for (int j=0; j<numRels; j++) {
		ValueSet& greater = cells[grels[j]];
		ValueSet& lesser  = cells[lrels[j]];
		if (greater.Empty() || lesser.Empty())
			continue;
		// highest in greater Cell must be greater than the lowest in lesser Cell
		if (greater.Highest() <= lesser.Lowest())
			return false;
	}
	return true;
}

bool FBoard::CheckCellConstraints(int i) const {
	// check the validity of a cell against constraints (allows for empty cells)
	if (cells[i].Empty())
		return true;

	int iCol, iRow;
	iCol = ColForCell(i);
	iRow = RowForCell(i);
	for (int j=0; j<numUnits; j++) {
		int k;
		k = ColCell(iCol, j);
		if (k != i && cells[k].Fixed() && cells[i].Contains(cells[k]))
			return false;
		k = RowCell(iRow, j);
		if (k != i && cells[k].Fixed() && cells[i].Contains(cells[k]))
			return false;
	}
	return true;
}

bool FBoard::CheckCellRels(int i) const {
	// check the validity of a cell against relations (allows for empty cells)
	if (cells[i].Empty())
		return true;

	if (grels == nullptr || lrels == nullptr)
		return true;

	for (int j=0; j<numRels; j++) {
		if (i == grels[j] || i == lrels[j]) {
			ValueSet& greater = cells[grels[j]];
			ValueSet& lesser  = cells[lrels[j]];
			if (greater.Empty() || lesser.Empty())
				continue;
			// highest in greater Cell must be greater than the lowest in lesser Cell
			if (greater.Highest() <= lesser.Lowest())
				return false;
		}
	}
	return true;
}

bool FBoard::CheckNumFixed() const {
	// only call when at top level
	int num = 0;
	for (int i=0;i<numCells; i++) {
		assert(cells[i].Fixed() == fixed[i]);
		if (cells[i].Fixed())
			num++;
	}
	return num == numFixedCells;
}

bool FBoard::CheckNumInfeasible() const {
	// only call when at top level
	int num = 0;
	for (int i=0;i<numCells; i++) {
		assert(cells[i].Empty() == empty[i]);
		if (cells[i].Empty())
			num++;
	}
	return num == numInfeasible;
}

bool FBoard::CheckSolution(const FBoard& other) const {
	// check that other is 1/ a solution, 2/ consistent with this board
	if (other.CellCount() != CellCount())
		return false;
	bool isSolution   = true;
	bool isConsistent = true;

	// check its a solution
	// first - all cells must be filled
	for (int i = 0; i < other.CellCount(); i++) {
		if (!other.GetCell(i).Fixed())
			isSolution = false;
	}

	// second - all rows, columns must have one of each number
	for (int i = 0; i < numUnits; i++) {
		ValueSet row, col;
		row.Init(numUnits);
		col.Init(numUnits);
		for (int j = 0; j < numUnits; j++) {
			row += other.GetCell(RowCell(i, j));
			col += other.GetCell(ColCell(i, j));
		}
		if (row.Count() != numUnits || col.Count() != numUnits)
			isSolution = false;
	}

	// third - all relations must be valid
	for (int i=0; i<numRels; i++) {
		ValueSet& greater = cells[grels[i]];
		ValueSet& lesser  = cells[lrels[i]];
		// highest in greater Cell must be greater than the lowest in lesser Cell
		if (greater.Highest() <= lesser.Lowest()) {
			// relation is broken
			isSolution = false;
		}
	}

	// check consistency with this board
	for (int i = 0; i < CellCount(); i++) {
		if (GetCell(i).Fixed()) {
			if (GetCell(i).Index() != other.GetCell(i).Index())
				isConsistent = false;
		}
	}

	return isSolution && isConsistent;
}

