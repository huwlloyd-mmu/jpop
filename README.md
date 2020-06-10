# J-POP

## Japanese Puzzles as Optimization Problems

This code implements a number of nature-inspired algorithms to solve Japanese Pencil Puzzles. Puzzles are represented by a simulator which iteratively constructs a solution for a puzzle instance. The simulator repeatedly presents set of choices to a puzzle-agnostic solver until the solution is complete, and calculates a cost value for the solution.

### Simulators:
Sudoku, Futoshiki, Nurikabe, Slitherlink, Hashiwokakero, Zen Puzzle Garden
### Solvers:
ACO, GA, Simulated Annealing, Random search, Backtracking search

## Build
### Linux (g++)
make puzzlesolver

### Windows (Visual Studio)
Project files in msvc folder

### Data 

The instances used in the published experiments can be found in the __puzzledata__ folder. Instances for the puzzles are in the subfolders. The top level folder contains two spreadsheets - test_instances.csv and train_instances.csv which list the instances used for parameter tuning (train) and the main experiments (test). The log_ss_size column gives log10 of the size of the search space, measured by the 'prescient' solver (see paper for details).

## Run

#### Command line qualifiers:

__--puzzle__ (sudoku, futoshiki, nurikabe, hashi, zpg) puzzle type

__--file__ filename - file containing puzzle instance

__--solver__ (aco, ga, backtrack, random) solver type - default random

__--timeout__ seconds - timeout in seconds, default 0 (no timeout)

__--evaluations__ maxeval - maximum number of puzzle evaluations, default 0 (unlimited)

#### GA options

__--length__ n - chromosome length (ints or bits)

__--chromosome__ (int, bitmap) - chromosome representation

__--mutation__ value - mutation rate, default 1/length

__--crossover__ value - crossover fraction (0 to 1, default 0.98)

__--population__ n - population size, default 100

__--tournament__ n - tournament size, default 10

#### ACO options

__--ants__ n - number of ants (default 10)

__--q0__ value - ACS q0 parameter (default 0.9)

__--rho__ value - ACS rho parameter (default 0.9)

__--evap__ value - best-value evaporation constant (default 0.005)

#### SA Options

__--maxtemp__ value - initial temperature (default 10000)

__--tempchange__ value - change in temperature per iteration (default 0.98)