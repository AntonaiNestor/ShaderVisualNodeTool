#pragma once

#include <map>
#include <vector>
#include <string>
#include "Node.h"


class Node;

class Graph
{
public:
	
	//----------- singleton stuff-------
	static Graph* getInstance();
	Graph(Graph const&);              // Don't Implement
	void operator=(Graph const&); // Don't implement
	// ---------------------------------


	// -- Variables

	//adjacency list for nodes
	std::vector<std::vector<int>> AdjacencyList;

	//Root of the graph
	Node* root;

	//Global map for variable name convertion
	std::map <std::string, std::string> SlotToVariableMap;

	//this is temporarily here for testing
	std::vector<Node*> constantNodes;

	// ---- Methods 

	//this probably will be part of a GUI Manager
	void DrawConstantNodes();
	void AddConstant(Node* node);

	//Create connection between nodes. Either: 
	//  Output slot -> Input Slot
	//  Input slot -> Output Slot
	bool CreateConnectionOutIn(Node* from, Node* to, int FromIndex, int ToIndex);
	bool CreateConnectionInOut(Node* from, Node* to, int FromIndex, int ToIndex);

	//Assigns unique ID to newly created nodes
	int AssignID();

	//Depth first backwards traversal and compilation of the nodes
	void CompileGraph(Node* CurrentNode , std::string* ShaderCode);

	//TODO 
	// Implement function that checks if the graph contains any circles 
	bool CircleInGraph();

private :

	static Graph* Instance;
	int ID;
	Graph();
};

