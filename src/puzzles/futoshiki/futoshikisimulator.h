#include "../../simulator.h"
#include "fboard.h"
#include <random>
#include <set>

class FutoshikiSimulator : public Simulator
{
	FBoard startBoard;
	FBoard curBoard;
	int failCells;
	int curCell;
	int cellsVisited;
	
	std::mt19937 randGen;
	bool hasSolution;
	std::set<unsigned int> solution;
public:
	FutoshikiSimulator( const FBoard &board )
	{
		startBoard.Copy(board);
		randGen = std::mt19937(std::random_device{}());
	}
	~FutoshikiSimulator()
	{ 
	};
	
	virtual void StartRun(unsigned int startVal) override;
	virtual void GetChoices( std::vector<unsigned int>& choices ) override;
	virtual void SetChoice( unsigned int choice ) override;
	virtual int GetCost( void ) override;
	virtual Simulator* Copy() override;
	virtual std::string StateAsString() override;
	virtual bool NeedsStartVal() override { return true; }
	virtual void ReadSolution(const std::string& solFile) override;
	virtual bool HasSolution() const override { return hasSolution; }
	virtual bool ComponentInSolution(unsigned int component) override;


};
