#include <vector>
#include <string>
#pragma once

class Simulator
{
	// abstract interface to a simulator
public:
	virtual void StartRun(unsigned int startVal) = 0;
	virtual void GetChoices( std::vector<unsigned int>& choices ) = 0;
	virtual void SetChoice( unsigned int choice ) = 0;
	virtual int GetCost( void ) = 0;
	virtual Simulator* Copy() = 0;
	virtual std::string StateAsString() = 0;
	virtual ~Simulator() {};
	virtual bool NeedsStartVal() = 0;
	// functions required by prescient solver
	virtual void ReadSolution(const std::string& solFile) = 0;
	virtual bool HasSolution() const = 0;
	virtual bool ComponentInSolution(unsigned int component) = 0;
};
