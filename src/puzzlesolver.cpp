#include "solvers/aco/puzzleantsystem.h"
#include "solvers/ga/puzzlegeneticalgorithm.h"
#include "solvers/random/RandomSearch.h"
#include "solvers/backtracking/backtracking.h"
#include "solvers/sa/simulatedannealing.h"
#include "puzzles/nurikabe/nurikabesimulator.h"
#include "puzzles/nurikabe/nkboard.h"
#include "puzzles/sudoku/sudokusimulator.h"
#include "puzzles/sudoku/board.h"
#include "puzzles/futoshiki/futoshikisimulator.h"
#include "puzzles/futoshiki/fboard.h"
#include "puzzles/hashiwokakero/hashiboard.h"
#include "puzzles/hashiwokakero/hashisimulator.h"
#include "puzzles/zpg/zpgboard.h"
#include "puzzles/zpg/zpgsimulator.h"
#include "puzzles/slitherlink/slboard.h"
#include "puzzles/slitherlink/slitherlinksimulator.h"
#include "puzzles/binpacking/binpacking.h"
#include "puzzles/synthetic/synthetic.h"
#include "solutionstore.h"
#include "InstrumentedSimulator.h"
#include "analysis/PrescientSolver.h"
#include "arguments.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
float pher0; // will make the simulator return instance size at some point

Simulator *InitSlitherlink(Arguments &a )
{
	string fileName = a.GetArg(string("file"), string());
	SLBoard board(fileName.c_str());
	pher0 = 0.02f / (board.GetNX()*board.GetNY());

	return (Simulator*)(new SlitherlinkSimulator(board));
}

Simulator *InitZPG(Arguments &a)
{
	string fileName = a.GetArg(string("file"), string());
	ZPGBoard board(fileName.c_str());
	pher0 = 0.02f / (board.getRows()*board.getColumns());

	return (Simulator*)(new ZPGSimulator(board));

}

Simulator *InitNurikabe( Arguments &a )
{
	string fileName = a.GetArg(string("file"), string());
	NurikabeBoard board(fileName.c_str());
	pher0 = 1.0f / board.GetNumCells();

	return (Simulator*)(new NurikabeSimulator(board));
}

Simulator *InitSudoku( Arguments & a)
{
	string puzzleString;
	Board board;
	if (a.GetArg("blank", 0) && a.GetArg("order", 0))
	{
		int order = a.GetArg("order", 0);
		if (order != 0)
			puzzleString = string(order*order*order*order, '.');
	}
	else
	{
		// read in the puzzle from a one-line string
		puzzleString = a.GetArg(string("puzzlestring"), string());
		if (puzzleString.length() == 0)
		{
			// try from a file
			string fileName = a.GetArg(string("file"), string());
			board.Copy(Board(fileName));
		}
		else
		{
			board.Copy(Board(puzzleString, false));
		}
	}
	pher0 = 1.0f / board.CellCount();
	return (Simulator*)(new SudokuSimulator(board));
}

Simulator *InitFutoshiki(Arguments &a)
{
	string puzzleString;
	FBoard board;
	// read in the puzzle from a one-line string
	puzzleString = a.GetArg(string("puzzlestring"), string());
	if (puzzleString.length() == 0)
	{
		// try from a file
		string fileName = a.GetArg(string("file"), string());
		board.Copy(FBoard(fileName));
	}
	else
	{
		board.Copy(FBoard(puzzleString, false));
	}
	pher0 = 1.0f / board.CellCount();
	return (Simulator*)(new FutoshikiSimulator(board));
}

Simulator *InitHashi(Arguments &a)
{
	string fileName = a.GetArg(string("file"), string());
	HashiBoard board(fileName.c_str());
	pher0 = 0.01f / board.GetNumEdgeSites();

	return (Simulator*)(new HashiSimulator(board));

}

Simulator* InitSynthetic( Arguments &a )
{
	int n = a.GetArg(string("synth_n"), 20);
	int m = a.GetArg(string("synth_m"), 2);
	pher0 = 1.0f / (float)n;
	return (Simulator*)(new SyntheticSimulator(n, m));
}

Simulator* InitBinPacking( Arguments &a)
{
	vector<string> names;
	vector<uint32_t> bestKnown;
	vector<uint32_t> binSizes;
	vector<vector<uint32_t>> objectSizes;

	// read the file
	string fileName = a.GetArg(string("file"), string());
	ifstream infile(fileName);
	if (!infile.is_open())
	{
		cerr << "could not open file " << fileName << endl;
	}
	// read all the instances
	uint32_t nInst;
	infile >> nInst;
	for (uint32_t i = 0; i < nInst; i++)
	{
		string name;
		uint32_t numObj, binSize, best;
		infile >> name;
		infile >> binSize >> numObj >> best;
		names.push_back(name);
		binSizes.push_back(binSize);
		bestKnown.push_back(best);
		vector<uint32_t> sizes;
		for (uint32_t j = 0; j < numObj; j++)
		{
			uint32_t s;
			infile >> s;
			sizes.push_back(s);
		}
		objectSizes.push_back(sizes);
	}

	uint32_t instanceIndex = a.GetArg(string("instance_num"), 0);

	BinPackingSimulator* bps = new BinPackingSimulator(objectSizes[instanceIndex], binSizes[instanceIndex], names[instanceIndex], bestKnown[instanceIndex]);
	pher0 = 0.01f / objectSizes[instanceIndex].size();
	return (Simulator*)bps;
}

int main(int argc, char *argv[])
{
	Arguments a(argc, argv);
	Simulator *sim = nullptr;

	string puzzleType = a.GetArg(string("puzzle"), string());
	if (puzzleType == "sudoku")
		sim = InitSudoku(a);
	if (puzzleType == "futoshiki")
		sim = InitFutoshiki(a);
	if (puzzleType == "nurikabe")
		sim = InitNurikabe(a);
	if (puzzleType == "hashi")
		sim = InitHashi(a);
	if (puzzleType == "zpg")
		sim = InitZPG(a);
	if (puzzleType == "slitherlink")
		sim = InitSlitherlink(a);
	if (puzzleType == "binpacking")
		sim = InitBinPacking(a);
	if (puzzleType == "synthetic")
		sim = InitSynthetic(a);

	int timeOutSecs = a.GetArg("timeout", 0);
	int maxEvals = a.GetArg("evaluations", 0);
	bool blank = a.GetArg("blank", false);
	bool verbose = a.GetArg("verbose", false);
	bool showInitial = a.GetArg("showinitial", 0);
	bool success;

	float solTime;
	std::string solution;

	Solver *as;

	string solverType = a.GetArg(string("solver"), string("random") );


	if (solverType == "aco")
	{
		int nAnts = a.GetArg("ants", 10);
		float q0 = a.GetArg("q0", 0.9f);
		float rho = a.GetArg("rho", 0.9f);
		float evap = a.GetArg("evap", 0.005f);

		as = new PuzzleAntSystem(nAnts, q0, rho, pher0, evap);
	}
	else if (solverType == "ga")
	{
		// length is number of ints for integer, number of bits for bitmap
		int numbits = a.GetArg("length", 24);
		int populationSize = a.GetArg("population", 100);
		int tournamentSize = a.GetArg("tournament", 10);
		float crossover = a.GetArg("crossover", 0.98f);
		float mutation = a.GetArg("mutation", 1/(float)numbits);
		std::string ctype = a.GetArg("chromosome", string("int"));
		ChromosomeType ct = CT_BITMAP;
		if (ctype == "int")
			ct = CT_INT;

		as = new PuzzleGeneticAlgorithm(numbits, populationSize, tournamentSize, crossover, mutation, ct);
	}
	else if (solverType == "sa")
	{
		// the temperature parameters are logarithmic - easier for tuning
		// input parameter for maxTemp is log10 of the real value and tempchange is log10(1-tempChange)
		float maxTemp = a.GetArg("maxtemp", 3.0f);
		maxTemp = pow(10.0f, maxTemp);
		float tempChangeDefault;
		// defult temp change corresponds to 8 decades of change over 100000 iterations
		float tempChange = a.GetArg("tempchange", -3.735f);
		tempChange = 1.0f - pow(10.0f, tempChange);
		int length = a.GetArg("length", 24);
		as = new SimulatedAnnealing(maxTemp, tempChange, length);
	}
	else if (solverType == "backtrack")
	{
		as = new BacktrackingSearch();
	}
	else if (solverType == "ps") // prescient solver
	{
		// make a random solver for now
		PrescientSolver *ps = new PrescientSolver();
		// expect there to be a solution
		std::string solFile = a.GetArg("solution", std::string(""));
		sim->ReadSolution(solFile);
		ps->Solve(sim,0.0f,0);
		std::cout << ps->GetNumSteps() << " " << ps->GetLogProb() << std::endl;
		return 0;
	}
	else
	{
		as = new RandomSearch();
	}

	bool instrumented = a.GetArg("instrument", false);
	string instrument_outfile = a.GetArg("instrumentfile", std::string());

	// only instrument stochastic solvers - doesn't make sense for backtracking
	//if (solverType == "bs")
	//	instrumented = false;

	SolutionStore* solStore = nullptr;
	if (instrumented)
	{
		solStore = new SolutionStore();
		sim = new InstrumentedSimulator(sim, solStore);
	}

	if (!verbose)
		as->Mute();

/*	if (showInitial)
	{
		// print inital grid
		cout << "Initial grid" << endl;
		cout << board.AsString() << endl;
	}
*/

	int bestVal = as->Solve(sim, (float)timeOutSecs, maxEvals);
	int numEvals = as->GetNumEvals();
	solution = as->GetSolution();
	solTime = as->GetSolutionTime();
	if (instrumented)
	{
		int numUnique = solStore->GetNumUnique();
		int numBest = solStore->GetNumBest();
		float meanCost = solStore->GetMeanCost();
		float meanIncidence = solStore->GetMeanIncidence();
		std::cout << bestVal << " " << solTime << " " << numEvals << " " <<
			numUnique << " " << numBest << " " << meanCost << " " << meanIncidence << endl;
		if (instrument_outfile != std::string())
			solStore->WriteSolStats(instrument_outfile);
	}
	else
	{
		std::cout << bestVal << " " << solTime << " " << numEvals << endl;
	}
	if (verbose)
	{
		cout << solution << endl;
	}
	/*
	if (!success)
	{
		cout << "failed in time " << solTime << endl;
		cout << solution << endl;
	}
	else
	{
		
		cout << "Solution:" << endl;
		cout << solution << endl;
		cout << "solved in " << solTime << endl;
	}
	*/
}
