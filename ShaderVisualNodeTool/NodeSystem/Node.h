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
	
		//InputnodeT,
		InputnodeConst,
		InputnodeUniform,
		InputnodeGlobal,
		OutputnodeT,
		ShadernodeT,
		FunctionnodeT
};

typedef union Datatype {
	bool b_var;
	float f_var;
	int i_var;
	//std::vector<float> vec2;
	glm::vec2 vec2_var;
	glm::vec3 vec3_var;
	glm::vec4 vec4_var;
	glm::mat4 mat4_var;

	Datatype() { vec2_var = glm::vec2(); vec3_var = glm::vec3(); vec4_var = glm::vec4(); mat4_var = glm::mat4(); };

}Datatype;


class Node;


struct Connection {
	std::shared_ptr<Node> ConnectedNode;
	int ConnectionIndex;
	ValueType VariableType;
	std::string Name;
	//template<typename T> value;
	Datatype Value; // This will be Type or Symbol, should it be in the connection though?
	bool Enabled; //Enabled /disable connection
	
	//there is an issue with this in regards to initialising or reseting the  default value
	// Node class doesn't node the default
	Connection() { ConnectedNode = nullptr; ConnectionIndex = -1; Enabled = true;  }
	//Connection(std::string name , ValueTyPE)
	void ResetConnection() { ConnectedNode = nullptr; ConnectionIndex = -1; Enabled = true; }
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
	

	std::vector<Connection> Input;
	std::vector<Connection> Output; 
	//mnot sure yet if this should be here, i have the information already
	//in the the Input vector
	//std::vector<std::string> slotNames;

	//methods
	virtual void Compile(std::string *ShaderCode) = 0; 
	//irtual void DrawNode(bool active) = 0;

	//output to input
	//maybe this shouldn't be in the parent base class since it might differ 
	void ConnectNode(std::shared_ptr<Node> ConnectedNode, int ConnectedIndex, int OutputIndex);
	
	
	/* std::string GetNodeName();
	 int GetNodeID();
	 NodeType GetNodeType();

	 void SetName(std::string name);
	 void SetID(int ID);*/

private:
	
	
	
};

