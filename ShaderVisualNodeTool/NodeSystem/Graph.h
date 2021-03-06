#pragma once

#include <map>
#include "../Rendering/Shader.h"
#include "Node.h"
//#include "InputNode.h"
#include "../Utility.h"


class Node;


struct SlotInformation {

	bool SlotType; // 0 -> Input - 1-> Output
	std::string Name; // Name of the slot variable 
	ValueType VarType;
	//Add custom default value here somehow


};

struct InputNodeInformation {

	std::string Name;
	std::vector<std::string> SlotNames;
	std::vector<ValueType> VarTypes;
	int InitInputType;
	int ShaderType;
};

struct FunctionNodeInformation {

	std::string FilterNodeType; // under which type should this be placed
	std::string Name; // Name of the node 
	std::vector<int> AllowedExecShaders; // The shader in which the code is can be run in
	std::vector<SlotInformation> Slots; // slot information
	std::string Code; // GLSL code
};

struct ShaderNodeInformation {

	std::string Name; // name which will tell the shadertype
	ShaderType ShadeType; // the shaderType according to the name
	std::vector<SlotInformation> Slots; //slot information  
	std::string DefaultCode[5]; // the default shader code, retrieved from the program 
	//in the shader though, not from the json file (for now)
	
};

enum ShaderSection {

	VersionSeg,
	VaryingSeg,
	UniformSeg,
	ConstantSeg,
	MainSeg, // the main segment which will run once
	MainLoopSeg //the per vertex main body of geometry shader for the loop
	//I need an array of these segments if I am to not to have a loop in the geom
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

	//default  variable values for initialization
	const char* VariableTypes[5] = { "Constant", "Uniform", "Global","Attribute","TransformationMatrix"};
	const char* ShaderTypes[5] = {"Vertex","Tess_Control","Tess_Eval", "Geometry","Fragment"};
	bool DefaultBool = false;
	float DefaultFloat = 1.0f;
	int DefaultInt = 1;
	glm::vec2 DefaultVec2 = glm::vec2(1.0f, 1.0f);
	glm::vec3 DefaultVec3 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 DefaultVec4 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 DefaultMat4 = glm::mat4();

	//unique ID for names
	int NameCounter;

	//application time
	float time;

	//Texture Counter
	unsigned int TextureCounter;
	
	//Resource filepath
	//TODO MAKE THIS RELEVANT PATH
	std::string ResourcesPath = "D:/Graphics Libraries/Resources/";

	//Input Node Types
	std::map <std::string, InputNodeInformation> InputNodes;

	// The node type information for function ndoes, saved once.
	// The information from these will be used on creation
	//Same idea can be extended to everything
	std::map<std::string,FunctionNodeInformation> FunctionNodes;

	//Subcategory maps , for convenience

	std::vector<std::vector<std::string>> FunctionSubCategories ;
	
	std::map<std::string, ShaderNodeInformation> ShaderNodes;

	//adjacency list for nodes
	//Does not save double info, only which nodes are connected to
	std::vector<std::vector<int>> AdjacencyList;
	
	//Root of the graph
	std::shared_ptr<Node> root;
	//these two here are probably not correct
	//std::string* ShaderCode = new std::string("");
	
	std::vector<std::string> Identifiers = {"$Version$","$Varyings$","$Uniforms$","$Constants$" ,"$Main$","$MainLoop$" };
	std::vector<std::string> FunctionNodeTypes = {"Mathematical","Logic","Type Conversion","Geometric Effects","Lighting Effects","Texture Manipulation","Utility"};

	Shader* daShader;
	//Very nice that this is independent from the shadercode
	int GeomTopologyOut = GL_TRIANGLE_STRIP;
	int GeomMaxVerticesOut = 256;
	int DrawingTopology = GL_PATCHES; //DEFAULT TO PATCHES IF THERE IS TESSELATION
	int VertexOutTopology = GL_TRIANGLES;
	int PatchSize = 3;
	//TCS info
	int TCSVertNumberOut = 3;
	
	//TES layout info
	const char* GeneratedPrimitiveMode[3] = { "isolines", "triangles", "quads" };
	const char* GeneratedSpacing[3] = { "equal_spacing", "fractional_even_spacing", "fractional_odd_spacing" };
	const char* VertexOrder[2] = { "cw", "ccw" };

	const char*  TESGenPrimMode = GeneratedPrimitiveMode[1];
	const char* TESGenSpacing = GeneratedSpacing[0];
	const char*	 TESVertexOrder = VertexOrder[1];
	/*int TESGenPrimMode = GL_TRIANGLES;
	int TESGenSpacing = GL_EQUAL;
	int TESVertexOrder = GL_CCW;*/
	bool TESPointMode = false;
	

	//Global map for variable name convertion
	std::map <std::string, std::string> SlotToVariableMap;
	std::map <std::string, std::string> VarToSlotMap;

	//List of all the nodes in the graph - order is not important, just access to all of them
	std::vector<std::shared_ptr<Node>> NodeList;
	std::vector<std::shared_ptr<Node>> UniformNodes;
	std::vector<std::shared_ptr<Node>> ShaderNodeList;

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

	unsigned int AssignTextureID();

	std::string GiveName();

	void ResetNameCounter();

	//Depth first backwards traversal and compilation of the nodes
	void CompileGraph(std::shared_ptr<Node> CurrentNode , std::shared_ptr<Node> rootNode);
	void ChangeShader(Shader* shader);
	
	void PrintConnections();
	void ResetGraph();
	void UpdateGraph();

	//utility functions etc
	std::string AssignUniqueName(std::string initName, std::string slotName);
	void ReplaceUniqueName(std::string newName,std::string slotName);
	std::string ReplaceVarNames(std::string code, std::string oldName, std::string newName);
	std::string ReplaceLine(std::string shaderCode, std::string identifier, std::string newLine);
	//this shouldn't be part of the graph
	std::string GetShaderPrefix(ShaderType type,bool usecase);
	void CreateProgramUniform(std::string Varname);
	void UpdateUniforms();

	//Datatype InitialiseValue(ValueType type);
	//TODO 
	// Implement function that checks if the graph contains any circles 
	bool CircleInGraph();

private :

	static Graph* Instance;
	int ID;
	Graph();
};

