#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Graph.h"
#include "../Utility.h"

//maybe temporary includes

//
// Base class for all nodes.
//

//early declaration of Node for connection struct


//typedef std::shared_ptr<Node> NodePointer;




enum BaseNodeType{
	
		InputnodeT,
		OutputnodeT,
		ShadernodeT,
		FunctionnodeT
};



typedef union Datatype {
	bool b_var;
	float f_var;
	int i_var;
	glm::vec2 vec2_var;
	glm::vec3 vec3_var;
	glm::vec4 vec4_var;
	glm::mat4 mat4_var;

	Datatype() { vec2_var = glm::vec2(); vec3_var = glm::vec3(); vec4_var = glm::vec4(); mat4_var = glm::mat4(); };


}Datatype;


class Node;

//typedef

struct InputConnection {

	std::shared_ptr<Node> ConnectedNode;
	int ConnectionIndex;
	ValueType VariableType;
	std::string Name;
	Datatype Value; 
	bool Enabled; //Enabled /disable connection
	
	//there is an issue with this in regards to initialising or reseting the  default value
	// Node class doesn't node the default
	InputConnection() { ConnectedNode = nullptr; ConnectionIndex = -1; Enabled = true;  }
	//Connection(std::string name , ValueTyPE)
	
	//this clears the connection only from this side, not for the connected node
	void ResetConnection() { ConnectedNode = nullptr; ConnectionIndex = -1; Enabled = true; }
	
};

struct OutputConnection {

	std::vector<std::shared_ptr<Node>> ConnectedNode;
	std::vector<int> ConnectionIndex;
	ValueType VariableType;
	std::string Name;
	Datatype Value;
	bool Enabled; //Enabled /disable connection

	//there is an issue with this in regards to initialising or reseting the  default value
	// Node class doesn't node the default
	OutputConnection() { Enabled = true; }//ConnectedNode = nullptr;  }
	//Connection(std::string name , ValueTyPE)

	//this clears the connection only from this side, not for the connected node
	void ResetConnection(std::shared_ptr<Node> conNode) {

		//search through the list of connections and when you find correct pointer delete that connection
		for (int i = 0; i < ConnectedNode.size(); i++) {
			if (ConnectedNode.at(i) == conNode) {

				//erase the pointer and the index from the two lists
				//TODO : make sure that the two lists have no index mismatches when created
				ConnectedNode.at(i) = nullptr;
				ConnectedNode.erase(ConnectedNode.begin() + i);

				ConnectionIndex.erase(ConnectionIndex.begin() + i);
				break;
			}
		}
	
		Enabled = true; 
	}

};



class Node : public std::enable_shared_from_this<Node>
{

public: 
	

	//Attributes
	std::string Name;
	BaseNodeType Type;
	int UniqueID;
	bool HasCompiled;
	Datatype value;
	int CurrShaderType;
	std::vector<int> AllowedExecShaderTypes;

	std::vector<InputConnection> Input;
	std::vector<OutputConnection> Output; 


	//virtual methods
	virtual void Compile(std::shared_ptr<Node> root) = 0;
	virtual std::string CodeString() = 0;
	
	//output to input connection 
	//Edit: Multiple outputs needs an extra index in the information
	void ConnectNode(std::shared_ptr<Node> ConnectedNode, int ConnectedIndex, int OutputIndex);
	

private:
	
	
	
};

