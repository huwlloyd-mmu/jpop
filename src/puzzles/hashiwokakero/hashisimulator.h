#pragma once
#include "../../simulator.h"
#include "hashiboard.h"
#include <random>
#include <vector>
#include <set>

using namespace std;

class HashiSimulator : public Simulator
{
	HashiBoard startBoard;
	HashiBoard curBoard;
	mt19937 rng;
	
	int curEdge;
	int numEdgesVisited;
	vector<int> edgeOrder;
	bool hasSolution;
	std::set<unsigned int> solution;
public:
	HashiSimulator(const HashiBoard &instance) { startBoard.Copy(instance); rng = mt19937(random_device{}()); }
	virtual void StartRun(unsigned int startVal ) override;
	virtual void GetChoices(std::vector<unsigned int>& choices) override;
	virtual void SetChoice(unsigned int choice) override;
	virtual int GetCost(void) override;
	virtual Simulator* Copy() override;
	virtual std::string StateAsString() override;
	virtual bool NeedsStartVal() override { return true;  }
	virtual void ReadSolution(const std::string& solFile) override;
	virtual bool HasSolution() const override { return hasSolution; }
	virtual bool ComponentInSolution(unsigned int component) override;
};