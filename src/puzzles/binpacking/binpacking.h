#pragma once
#ifndef _BINPACKING_INC_
#define _BINPACKING_INC_

#include "../../simulator.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iostream>
using namespace std;


class BinPackingSimulator : public Simulator
{

	// one-dimensional bin-packing problem
	// treated as a combinatorial optimization problem, bins are filled in the order given, then closed off as soon as a new bin started
	vector<uint32_t> sizes;
	uint32_t binSize;
	vector<uint32_t> solution;
	uint32_t curBin;
	uint32_t nBins;
	uint32_t* tabu; // list of object used
	uint32_t firstFitSol = 0; 
	uint32_t lowerLimit = 0;
	uint32_t bestKnown = 0;
	string name;

	void CalcSolutions()  
	{
		// first fit heuristic
		vector<uint32_t> bins;
		bins.push_back(binSize);
		uint32_t totalSize = 0;
		for (auto s : sizes)
		{
			totalSize += s;
			uint32_t i = 0;
			while (i < bins.size() && bins[i] < s)
				++i;
			if (i == bins.size())
			{
				// need to start a new bin
				bins.push_back(binSize-s);
			}
			else
				bins[i] -= s;
		}
		firstFitSol = bins.size();

		lowerLimit = totalSize / binSize;
		if (totalSize % binSize != 0)
			lowerLimit++;
	}
public:
	BinPackingSimulator(const vector<uint32_t> sizes, uint32_t binSize, const string& name, uint32_t bestKnownSol ) : sizes(sizes), binSize(binSize), name(name), bestKnown(bestKnownSol)
	{
		tabu = new uint32_t[sizes.size()];
		CalcSolutions();
	}
	BinPackingSimulator() {}
	~BinPackingSimulator()
	{
		delete[] tabu;
	};

	virtual void StartRun(unsigned int startVal) override;
	virtual void GetChoices(std::vector<unsigned int>& choices) override;
	virtual void SetChoice(unsigned int choice) override;
	virtual int GetCost(void) override;
	virtual Simulator* Copy() override;
	virtual std::string StateAsString() override;
	virtual bool NeedsStartVal() override { return false; }
	virtual bool HasSolution() const override { return true; }
	virtual void ReadSolution(const std::string& solFile) override {}
	virtual bool ComponentInSolution(unsigned int component) override { return false; } // for now, we won't be using the prescient solver here
	uint32_t GetFirstFit() const { return firstFitSol; }
	uint32_t GetLowerLimit() const { return lowerLimit; }
	uint32_t GetNumObjects() const { return sizes.size(); }
};

#endif