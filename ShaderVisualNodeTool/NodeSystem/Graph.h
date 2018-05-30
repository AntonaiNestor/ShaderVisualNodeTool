#pragma once

#include <map>
#include "../Rendering/Shader.h"
#include "Node.h"
#include "../Utility.h"


class Node;

struct SlotInformation {

	bool SlotType; // 0 -> Input - 1-> Output
	std::string Name; // Name of the slot variable 
	ValueType VarType;
	//Add custom default value here somehow


};

struct FunctionNodeType {

	std::string FilterNodeType; // under which type should this be placed
	std::string Name; // Name of the node 
	std::vector<SlotInformation> Slots; // slot information
	std::string Code; // GLSL code
};

class Graph
{
public:
	
	//----------- singleton stuff-------
	static Graph* getInstance();
	Graph(Graph const&);              // Don't Implement
	void operator=(Graph const&); // Don't implement
	// ---------------------------------


	// -- Variables



	//default  variable values
	float DefaultFloat = 1.0f;
	int DefaultInt = 1;
	glm::vec2 DefaultVec2 = glm::vec2(1.0f, 1.0f);
	glm::vec3 DefaultVec3 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 DefaultVec4 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 DefaultMat4 = glm::mat4();

	int NameCounter;
	
	// The node type information for function ndoes, saved once.
	// The information from these will be used on creation
	//Same idea can be extended to everything
	std::map<std::string,FunctionNodeType> FunctionNodes;
	//adjacency list for nodes
	std::vector<std::vector<int>> AdjacencyList;
	
	//Root of the graph
	std::shared_ptr<Node> root;
	//these two here are probably not correct
	std::string* ShaderCode = new std::string("");
	Shader* daShader;

	//Global map for variable name convertion
	std::map <std::string, std::string> SlotToVariableMap;
	std::map <std::string, std::string> VarToSlotMap;

	//List of all the nodes in the graph - order is not important, just access to all of them
	std::vector<std::shared_ptr<Node>> NodeList;

	// ---- Methods 

	//this probably will be part of a GUI Manager
	//void DrawConstantNodes();
	void AddNode(std::shared_ptr<Node> node);

	void ReadNodeTypes(std::string FilePath);

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
	void ChangeShader(Shader* shader);
	void PrintConnections();
	void ResetGraph();
	void UpdateGraph();
	
	std::string ReplaceVarNames(std::string code, std::string oldName, std::string newName);

	//TODO 
	// Implement function that checks if the graph contains any circles 
	bool CircleInGraph();

private :

	static Graph* Instance;
	int ID;
	Graph();
};

