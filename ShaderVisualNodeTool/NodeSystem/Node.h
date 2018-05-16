#pragma once


#include <string>
#include <vector>
#include "Graph.h"
#include <memory>


//maybe temporary includes

//
// Base class for all nodes.
//

//forward declaration of Node for connection struct
class Node;

//typedef std::shared_ptr<Node> NodePointer;



enum BaseNodeType{
	
	InputNode,
	OutputNode,
	ShaderNode,
	FunctionNode
};



enum Operations {

	Add,
	Mult,
	Power2,
	SquareWeird
};

enum ValueType {
	Float,
	Int,
	Vec2,
	Vec3,
	Vec4,
	Mat4,
	Sampler2D,
	SamplerCube
};


struct Connection {
	std::shared_ptr<Node> ConnectedNode;
	int ConnectionIndex;
	ValueType DataType;
	std::string Name;
	float Value; // This will be Type or Symbol, should it be in the connection though?
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
	
	std::vector<Connection> Input;
	std::vector<Connection> Output; 

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

