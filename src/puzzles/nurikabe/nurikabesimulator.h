#include "../../simulator.h"
#include "nkboard.h"
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace Nurikabe;
using namespace std;

class NurikabeSimulator : public Simulator
{
	NurikabeBoard startBoard;
	NurikabeBoard curBoard;
	
	mt19937 randGen;
	int curIslandIndex;
	int curIslandId;
	vector<int> islandOrder;
	set<int> curIsland;
	bool hasSolution = false;
	set<unsigned int> solution;
public:
	NurikabeSimulator( const NurikabeBoard &board )
	{
		startBoard.Copy(board);
		randGen = std::mt19937(std::random_device{}());
		for ( int i = 0; i < board.GetNumIslands(); i++ )
		{
			islandOrder.push_back(i);
		}
	}
	~NurikabeSimulator(){}; 
	
	virtual void StartRun(unsigned int startVal ) override;
	virtual void GetChoices( std::vector<unsigned int>& choices ) override;
	virtual void SetChoice( unsigned int choice ) override;
	virtual int GetCost( void ) override;
	virtual Simulator* Copy() override;
	virtual std::string StateAsString() override;
	virtual bool NeedsStartVal() override { return true; }
	virtual void ReadSolution(const std::string& solFile) override;
	virtual bool HasSolution() const override;
	virtual bool ComponentInSolution(unsigned int component) override;
};
	