#include "hashiboard.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>

using namespace std;

HashiBoard::HashiBoard(const std::string &fileName)
{
	// this is a monster - do something about it!
	ifstream inFile(fileName);
	if (!inFile.is_open())
		std::cout << "could not open " << fileName << std::endl;
	vector<string> lines;
	string line;
	while (std::getline(inFile, line))
	{
		lines.push_back(line);
	}
	ny = lines.size();
	nNodes = 0;
	int **tempGrid = new int*[ny];
	for (int j = 0; j < ny; j++)
	{
		// tokenize the line
		string token;
		stringstream ss = stringstream(lines[j]);
		vector<string> tokens;
		while (ss >> token)
		{
			tokens.push_back(token);
		}
		nx = tokens.size();
		tempGrid[j] = new int[nx];
		for (int i = 0; i < nx; i++)
		{
			if (tokens[i] == "-")
				tempGrid[j][i] = 0;
			else
			{
				tempGrid[j][i] = std::atoi(tokens[i].c_str());
				++nNodes;
			}
		}
	}
	// now create the nodes, replacing the count with the index in the node list, and empty nodes with -1
	nodes = new Node[nNodes];
	int iNode = 0;
	for (int j = 0; j < ny; j++)
	{
		for (int i = 0; i < nx; i++)
		{
			if (tempGrid[j][i] != 0)
			{
				Node thisNode;
				thisNode.x = i;
				thisNode.y = j;
				thisNode.value = tempGrid[j][i];
				thisNode.degree = 0;
				for (int k = 0; k < 4; k++)
				{
					thisNode.edges[k] = -1;
				}
				tempGrid[j][i] = iNode;
				nodes[iNode++] = thisNode;
			}
			else
				tempGrid[j][i] = -1;
		}
	}
	// now find all the neighbours, connections and edges
	// nodes are already in raster scan order, so need only consider right and down neighbours, to avoid double counting
	vector<Edge> tempEdgeList;

	for (int iNode = 0; iNode < nNodes; iNode++)
	{
		// look for first neighbours, if any, to the right and below
		int iRight = -1;
		int iBelow = -1;
		for (int i = nodes[iNode].x+1; i < nx; i++)
		{
			if (tempGrid[nodes[iNode].y][i] != -1)
			{
				iRight = tempGrid[nodes[iNode].y][i];
				break;
			}
		}
		for (int j = nodes[iNode].y+1; j < ny; j++)
		{
			if (tempGrid[j][nodes[iNode].x] != -1)
			{
				iBelow = tempGrid[j][nodes[iNode].x];
				break;
			}
		}
		if (iRight != -1)
		{
			Edge thisEdge;
			thisEdge.nodes[0] = iNode;
			thisEdge.nodes[1] = iRight;
			thisEdge.capacity = 2;
			if (nodes[iNode].value == 1 || nodes[iRight].value == 1)
				thisEdge.capacity = 1;
			nodes[iNode].edges[2] = nodes[iRight].edges[0] = tempEdgeList.size();
			tempEdgeList.push_back(thisEdge);
		}
		if (iBelow != -1)
		{
			Edge thisEdge;
			thisEdge.nodes[0] = iNode;
			thisEdge.nodes[1] = iBelow;
			thisEdge.capacity = 2;
			if (nodes[iNode].value == 1 || nodes[iBelow].value == 1)
				thisEdge.capacity = 1;
			nodes[iNode].edges[3] = nodes[iBelow].edges[1] = tempEdgeList.size();
			tempEdgeList.push_back(thisEdge);
		}
	}
	nEdges = tempEdgeList.size();
	edges = new Edge[nEdges];
	for (int i = 0; i < nEdges; i++)
	{
		edges[i] = tempEdgeList[i];
	}
	// now find the crossing edges, and we are done....
	for (int i = 0; i < nEdges; i++)
	{
		vector<int> tempCrossingList;
		for (int j = 0; j < nEdges; j++)
		{
			if (i != j)
			{
				int xmin0 = nodes[edges[i].nodes[0]].x < nodes[edges[i].nodes[1]].x ? nodes[edges[i].nodes[0]].x : nodes[edges[i].nodes[1]].x;
				int xmax0 = nodes[edges[i].nodes[0]].x > nodes[edges[i].nodes[1]].x ? nodes[edges[i].nodes[0]].x : nodes[edges[i].nodes[1]].x;
				int xmin1 = nodes[edges[j].nodes[0]].x < nodes[edges[j].nodes[1]].x ? nodes[edges[j].nodes[0]].x : nodes[edges[j].nodes[1]].x;
				int xmax1 = nodes[edges[j].nodes[0]].x > nodes[edges[j].nodes[1]].x ? nodes[edges[j].nodes[0]].x : nodes[edges[j].nodes[1]].x;
				int ymin0 = nodes[edges[i].nodes[0]].y < nodes[edges[i].nodes[1]].y ? nodes[edges[i].nodes[0]].y : nodes[edges[i].nodes[1]].y;
				int ymax0 = nodes[edges[i].nodes[0]].y > nodes[edges[i].nodes[1]].y ? nodes[edges[i].nodes[0]].y : nodes[edges[i].nodes[1]].y;
				int ymin1 = nodes[edges[j].nodes[0]].y < nodes[edges[j].nodes[1]].y ? nodes[edges[j].nodes[0]].y : nodes[edges[j].nodes[1]].y;
				int ymax1 = nodes[edges[j].nodes[0]].y > nodes[edges[j].nodes[1]].y ? nodes[edges[j].nodes[0]].y : nodes[edges[j].nodes[1]].y;

				if (!(xmin0 >= xmax1 || xmax0 <= xmin1 || ymin0 >= ymax1 || ymax0 <= ymin1))
				{
					// edges are crossing
					tempCrossingList.push_back(j);
				}
			}
		}
		edges[i].nCrossingEdges = tempCrossingList.size();
		if (edges[i].nCrossingEdges != 0)
		{
			edges[i].crossingEdges = new int[edges[i].nCrossingEdges];
			for (int j = 0; j < edges[i].nCrossingEdges; j++)
				edges[i].crossingEdges[j] = tempCrossingList[j];
		}
	}
	// clean up the temporary memory
	for (int j = 0; j < ny; j++)
		delete[] tempGrid[j];
	delete[] tempGrid;
}

HashiBoard::~HashiBoard()
{
	
	if (nodes != nullptr)
		delete[] nodes;
	if (edges != nullptr)
		delete[] edges;
}

void HashiBoard::Copy(const HashiBoard& other)
{
	nx = other.nx;
	ny = other.ny;
	nNodes = other.nNodes;
	if (nodes != nullptr)
		delete[] nodes;
	nodes = new Node[nNodes];
	for (int i = 0; i < nNodes; i++)
	{
		nodes[i] = other.nodes[i];
	}
	nEdges = other.nEdges;
	if (edges != nullptr)
		delete[] edges;
	edges = new Edge[nEdges];
	for (int i = 0; i < nEdges; i++)
	{
		edges[i] = other.edges[i];
		// make new memory for crossing edges - copy constructor elision makes this a pain to do in the Edge struct
		if (edges[i].nCrossingEdges != 0)
		{
			edges[i].crossingEdges = new int[edges[i].nCrossingEdges];
			for (int j = 0; j < edges[i].nCrossingEdges; j++)
				edges[i].crossingEdges[j] = other.edges[i].crossingEdges[j];
		}
	}
}

std::string HashiBoard::AsString() const
{
	char **tempGrid;
	tempGrid = new char*[ny];
	for (int j = 0; j < ny; j++)
	{
		tempGrid[j] = new char[nx+1]; // +1 for the null terminator
		std::memset(tempGrid[j], ' ', nx);
		tempGrid[j][nx] = (char)0;
	}
	// draw the numbers
	for (int iNode = 0; iNode < nNodes; iNode++)
	{
		tempGrid[nodes[iNode].y][nodes[iNode].x] = '0' + nodes[iNode].value;
	}
	// draw the edges
	for (int iEdge = 0; iEdge < nEdges; iEdge++)
	{
		int c0, c1;
		if (edges[iEdge].capacity != 0 || edges[iEdge].value == 0)
			continue; // not set, or set to zero

		c0 = edges[iEdge].nodes[0];
		c1 = edges[iEdge].nodes[1];
		if (nodes[c0].y == nodes[c1].y)
		{
			// horizontal edge
			int x0, x1, y;
			y = nodes[c0].y;
			x0 = nodes[c0].x < nodes[c1].x ? nodes[c0].x : nodes[c1].x;
			x1 = nodes[c0].x > nodes[c1].x ? nodes[c0].x : nodes[c1].x;
			for (int j = x0 + 1; j < x1; j++)
			{
				if (edges[iEdge].value == 1)
					tempGrid[y][j] = '-';
				else
					tempGrid[y][j] = '=';
			}
		}
		else
		{
			// vertical edge
			int y0, y1, x;
			x = nodes[c0].x;
			y0 = nodes[c0].y < nodes[c1].y ? nodes[c0].y : nodes[c1].y;
			y1 = nodes[c0].y > nodes[c1].y ? nodes[c0].y : nodes[c1].y;
			for (int j = y0 + 1; j < y1; j++)
			{
				if (edges[iEdge].value == 1)
					tempGrid[j][x] = '|';
				else
					tempGrid[j][x] = '"';
			}
		}

	}
	string retVal = string();
	for (int j = 0; j < ny; j++)
	{
		retVal += string(tempGrid[j]) + "\n";
	}
	for (int j = 0; j < ny; j++)
		delete[] tempGrid[j];
	delete tempGrid;

	return retVal;
}

void HashiBoard::SetEdgeSite(int iEdge, int value)
{
	if (edges[iEdge].capacity == 0)
		return; // can't set a fixed edge

	edges[iEdge].value = value;
	edges[iEdge].capacity = 0;

	// propagate constraints if values is not zero (i.e we are adding an edge) 
	if (value == 0)
		return;
	
	// first remove crossing edges 
	for (int i = 0; i < edges[iEdge].nCrossingEdges; i++)
	{
		SetEdgeSite(edges[iEdge].crossingEdges[i], 0);
	}
	// update the degree on the attached nodes
	for (int j = 0; j < 2; j++)
	{
		nodes[edges[iEdge].nodes[j]].degree += value;
	}
	// now check the ends - first limit capacity on attached edges to remaining slots on the node
	for (int j = 0; j < 2; j++)
	{
		int iNode = edges[iEdge].nodes[j];
		int remaining = nodes[iNode].degree - nodes[iNode].value;
		int totalCapacity = 0;
		if (remaining >= 0)
		{
			// limit the capacity of all unset connected edges to the remaining capacity of the node
			for (int k = 0; k < 4; k++)
			{
				if (nodes[iNode].edges[k] != -1)
				{
					if (remaining == 0)
						SetEdgeSite(nodes[iNode].edges[k], 0);
					else
						edges[nodes[iNode].edges[k]].capacity = std::min(edges[nodes[iNode].edges[k]].capacity, remaining);
					totalCapacity += edges[nodes[iNode].edges[k]].capacity;
				}
			}
			// now check if we can set all the edges
			if (remaining == totalCapacity)
			{
				// set all edges
				for (int k = 0; k < 4; k++)
				{
					if (nodes[iNode].edges[k] != -1)
					{
						SetEdgeSite(nodes[iNode].edges[k], edges[nodes[iNode].edges[k]].capacity);
					}
				}
			}

		}
	}
}

void HashiBoard::StepDFS(int node, int* visited ) const
{
	if (visited[node])
		return;
	visited[node] = 1;
	for (int i = 0; i < 4; i++)
	{
		if (nodes[node].edges[i] != -1)
		{
			Edge* e = edges + nodes[node].edges[i];
			if (e->capacity == 0)
			{
				if (e->value != 0)
				{
					int next;
					if (e->nodes[0] == node)
						next = e->nodes[1];
					else
						next = e->nodes[0];
					StepDFS(next, visited);
				}
			}
		}
	}
}

int HashiBoard::ConnectedComponents() const
{
	int* visited = new int[nNodes];
	std::memset(visited, 0, nNodes * sizeof(int));
	int componentCount = 0;
	bool done = false;
	while (!done)
	{
		// find first unvisited
		int next = 0;
		while (visited[next] && next != nNodes)
			++next;
		if (next == nNodes)
			done = true;
		else
		{
			++componentCount;
			StepDFS(next, visited);
		}
	}
	delete[] visited;
	return componentCount;
}

int HashiBoard::Cost() const
{
	// calculate the cost function for this board
	// this is the sum of the absolute differences between node values and degrees, and the number of coneected components over 1.
	int sumDiff = 0;
	for (int i = 0; i < nNodes; i++)
	{
		int connections = 0;
		for (int k = 0; k < 4; k++)
		{
			if (nodes[i].edges[k] != -1)
			{
				if ( edges[nodes[i].edges[k]].capacity == 0 )
					connections += edges[nodes[i].edges[k]].value;
			}
		}
		sumDiff += std::abs(connections - nodes[i].value);
	}
	int cost = sumDiff + (ConnectedComponents() - 1);
	return cost;
}
