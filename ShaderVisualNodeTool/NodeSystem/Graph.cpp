#include "Graph.h"
#include <iostream>


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

	//
	

	//if both nodes have valid IDs
	if (i < AdjacencyList.size() && j < AdjacencyList.size()) {


		//create the connection between slots
		
		//if the input slot that we are  connecting to was already connected with something
		//delete that connection correctly
		if (to->Input.at(ToIndex).ConnectedNode != nullptr) {
			RemoveConnection(to->Input.at(ToIndex).ConnectedNode, to, to->Input.at(ToIndex).ConnectionIndex, ToIndex);
		}

		//same goes for this output slot as well. We need to update whatever the node this slot was connected to if there 
		//was one. 

		//TODO THIS SHOULD BE REMOVED WHEN I MAKE INPUTS TO BE PASSED TO MORE OUTPUTS
		if (from->Output.at(FromIndex).ConnectedNode != nullptr) {
			RemoveConnection(from,from->Output.at(FromIndex).ConnectedNode,FromIndex , from->Output.at(FromIndex).ConnectionIndex);
		}

		from->ConnectNode(to, ToIndex, FromIndex);


		auto x = find(AdjacencyList.at(i).begin(), AdjacencyList.at(i).end(), j);
		//if i and j are not already connected
		if (AdjacencyList.at(i).empty()||
				x== AdjacencyList.at(i).end() ){

				//create connection i - > j ,  directed graph so only one way
				AdjacencyList.at(i).push_back(j);

				// in case i want to have the connections two ways
				//AdjacencyList.at(j).push_back(i);
				
				
				//We need to notify Node from that it is now connecteed to j as well.
				

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

	//no need to remove the connection from the output slot of TO this time
	//I will do it for now because i think i do not allow connection between multiple slots 
	//at the moment

	//TODO REMOVE THIS FIRST ONE IF YOU WANT TO HAVE MULTIPLE OUTPUTS
	if (to->Output.at(ToIndex).ConnectedNode != nullptr) {
		RemoveConnection(to,to->Output.at(ToIndex).ConnectedNode, ToIndex, to->Output.at(ToIndex).ConnectionIndex);
	}

	if (from->Input.at(FromIndex).ConnectedNode != nullptr) {
		RemoveConnection(from->Input.at(FromIndex).ConnectedNode,from, from->Input.at(FromIndex).ConnectionIndex,FromIndex);
	}

	to->ConnectNode(from, FromIndex, ToIndex);


	//if both nodes have valid IDs
	if (i < AdjacencyList.size() && j < AdjacencyList.size()) {
		//search for the connection in an opposite direction
		auto x = find(AdjacencyList.at(j).begin(), AdjacencyList.at(j).end(), i);
		//if i and j are not already connected
		if (AdjacencyList.at(j).empty() ||
			x == AdjacencyList.at(j).end()) {

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

//returns false if by removing the connection we also detach 2 nodes.
//returns true if even when removing a specific connection the nodes are still attached
bool Graph::RemoveConnection(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int FromIndex, int ToIndex)
{

	from->Output.at(FromIndex).ResetConnection();
	to->Input.at(ToIndex).ResetConnection();

	//here we need to check (as in the replace function ) if by removing this connection we 
	// lose a connection between nodes for the adjacency list.

	//to do that check after the rest if there still and output that points to that node.
	//If there isn't, find and delete the node from the list.
	bool StillConnected = false;
	for (auto it : from->Output) {
		if (it.ConnectedNode == to) {
			StillConnected = true;
			break;
		}
	}
	if (!StillConnected) {

		int toID = to->UniqueID;
		int fromID = from->UniqueID;

		for (std::vector<int>::iterator it = AdjacencyList.at(toID).begin(); it != AdjacencyList.at(toID).end(); ++it)
		{
			if (*it == fromID)
			{
				AdjacencyList.at(toID).erase(it);
				break;

			}
		}
		return false;
	}
	return true;
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

void Graph::PrintConnections()
{
	for (auto node : NodeList) {
		std::cout << " Node : " << node->UniqueID << " is connected with :" << std::endl;
		int counter = 0;
		for (auto in : node->Output) {

			if (in.ConnectedNode != nullptr){
				std::cout << "Node :" << in.ConnectedNode->UniqueID << " From slot -> "<< counter++<< " To slot -> " << in.ConnectionIndex << std::endl;
			}
		}
		std::cout << "------------------------------------------------------" << std::endl;
	}

}

