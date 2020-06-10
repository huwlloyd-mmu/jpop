#pragma once
#include <string>

class HashiBoard
{
	// describes an instance of Hashiwokakero. Allows moves to be made and implements constraint propogation 

	int nx, ny; // grid size
	typedef struct
	{
		// order convention in the following arrays is left, up, right, down.
		int edges[4];  // indices in the edge list of the four potential connections (-1 for none)
		int x, y; // position in the grid 
		int degree; // current degree of the vertex
		int value; // the target number of connections (the given number in the puzzle) 
	} Node;

	struct Edge
	{
		int nodes[2]; // the indices of the two ends
		int capacity; // how many connections this edge has available (0, 1, 2) - 0 if it is fixed
		int value; // no of edges on this site (once fixed)
		int *crossingEdges; // list of edges which cross this edge
		int nCrossingEdges;
		Edge() : crossingEdges(nullptr), nCrossingEdges(0) {}
		~Edge()
		{
			if (crossingEdges != nullptr)
				delete[] crossingEdges;
		}
	};

	// cell and edge lists
	Node* nodes;
	int nNodes;
	Edge* edges;
	int nEdges;

	void StepDFS(int cell, int* visited) const; // depth-first search
public:
	HashiBoard() : nodes(nullptr), edges(nullptr), nNodes(0), nEdges(0), nx(0), ny(0) {};
	HashiBoard(const std::string& fileName);
	~HashiBoard();
	void Copy(const HashiBoard& other);
	std::string AsString() const;
	int ConnectedComponents() const; // counts the connected components
	int Cost() const;
	int GetNX() const { return nx; }
	int GetNY() const { return ny; }
	int GetNumNodes() const { return nNodes; }
	int GetNodeValue(int iNode) const { return nodes[iNode].value; }
	int GetNumEdgeSites() const { return nEdges; }
	int GetEdgeCapacity(int iEdge) const { return edges[iEdge].capacity; }
	bool GetEdgeFixed(int iEdge) const { return edges[iEdge].capacity == 0; }
	int GetEdgeValue(int iEdge) const { return edges[iEdge].value; }
	void SetEdgeSite(int iEdge, int value);
	void GetEdge(int iEdge, int& x0, int& y0, int& x1, int& y1)
	{
		x0 = nodes[edges[iEdge].nodes[0]].x;
		y0 = nodes[edges[iEdge].nodes[0]].y;
		x1 = nodes[edges[iEdge].nodes[1]].x;
		y1 = nodes[edges[iEdge].nodes[1]].y;
	};
};