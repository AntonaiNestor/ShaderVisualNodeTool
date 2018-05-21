#pragma once

#include <map>
#include <vector>
#include <string>
#include "Node.h"
#include <memory>





class Node;

//typedef std::shared_ptr<Node> NodePointer;

class Graph
{
public:
	
	//----------- singleton stuff-------
	static Graph* getInstance();
	Graph(Graph const&);              // Don't Implement
	void operator=(Graph const&); // Don't implement
	// ---------------------------------


	// -- Variables

	int NameCounter;

	//adjacency list for nodes
	std::vector<std::vector<int>> AdjacencyList;
	
	//Root of the graph
	std::shared_ptr<Node> root;
	std::string* ShaderCode = new std::string("");

	//Global map for variable name convertion
	std::map <std::string, std::string> SlotToVariableMap;
	std::map <std::string, std::string> VarToSlotMap;

	//List of all the nodes in the graph - order is not important, just access to all of them
	std::vector<std::shared_ptr<Node>> NodeList;

	// ---- Methods 

	//this probably will be part of a GUI Manager
	//void DrawConstantNodes();
	void AddNode(std::shared_ptr<Node> node);

	//Create connection between nodes. Either: 
	//  Output slot -> Input Slot
	//  Input slot -> Output Slot
	bool CreateConnectionOutIn(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex);
	bool CreateConnectionInOut(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex);

	bool RemoveConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex);

	//Assigns unique ID to newly created nodes
	int AssignID();
	std::string GiveName();

	void ResetNameCounter();

	//Depth first backwards traversal and compilation of the nodes
	void CompileGraph(std::shared_ptr<Node> CurrentNode , std::string* ShaderCode);

	void PrintConnections();
	void ResetGraph();
	
	std::string ReplaceVarNames(std::string code, std::string oldName, std::string newName);

	//TODO 
	// Implement function that checks if the graph contains any circles 
	bool CircleInGraph();

private :

	static Graph* Instance;
	int ID;
	Graph();
};

