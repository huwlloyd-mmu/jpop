#pragma once
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#define SOLUTION_DISTANCE 0

#include "SpookyV2.h" // Spooky hash function, for hashing solutions.
static SpookyHash hasher;

class SolutionStore
{
	// used by instrumented simulators to record all the choices made in the search process
	unsigned int uid;

	uint64_t HashSolution(std::vector<unsigned int> choices)
	{
		return hasher.Hash64(choices.data(), choices.size() * sizeof(unsigned int), 1234);
	}

	struct Solution
	{
		unsigned int cost;
		std::vector<unsigned int> choices;
		int incidence;
		Solution() : incidence(0), cost(0) {}
		Solution(int cost, const std::vector<unsigned int>& ch ) : cost(cost), choices(ch), incidence(1){}
	};

	bool statsValid = false;
	std::map<uint64_t, Solution> uniqueSolutions;
	std::vector<std::vector<unsigned int>> solutions;

	// solution stats
	std::vector<Solution> solArray; // working array used by CalcStats
	// counts of solutions by incidence (number of times repeated) and cost.
	std::vector<std::pair<unsigned int, unsigned int>> incidenceHistogram;
	std::vector<std::pair<unsigned int, unsigned int>> costHistogram;
	std::vector<unsigned int> bestSols;
	std::vector<unsigned int> otherSols;
#if SOLUTION_DISTANCE 
	std::vector<float> bestVsBest;
	std::vector<std::pair<int, float>> bestVsRest;
#endif
public:
	SolutionStore() {}
	unsigned int GetUID() { solutions.push_back(std::vector<unsigned int>());  return solutions.size()-1; }
	int GetNumSolutions() const { return uniqueSolutions.size(); }

	float MeanSolSize() const
	{
		uint64_t total = 0;
		for (auto s : uniqueSolutions)
			total += s.second.choices.size();
		return (float)total / uniqueSolutions.size();
	}
	
	void NewSolution(unsigned int uid)
	{
		// clear the live solution
		solutions[uid].clear();
	}

	void EndSolution(unsigned int uid, unsigned int cost)
	{
		// sort the solution - could have made the same choices in a different order, but it is still the same solution.
		std::sort(solutions[uid].begin(), solutions[uid].end());
		// hash the solution to see if it is new or old
		uint64_t solHash = HashSolution(solutions[uid]);
		if (uniqueSolutions.find(solHash) == uniqueSolutions.end())
		{
			Solution newSol = Solution(cost, solutions[uid]);
			uniqueSolutions[solHash] = newSol;
		}
		else
		{
			// sanity check
			if (uniqueSolutions[solHash].cost != cost)
			{
				std::cout << "solution store - cost mismatch, possible hash collision" << std::endl;
			}
			++uniqueSolutions[solHash].incidence;
		}
	}
	void AddChoice(unsigned int uid, unsigned int choice )
	{	
		// ignore choices with the top bit set - these don't form part of the real solution
		if (choice & 0x80000000)
			return;
		solutions[uid].push_back(choice);
	}

	float JaccardDistance(const std::vector<unsigned int>& vals0, const std::vector<unsigned int>& vals1)
	{
		std::vector<unsigned int> s0 = vals0;
		std::vector<unsigned int> s1 = vals1;
		std::sort(s0.begin(), s0.end());
		std::sort(s1.begin(), s1.end());

		std::vector<unsigned int> s_inter;
		std::vector<unsigned int> s_union;

		std::set_intersection(s0.begin(),s0.end(), s1.begin(), s1.end(), std::back_inserter(s_inter));
		std::set_union(s0.begin(), s0.end(), s1.begin(), s1.end(), std::back_inserter(s_union));

		return 1.0f - (float)s_inter.size() / (float)s_union.size();
	}

	void CalcSolStats()
	{
		statsValid = true;
		unsigned int bestCost = 0xffffffff;
		for (auto v : uniqueSolutions)
		{
			solArray.push_back(v.second);
			if (v.second.cost < bestCost)
				bestCost = v.second.cost;
		}

		for (int i = 0; i < solArray.size(); i++)
		{
			if (solArray[i].cost == bestCost)
				bestSols.push_back(i);
			else
				otherSols.push_back(i);
		}
#if SOLUTION_DISTANCE
		// now work out the distances
		if (bestSols.size() > 1)
		{
			for (int i = 0; i < bestSols.size() - 1; i++)
			{
				for (int j = i + 1; j < bestSols.size(); j++)
				{
					int sol0 = bestSols[i];
					int sol1 = bestSols[j];
					float dist = JaccardDistance(solArray[sol0].choices, solArray[sol1].choices);
					bestVsBest.push_back(dist);
				}
			}
		}
		for (int i = 0; i < bestSols.size(); i++)
		{
			for (int j = 0; j < otherSols.size(); j++)
			{
				int sol0 = bestSols[i];
				int sol1 = otherSols[j];
				float dist = JaccardDistance(solArray[sol0].choices, solArray[sol1].choices);
				int deltaCost = solArray[sol1].cost - solArray[sol0].cost;
				bestVsRest.push_back(std::pair<int, float>(deltaCost, dist));
			}
		}
#endif
		// histogram by incidence count
		std::sort(solArray.begin(), solArray.end(), [](const Solution& t0, const Solution& t1) -> bool {return t0.incidence > t1.incidence; });

		unsigned int cur = solArray[0].incidence;
		unsigned int count = 1;
		for (auto it = solArray.begin() + 1; it != solArray.end(); ++it)
		{
			if (it->incidence == cur)
				++count;
			else
			{
				incidenceHistogram.push_back(std::pair<unsigned int, unsigned int>(cur, count));
				cur = it->incidence;
				count = 1;
			}
		}
		incidenceHistogram.push_back(std::pair<unsigned int, unsigned int>(cur, count));

		// now histogram by cost
		std::sort(solArray.begin(), solArray.end(), [](const Solution& t0, const Solution& t1) -> bool {return t0.cost > t1.cost; });

		cur = solArray[0].cost;
		count = 1;
		for (auto it = solArray.begin() + 1; it != solArray.end(); ++it)
		{
			if (it->cost == cur)
				++count;
			else
			{
				costHistogram.push_back(std::pair<unsigned int, unsigned int>(cur, count));
				cur = it->cost;
				count = 1;
			}
		}
		costHistogram.push_back(std::pair<unsigned int, unsigned int>(cur, count));
	}

	void WriteSolStats( const std::string &fileName )
	{
		if (!statsValid)
			CalcSolStats();
		// output to file
		std::ofstream out_file(fileName);
		if (out_file.is_open())
		{
			// solutions
			out_file << "Unique solutions: " << solArray.size() << " Best: " << bestSols.size() << " Rest: "<< otherSols.size() << std::endl;
			for (auto s : solArray)
			{
				out_file << s.cost << " " << s.incidence << std::endl;
			}
#if SOLUTION_DISTANCE
			out_file << "Best vs best: " << bestVsBest.size() << std::endl;
			for (auto elem : bestVsBest)
			{
				out_file << elem << std::endl;
			}
			out_file << "best vs rest: " << bestVsRest.size() << std::endl;
			for (auto elem : bestVsRest)
			{
				out_file << elem.first << " " << elem.second << std::endl;
			}
#endif
			out_file << "Incidence histogram: " << incidenceHistogram.size() << std::endl;
			for (auto elem : incidenceHistogram)
			{
				out_file << elem.first << " " << elem.second << std::endl;
			}
			out_file << "Cost histogram: " << costHistogram.size() << std::endl;
			for (auto elem : costHistogram)
			{
				out_file << elem.first << " " << elem.second << std::endl;
			}
			out_file.close();
		}
		else
		{
			std::cerr << "could not open file " << fileName << "for writing" << std::endl;
		}
	}

	unsigned int GetNumUnique() 
	{ 
		if (!statsValid)
			CalcSolStats();
		return solArray.size(); 
	}
	unsigned int GetNumBest() 
	{ 
		if (!statsValid)
			CalcSolStats();
		return bestSols.size(); 
	}
	float GetMeanCost()
	{
		if (!statsValid)
			CalcSolStats();

		float totalCost = 0.0f;
		for (auto s : solArray)
			totalCost += s.cost;
		return totalCost / solArray.size();
	}
	float GetMeanIncidence()
	{
		if (!statsValid)
			CalcSolStats();

		float totalIncidence = 0.0f;
		for (auto s : solArray)
			totalIncidence += s.incidence;
		return totalIncidence / solArray.size();
	}
};
