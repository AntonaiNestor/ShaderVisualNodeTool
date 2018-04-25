#include "Node.h"
#include <iostream>


void Node::ConnectNode(Node* ConnectedNode, int ConnectedIndex, int OutputIndex) {
	
	
	//Try for out of range exception 
	try {

		//connect this node's output index slot to Node ConnectedNode
		
		Output.at(OutputIndex).ConnectedNode = ConnectedNode;
		Output.at(OutputIndex).ConnectionIndex = ConnectedIndex;
		

		//connect ConnectedNode's input index slot with this node's output slot
		
		ConnectedNode->Input.at(ConnectedIndex).ConnectedNode = this;
		ConnectedNode->Input.at(ConnectedIndex).ConnectionIndex = OutputIndex;

		//we dont need to assign the value, it is handled in the compile function 
		//ConnectedNode->Input.at(ConnectedIndex).Value = Output.at(OutputIndex).Value;
	}
	catch (const std::out_of_range& oor) {
		//TODO send to debugger -- or return false
		std::cout << "Index out of range" << std::endl;
		
	}
	

}




//std::string Node::GetNodeName()
//{
//	return Name;
//}
//
//NodeType Node::GetNodeType()
//{
//	return Type;
//}
//
//void Node::SetName(std::string name)
//{
//	Name = name;
//}
//
//void Node::SetID(int ID)
//{
//	UniqueID = ID;
//}
//
//int Node::GetNodeID() {
//	return UniqueID;
//}