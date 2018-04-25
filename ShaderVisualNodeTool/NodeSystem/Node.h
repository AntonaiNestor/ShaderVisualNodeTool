#pragma once


#include <string>
#include <vector>
#include "Graph.h"


//maybe temporary includes

//
// Base class for all nodes.
//

//forward declaration of Node for connection struct
class Node;


enum NodeType{
	
	InputNode,
	OutputNode,
	ShaderNode,
	FunctionNode
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
	Node* ConnectedNode;
	int ConnectionIndex;
	float Value; // This will be Type or Symbol, should it be in the connection though?
	bool Enabled; //Enabled /disable connection
};

class Node
{

public: 
	

	//Attributes
	std::string Name;
	NodeType Type;
	int UniqueID;
	bool HasCompiled;
	std::vector<Connection> Input;
	std::vector<Connection> Output; 

	//methods
	virtual void Compile(std::string *ShaderCode) = 0; 
	virtual void DrawNode(bool active) = 0;

	//output to input
	//maybe this shouldn't be in the parent base class since it might differ 
	void ConnectNode(Node* ConnectedNode, int ConnectedIndex, int OutputIndex);
	
	
	/* std::string GetNodeName();
	 int GetNodeID();
	 NodeType GetNodeType();

	 void SetName(std::string name);
	 void SetID(int ID);*/

private:
	
	
	
};

