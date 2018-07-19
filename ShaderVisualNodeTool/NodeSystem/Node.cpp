#include "Node.h"
#include <iostream>



void Node::ConnectNode(std::shared_ptr<Node> ConnectedNode, int ConnectedIndex, int OutputIndex) {
	
	
	//Try for out of range exception 
	try {

		//connect this node's output index slot to Node ConnectedNode
		//Multiple outputs means, that the list of Outputs simply adds a new 
		//Connection on its list.
		
		//Output.at(OutputIndex).ConnectedNode = ConnectedNode;
		//Output.at(OutputIndex).ConnectionIndex = ConnectedIndex;
		
		/*OutputConnection NewConnection;
		NewConnection.ConnectedNode = ConnectedNode;
		NewConnection.ConnectionIndex = ConnectedIndex;*/

		//pushback in the output list of the slot the pointer and the index
		Output.at(OutputIndex).ConnectedNode.push_back(ConnectedNode);
		Output.at(OutputIndex).ConnectionIndex.push_back(ConnectedIndex);


		//connect ConnectedNode's input index slot with this node's output slot
		
		ConnectedNode->Input.at(ConnectedIndex).ConnectedNode = shared_from_this();
		ConnectedNode->Input.at(ConnectedIndex).ConnectionIndex = OutputIndex;

		//we dont need to assign the value, it is handled in the compile function 
		//ConnectedNode->Input.at(ConnectedIndex).Value = Output.at(OutputIndex).Value;
		//ConnectedNode->Input.at(ConnectedIndex).Name = Output.at(OutputIndex).Name;
	}
	catch (const std::out_of_range& oor) {
		//TODO send to debugger -- or return false
		std::cout << "Index out of range" << std::endl;
		
	}
	

}


