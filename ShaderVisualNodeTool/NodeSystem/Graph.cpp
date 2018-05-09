#include "Graph.h"



Graph::Graph()
{
	ID = 0;
}


//null since it will be created on demand
Graph* Graph::Instance = 0;

Graph * Graph::getInstance()
{
	if (Instance == 0)
	{
		Instance = new Graph();
	}

	return Instance;
}



bool Graph::CreateConnectionOutIn(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex)
{
	int i = from->UniqueID;
	int j = to->UniqueID;

	if (i == j) {
		//Add debug message " node cannot be connected to itself"
		return false;
	}

	//if both nodes have valid IDs
	if (i < AdjacencyList.size() && j < AdjacencyList.size()) {

		auto x = find(AdjacencyList.at(i).begin(), AdjacencyList.at(i).end(), j);
		//if i and j are not already connected
		if (AdjacencyList.at(i).empty()||
				x!= AdjacencyList.at(i).end() ){

				//create connection i - > j ,  directed graph so only one way
				AdjacencyList.at(i).push_back(j);

				// in case i want to have the connections two ways
				//AdjacencyList.at(j).push_back(i);
				
				
				//We need to notify Node from that it is now connecteed to j as well.
				from->ConnectNode(to, ToIndex, FromIndex );

				return true;		
			 }
		}

	//connection was not created
	return false;
	
}

bool Graph::CreateConnectionInOut(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex)
{
	int i = from->UniqueID; 
	int j = to->UniqueID; 

	if (i == j) {
		//Add debug message " node cannot be connected to itself"
		return false;
	}

	//if both nodes have valid IDs
	if (i < AdjacencyList.size() && j < AdjacencyList.size()) {
		//search for the connection in an opposite direction
		auto x = find(AdjacencyList.at(j).begin(), AdjacencyList.at(j).end(), i);
		//if i and j are not already connected
		if (AdjacencyList.at(j).empty() ||
			x != AdjacencyList.at(j).end()) {

			//create connection to - > from since we are connecting an input to an 
			//output
			AdjacencyList.at(j).push_back(i);

			// in case i want to have the connections two ways
			//AdjacencyList.at(i).push_back(j);

			
			//We need to notify Node To that it is now connecteed to From as well.
			to->ConnectNode(from, FromIndex, ToIndex);

			return true;
		}
	}

	//connection was not created
	return false;
}

bool Graph::RemoveConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex)
{
	


	return false;
}

int Graph::AssignID()
{
	//A new node has been assigned. Add it in the list
	//NodeList.push_back(CreatedNode);

	//Expand the adjacency list size , a new node is available for connections
	AdjacencyList.push_back(std::vector<int>());


	//return new unique ID
	return ID++;
}

void Graph::AddNode(std::shared_ptr<Node> node) {
	NodeList.push_back(node);
}

void Graph::CompileGraph(std::shared_ptr<Node> CurrentNode , std::string* ShaderCode)
{
	//Traverse all the Inputs list of this node
	for (std::vector<Connection>::iterator it = CurrentNode->Input.begin(); it != CurrentNode->Input.end(); ++it) {

		//if the connected node's inputs are empty 
		//it means that it is an input node only
		
		if (it->ConnectedNode->Input.empty() && !it->ConnectedNode->HasCompiled) {
			//if that node hasn't been compiled, compile it and continue to this node's next input
			it->ConnectedNode->Compile(ShaderCode);
			
			continue;
		}
		// if it isn't empty and the node hasn't compiled  move to that node and call this function again
		else if (!it->ConnectedNode->HasCompiled) {
			CompileGraph(it->ConnectedNode, ShaderCode);
		}
	}

	CurrentNode->Compile(ShaderCode);
}

