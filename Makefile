CC=g++
#CFLAGS=-c -O3 -std=c++0x
CFLAGS=-c -O3 -std=gnu++11

all : puzzlesolver ppectest

# main program
puzzlesolver : puzzlesolver.o board.o sudokusimulator.o fboard.o futoshikisimulator.o nkboard.o nurikabesimulator.o hashiboard.o hashisimulator.o slboard.o slitherlinksimulator.o zpgboard.o zpgsimulator.o Tile.o PerimeterTile.o puzzleant.o puzzleantsystem.o IntChromosome.o BitmapChromosome.o puzzlegeneticalgorithm.o RandomSearch.o backtracking.o simulatedannealing.o prescientsolver.o SpookyV2.o
	$(CC) -o puzzlesolver obj/puzzlesolver.o obj/SpookyV2.o obj/puzzles/nurikabe/*.o obj/puzzles/sudoku/*.o obj/puzzles/futoshiki/*.o obj/puzzles/hashiwokakero/*.o obj/puzzles/zpg/*.o obj/puzzles/slitherlink/*.o obj/solvers/aco/*.o obj/solvers/ga/*.o obj/solvers/backtracking/*.o obj/solvers/random/*.o obj/solvers/sa/*.o obj/analysis/*.o

puzzlesolver.o : src/puzzlesolver.cpp
	$(CC) $(CFLAGS) src/puzzlesolver.cpp -o obj/puzzlesolver.o
SpookyV2.o : src/SpookyV2.cpp # hashing algorithm
	$(CC) $(CFLAGS) src/SpookyV2.cpp -o obj/SpookyV2.o
#
# TESTS
#
ppectest : valuesettest.o fboardtest.o fboard.o
	$(CC) -o ppectest test/puzzles/sudoku/valuesettest.o \
	test/puzzles/futoshiki/fboardtest.o obj/puzzles/futoshiki/fboard.o -pthread -lgtest -lgtest_main

valuesettest.o : test/puzzles/sudoku/valuesettest.cpp
	$(CC) $(CFLAGS) test/puzzles/sudoku/valuesettest.cpp -o test/puzzles/sudoku/valuesettest.o

fboardtest.o : test/puzzles/futoshiki/fboardtest.cpp
	$(CC) $(CFLAGS) test/puzzles/futoshiki/fboardtest.cpp -o test/puzzles/futoshiki/fboardtest.o	
#
# PUZZLES
#

# sudoku simulator
board.o: src/puzzles/sudoku/board.cpp
	$(CC) $(CFLAGS) src/puzzles/sudoku/board.cpp -o obj/puzzles/sudoku/board.o
sudokusimulator.o: src/puzzles/sudoku/sudokusimulator.cpp
	$(CC) $(CFLAGS) src/puzzles/sudoku/sudokusimulator.cpp -o obj/puzzles/sudoku/sudokusimulator.o

# futoshiki simulator
fboard.o: src/puzzles/futoshiki/fboard.cpp
	$(CC) $(CFLAGS) src/puzzles/futoshiki/fboard.cpp -o obj/puzzles/futoshiki/fboard.o
futoshikisimulator.o: src/puzzles/futoshiki/futoshikisimulator.cpp
	$(CC) $(CFLAGS) src/puzzles/futoshiki/futoshikisimulator.cpp -o obj/puzzles/futoshiki/futoshikisimulator.o

# nurikabe simulator
nkboard.o: src/puzzles/nurikabe/nkboard.cpp
	$(CC) $(CFLAGS) src/puzzles/nurikabe/nkboard.cpp -o obj/puzzles/nurikabe/nkboard.o
nurikabesimulator.o: src/puzzles/nurikabe/nurikabesimulator.cpp
	$(CC) $(CFLAGS) src/puzzles/nurikabe/nurikabesimulator.cpp -o obj/puzzles/nurikabe/nurikabesimulator.o

# hashiwokakero simulator
hashiboard.o: src/puzzles/hashiwokakero/hashiboard.cpp
	$(CC) $(CFLAGS) src/puzzles/hashiwokakero/hashiboard.cpp -o obj/puzzles/hashiwokakero/hashiboard.o
hashisimulator.o: src/puzzles/hashiwokakero/hashisimulator.cpp
	$(CC) $(CFLAGS) src/puzzles/hashiwokakero/hashisimulator.cpp -o obj/puzzles/hashiwokakero/hashisimulator.o

# zen puzzle garden simulator
zpgboard.o: src/puzzles/zpg/zpgboard.cpp
	$(CC) $(CFLAGS) src/puzzles/zpg/zpgboard.cpp -o obj/puzzles/zpg/zpgboard.o
zpgsimulator.o: src/puzzles/zpg/zpgsimulator.cpp
	$(CC) $(CFLAGS) src/puzzles/zpg/zpgsimulator.cpp -o obj/puzzles/zpg/zpgsimulator.o
Tile.o : src/puzzles/zpg/Tile.cpp
	$(CC) $(CFLAGS) src/puzzles/zpg/Tile.cpp -o obj/puzzles/zpg/Tile.o
PerimeterTile.o : src/puzzles/zpg/PerimeterTile.cpp
	$(CC) $(CFLAGS) src/puzzles/zpg/PerimeterTile.cpp -o obj/puzzles/zpg/PerimeterTile.o

# slitherlink simulator
slboard.o: src/puzzles/slitherlink/slboard.cpp
	$(CC) $(CFLAGS) src/puzzles/slitherlink/slboard.cpp -o obj/puzzles/slitherlink/slboard.o
slitherlinksimulator.o: src/puzzles/slitherlink/slitherlinksimulator.cpp
	$(CC) $(CFLAGS) src/puzzles/slitherlink/slitherlinksimulator.cpp -o obj/puzzles/slitherlink/slithrlinksimulator.o

#
# SOLVERS
#

# aco
puzzleant.o : src/solvers/aco/puzzleant.cpp
	$(CC) $(CFLAGS) src/solvers/aco/puzzleant.cpp -o obj/solvers/aco/puzzleant.o
puzzleantsystem.o : src/solvers/aco/puzzleantsystem.cpp
	$(CC) $(CFLAGS) src/solvers/aco/puzzleantsystem.cpp -o obj/solvers/aco/puzzleantsystem.o

# ga
IntChromosome.o : src/solvers/ga/IntChromosome.cpp
	$(CC) $(CFLAGS) src/solvers/ga/IntChromosome.cpp -o obj/solvers/ga/IntChromosome.o
BitmapChromosome.o : src/solvers/ga/BitmapChromosome.cpp	
	$(CC) $(CFLAGS) src/solvers/ga/BitmapChromosome.cpp -o obj/solvers/ga/BitmapChromosome.o
puzzlegeneticalgorithm.o : src/solvers/ga/puzzlegeneticalgorithm.cpp
	$(CC) $(CFLAGS) src/solvers/ga/puzzlegeneticalgorithm.cpp -o obj/solvers/ga/puzzlegeneticalgorithm.o

# random search
RandomSearch.o : src/solvers/random/RandomSearch.cpp
	$(CC) $(CFLAGS) src/solvers/random/RandomSearch.cpp -o obj/solvers/random/RandomSearch.o

# backtracking search
backtracking.o : src/solvers/backtracking/backtracking.cpp
	$(CC) $(CFLAGS) src/solvers/backtracking/backtracking.cpp -o obj/solvers/backtracking/backtracking.o

# simulated annealing
simulatedannealing.o : src/solvers/sa/simulatedannealing.cpp
	$(CC) $(CFLAGS) src/solvers/sa/simulatedannealing.cpp -o obj/solvers/sa/simulatedannealing.o

# 'prescient' solver
prescientsolver.o : src/analysis/PrescientSolver.cpp
	$(CC) $(CFLAGS) src/analysis/PrescientSolver.cpp -o obj/analysis/prescientsolver.o

clean :
	rm obj/puzzles/sudoku/*.o
	rm obj/puzzles/futoshiki/*.o
	rm obj/puzzles/nurikabe/*.o
	rm obj/puzzles/hashiwokakero/*.o
	rm obj/solvers/aco/*.o
	rm obj/solvers/ga/*.o
	rm obj/solvers/sa/*.o
	rm obj/analysis/*.o
	rm obj/*.o
