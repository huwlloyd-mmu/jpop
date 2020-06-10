#include "../../simulator.h"
#include "slboard.h"
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
using namespace Slitherlink;
using namespace std;
//#define DIRECTED_GRAPH
#include <set>
class SlitherlinkSimulator : public Simulator
{
	SLBoard board;	
	mt19937 randGen;
	Point curPoint;
	unsigned int HashEdge(const Point& p0, const Point& p1) const;
	unsigned int HashStartVert(const Point& p) const;
	Point NextFromHash( unsigned int hash ) const;
	Point StartNodeFromHash(unsigned int hash) const;
	bool hasSolution;
	std::set<unsigned int> solution;
public:
    SlitherlinkSimulator( const SLBoard &board ) : board(board)
	{
		randGen = std::mt19937(std::random_device{}());
	}
	~SlitherlinkSimulator(){}; 
	
	virtual void StartRun(unsigned int startVal ) override;
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
