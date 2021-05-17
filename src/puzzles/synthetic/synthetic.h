#ifndef _SYNTH_INC_
#define _SYNTH_INC_

#include "../../simulator.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iostream>
using namespace std;

class SyntheticSimulator : public Simulator
{
    // synthetic landscape 
    // there are n choices to be made between m alternatives.
    // cost function is n - ncorrect (0 if all correct, n if none correct)
    // probability of success for random solver is m**n

	//
	// with m=2, this is a smooth (k=0) N-k landscape.
	//
    int n;
    int m;
	bool hasSolution;
    mt19937 rng;
    unsigned int *correct;
    unsigned int *choices;
    unsigned int curStep;
public:
    SyntheticSimulator( int n, int m ) : n(n), m(m)
	{
        rng = mt19937(random_device()());
        correct = new unsigned int[n];
        choices = new unsigned int[n];
        std::uniform_int_distribution<unsigned int> dist(0,m-1);
        for ( unsigned int i = 0; i < n; i++ )
        {
            correct[i] = dist(rng);
        }
        curStep = 0;
	}
	~SyntheticSimulator()
    {
        delete [] correct;
        delete [] choices;
    }; 
	virtual void StartRun(unsigned int startVal ) override;
	virtual void GetChoices( std::vector<unsigned int>& choices ) override;
	virtual void SetChoice( unsigned int choice ) override;
	virtual int GetCost( void ) override;
	virtual Simulator* Copy() override;
	virtual std::string StateAsString() override;
	virtual bool NeedsStartVal() override { return false; }
	virtual bool HasSolution() const override { return true; }
	virtual void ReadSolution(const std::string& solFile) override {}
	virtual bool ComponentInSolution(unsigned int component) override;
};

#endif