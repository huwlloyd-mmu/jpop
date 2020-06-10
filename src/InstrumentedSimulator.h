#pragma once
#include "simulator.h"
#include "solutionstore.h"

class InstrumentedSimulator : public Simulator
{
	// this is a wrapper for another simulator that records all the choices made in a solution store
	Simulator* sim; // the underlying simulator
	SolutionStore* solStore; // the solution store
	unsigned int uid; // uid for this simulator obtained from the solution store 
public:
	InstrumentedSimulator(Simulator* newSim, SolutionStore *solStore ) : solStore(solStore)
	{
		sim = newSim->Copy();
		uid = solStore->GetUID();
	}
	virtual void StartRun(unsigned int startVal)
	{
		solStore->NewSolution(uid);
		sim->StartRun(startVal);
	};
	virtual void GetChoices(std::vector<unsigned int>& choices)
	{
		sim->GetChoices(choices);
		if (choices.size() == 0)
		{
			// no choices left, store the solution
			solStore->EndSolution(uid, sim->GetCost());
		}
	};
	virtual void SetChoice(unsigned int choice)
	{
		solStore->AddChoice(uid, choice);
		sim->SetChoice(choice);
	};
	virtual int GetCost(void)
	{
		return sim->GetCost();
	};
	virtual Simulator* Copy()
	{
		return new InstrumentedSimulator(sim, solStore);
	}
	virtual std::string StateAsString()
	{
		return sim->StateAsString();
	}
	~InstrumentedSimulator() 
	{
		delete sim;
	};
	virtual bool NeedsStartVal()
	{
		return sim->NeedsStartVal();
	};

	virtual void ReadSolution(const std::string& solFile) override
	{
		sim->ReadSolution(solFile);
	}
	virtual bool HasSolution() const override 
	{ 
		return sim->HasSolution(); 
	}
	virtual bool ComponentInSolution(unsigned int component) override 
	{ 
		return sim->ComponentInSolution(component); 
	}
};