#pragma once
#include "../../simulator.h"
#include "zpgboard.h"
#include "Position.h"
#include <random>
#include <vector>

using namespace std;

class ZPGSimulator : public Simulator
{
	ZPGBoard startBoard;
	ZPGBoard curBoard;
	mt19937 rng;

	int totalMoves;
	Position monkPosition;
	void MakeMove(Position selectedMove);
	void perimeterMove(std::shared_ptr<Tile> pTile, Position move);
	void inBoardMove(std::shared_ptr<Tile> pTile, Position move);

public:
	virtual ~ZPGSimulator() {};
	ZPGSimulator(const ZPGBoard& instance);
	virtual void StartRun(unsigned int startVal) override;
	virtual void GetChoices(std::vector<unsigned int>& choices) override;
	virtual void SetChoice(unsigned int choice) override;
	virtual int GetCost(void) override;
	virtual Simulator* Copy() override;
	virtual std::string StateAsString() override;
	virtual bool NeedsStartVal() override;
	virtual void ReadSolution(const std::string& solFile) override;
	virtual bool HasSolution() const override;
	virtual bool ComponentInSolution(unsigned int component) override;
};
