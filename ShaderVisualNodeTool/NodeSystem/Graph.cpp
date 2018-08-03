#include "Graph.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"
#include "../Utility.h"
#include "InputNode.h"
#include "TimeNode.h"
#include "OutputNode.h"
#include "TextureNode.h"


Graph::Graph()
{
	ID = 0;
	NameCounter = 0;
	TextureCounter = 0;

	//initialize the list with empty maps
	for (int i = 0; i < FunctionNodeTypes.size(); i++) {
	
		std::vector<std::string> temp ;
		FunctionSubCategories.push_back(temp);
	}


	//populate the vector of string with 5 empty ones
	
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


		//create the connection between slots
		
		//if the input slot that we are connecting to was already connected with something
		//Reset that connection correctly
		//This still might leave in the adjacency list the info that the connected node -> to node
		if (to->Input.at(ToIndex).ConnectedNode != nullptr) {
			RemoveConnection(to->Input.at(ToIndex).ConnectedNode, to, to->Input.at(ToIndex).ConnectionIndex, ToIndex);
		}

		//same goes for this output slot as well. We need to update whatever the node this slot was connected to if there 
		//was one. 

		//TODO THIS SHOULD BE REMOVED WHEN I MAKE INPUTS TO BE PASSED TO MORE OUTPUTS
		/*if (from->Output.at(FromIndex).ConnectedNode != nullptr) {
			RemoveConnection(from,from->Output.at(FromIndex).ConnectedNode,FromIndex , from->Output.at(FromIndex).ConnectionIndex);
		}*/

		from->ConnectNode(to, ToIndex, FromIndex);


		auto x = find(AdjacencyList.at(i).begin(), AdjacencyList.at(i).end(), j);
		//if i and j are not already connected
		if (AdjacencyList.at(i).empty()||
				x== AdjacencyList.at(i).end() ){

				//create connection i - > j ,  directed graph so only one way
				AdjacencyList.at(i).push_back(j);

				// in case i want to have the connections two ways
				//AdjacencyList.at(j).push_back(i);

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
		std::cout << " node cannot be connected to itself" << std::endl;
		return false;
	}

	//no need to remove the connection from the output slot of TO this time
	//I will do it for now because i think i do not allow connection between multiple slots 
	//at the moment

	//TODO REMOVE THIS FIRST ONE IF YOU WANT TO HAVE MULTIPLE OUTPUTS
	/*if (to->Output.at(ToIndex).ConnectedNode != nullptr) {
		RemoveConnection(to,to->Output.at(ToIndex).ConnectedNode, ToIndex, to->Output.at(ToIndex).ConnectionIndex);
	}*/

	


	//if both nodes have valid IDs
	if (i < AdjacencyList.size() && j < AdjacencyList.size()) {


		if (from->Input.at(FromIndex).ConnectedNode != nullptr) {
			RemoveConnection(from->Input.at(FromIndex).ConnectedNode, from, from->Input.at(FromIndex).ConnectionIndex, FromIndex);
		}

		to->ConnectNode(from, FromIndex, ToIndex);


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
	//From is always the output node and to is always the input node

	//Delete the connection from the outnode by searching in the slot's list and completely removing that entry
	//I do not care about the index of that list so no issue with that 
	//CAUTION : This will result in error if I do use extra indices

	//Go to the output slot and remove the connection that led to the connected node
	from->Output.at(FromIndex).ResetConnection(to);

	////just for not accessing the point again and again
	//auto tempList = from->Output[FromIndex];

	//for (int i = 0; i < tempList.size(); i++) {
	//	
	//	//if you found the entry in the out slot list that points to the about to delete connection, clear it
	//	if (tempList[i].ConnectedNode == to && tempList[i].ConnectionIndex == ToIndex) {
	//		from->Output.at(FromIndex).at(i).ResetConnection();
	//		//remove the entry from the vector as well, no need to keep it
	//		from->Output.at(FromIndex).erase(from->Output.at(FromIndex).begin() + i);
	//	}
	//}

	//Clear Input at
	to->Input.at(ToIndex).ResetConnection();

	//here we need to check (as in the replace function ) if by removing this connection we 
	// lose a connection between nodes for the adjacency list.

	//Instead of checking all outputs which are now many , check if nodes are connected from the input side
	// You smart son (m.)
	bool StillConnected = false;

	for (auto inslot : to->Input) {
		
		if (inslot.ConnectedNode == from) {
			StillConnected = true;
			break;

		}

	}

	//for (auto outslot : from->Output) {
	//	if (!StillConnected) {
	//		for (auto connection : outslot) {
	//			if (connection.ConnectedNode == to) {
	//				StillConnected = true;
	//				break;
	//			}
	//		}
	//	}
	//	else break;
	//	
	//}


	if (!StillConnected) {

		int fromID = from->UniqueID;
		int toID = to->UniqueID;
		
		// Removes the information that To node is connected to From node
		//If from  -> to existed then remove it  (to from did not exist since this a directed graph)
	/*	for (std::vector<int>::iterator it = AdjacencyList.at(toID).begin(); it != AdjacencyList.at(toID).end(); ++it)
		{
			if (*it == fromID)
			{
				AdjacencyList.at(toID).erase(it);
				break;

			}
		}*/


		for (std::vector<int>::iterator it = AdjacencyList.at(fromID).begin(); it != AdjacencyList.at(fromID).end(); ++it)
		{
			if (*it == toID)
			{
				AdjacencyList.at(fromID).erase(it);
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

unsigned int Graph::AssignTextureID()
{
	return TextureCounter++;
}

std::string Graph::GiveName()
{
	return std::to_string(NameCounter++);
}

void Graph::ResetNameCounter()
{
	NameCounter = 0;
}

void Graph::AddNode(std::shared_ptr<Node> node) {
	NodeList.push_back(node);


	//REMOVE FROM HERE
	if (typeid(*node) == typeid(TimeNode) ||
		typeid(*node) == typeid(TextureNode)){
		UniformNodes.push_back(node);
	}
}

void Graph::ReadNodeTypes(std::string FilePath)
{
	using json = nlohmann::json;
	json j;

	//--- Read Input Nodes

	std::ifstream inputifs((FilePath + "/InputNodes.json").c_str(), std::ifstream::in);

	if (!inputifs.is_open()) {
		// Debug Log this and try catch
		std::cout << "Error with opening Json File" << std::endl;
	}
	inputifs >> j;
	inputifs.close();


	//Parsing json information and building nodetypes in graph containers

	if (j.find("NodeTypes") != j.end()) {
		json info = j["NodeTypes"];

		for (int i = 0; i < info.size(); i++) {


			json objectRead = info.at(i);
			InputNodeInformation tempInput;

			std::string name = objectRead["Name"];

			tempInput.Name = name;
			

			//Initial Input Node Type

			std::string inputType = objectRead["InitVariableType"];

			//5 here is the size of the InputType enum, consider counting them dynamically
			for (int i = 0; i < 5; i++) {
				if (inputType.compare(VariableTypes[i]) == 0) {
					tempInput.InitInputType = i;
					break;
				}
			}

			tempInput.ShaderType = -1;
			std::string st = objectRead["ShaderType"];
			for (int i = 0; i < 5 ;i ++) {
				if (st.compare(ShaderTypes[i]) == 0) {
					tempInput.ShaderType = i;
					break;
				}
			}

			//Slots
			auto slots = objectRead["Slots"];
			for (auto slot : slots) {

				//temporary slot vars -- only one slot here but keep it in a loop

				std::string sname = slot["SlotName"];

				tempInput.SlotNames.push_back(sname);
				tempInput.VarTypes.push_back(util::stringToValueType(slot["VariableType"]));
			}

			InputNodes.insert(std::pair<std::string, InputNodeInformation>(name, tempInput));
		}
	}
	//------------------------------------


	//--- Read Function Nodes

	//Read the json file in a json object

	std::ifstream ifs((FilePath + "/FunctionNodes.json").c_str(), std::ifstream::in);

	if (!ifs.is_open()) {
		// Debug Log this and try catch
		std::cout << "Error with opening Json File" << std::endl;
	}
	ifs >> j;
	ifs.close();


	//Parsing json information and building nodetypes in graph containers

	if (j.find("NodeTypes") != j.end()) {
		json info = j["NodeTypes"];
		

		for (int i = 0; i < info.size(); i++) {
			
			FunctionNodeInformation tempNode;
			json objectRead = info.at(i);

			std::string type = objectRead["NodeType"];
			std::string name = objectRead["Name"];
			
			tempNode.FilterNodeType = type;
			tempNode.Name = name;

			//Allowed execution shaders
			auto shadertypes = objectRead["AvailableShaders"];
			for (std::string stype : shadertypes) {

				for (int i = 0; i < 5; i++) {
					if (stype.compare(ShaderTypes[i]) == 0) {
						tempNode.AllowedExecShaders.push_back(i);
					}
				}
			
			}
			
			 
			//Slots
			auto slots = objectRead["Slots"];
			for (auto slot : slots) {

				//temporary slot vars
				bool stype = util::stringToSlotType(slot["SlotType"]);
				std::string sname = slot["SlotName"];
				ValueType vartype = util::stringToValueType(slot["VariableType"]);

				SlotInformation tempSlot;
				tempSlot.Name = sname;
				tempSlot.SlotType = stype;
				tempSlot.VarType = vartype;
				tempNode.Slots.push_back(tempSlot);
				
			}

			//code 
		
			auto codeArray = objectRead["Code"];
			std::string codeLines = "";
			for (std::string codeline : codeArray){
				
				codeLines.append(codeline + "\n");
			}
			
			tempNode.Code = codeLines;


			//ADD TO GRAPH INFO
			FunctionNodes.insert(std::pair<std::string, FunctionNodeInformation>(tempNode.Name,tempNode));
			
			std::string NodeCategory = objectRead["Category"];
			int count = 0;
			for (std::string subcategoryName : FunctionNodeTypes) {
				if (NodeCategory.compare(subcategoryName) == 0) {
					FunctionSubCategories.at(count).push_back(tempNode.Name);
					break;
				}
				count++;
			}

		}
		

	}


	//--- Read Shader Nodes
	

	std::ifstream ioutfs((FilePath + "/ShaderNodes.json").c_str(), std::ifstream::in);

	if (!ioutfs.is_open()) {
		// Debug Log this and try catch
		std::cout << "Error with opening Json File" << std::endl;
	}
	ioutfs >> j;
	ioutfs.close();


	//Parsing json information and building nodetypes in graph containers

	if (j.find("NodeTypes") != j.end()) {
		json info = j["NodeTypes"];

		for (int i = 0; i < info.size(); i++) {

			ShaderNodeInformation tempNode;
			json objectRead = info.at(i);

			
			std::string name = objectRead["Name"];
			tempNode.Name = name;


			//Slots
			auto slots = objectRead["Slots"];
			for (auto slot : slots) {

				//temporary slot vars
				bool stype = util::stringToSlotType(slot["SlotType"]);
				std::string sname = slot["SlotName"];
				ValueType vartype = util::stringToValueType(slot["VariableType"]);

				SlotInformation tempSlot;
				tempSlot.Name = sname;
				tempSlot.SlotType = stype;
				tempSlot.VarType = vartype;
				tempNode.Slots.push_back(tempSlot);

			}


			//TEMPORARY PROBABLY
			tempNode.ShadeType = FRAGMENT;
			tempNode.DefaultCode[0] = daShader->vertCode;
			tempNode.DefaultCode[1] = daShader->tessControlCode;
			tempNode.DefaultCode[2] = daShader->tessEvalCode;
			tempNode.DefaultCode[3] = daShader->geomCode;
			tempNode.DefaultCode[4] = daShader->fragCode;
			//type and default code from graph according to name
			if (name.compare("Vertex") == 0) {
				tempNode.ShadeType = VERTEX;
				//tempNode.DefaultCode = daShader->vertCode;
			}
			else if (name.compare("Tess_Control") == 0) {
				tempNode.ShadeType = TESSELATION_CONTROL;
				//tempNode.DefaultCode = daShader->fragCode;
			}
			else if (name.compare("Tess_Eval") == 0) {
				tempNode.ShadeType = TESSELATION_EVALUATION;
				//tempNode.DefaultCode = daShader->fragCode;
			}
			else if (name.compare("Geometry") == 0) {
				tempNode.ShadeType = GEOMETRY;
				//tempNode.DefaultCode = daShader->fragCode;
			}
			else if (name.compare("Fragment") == 0) {
				tempNode.ShadeType = FRAGMENT;
				//tempNode.DefaultCode = daShader->fragCode;
			}
		
			//add to graph permanent information
			ShaderNodes.insert(std::pair<std::string, ShaderNodeInformation>(tempNode.Name, tempNode));
		}


	}



}

void Graph::CompileGraph(std::shared_ptr<Node> CurrentNode , std::shared_ptr<Node> rootNode)
{
	
	/*
	
	for (all inputs of the node){
	
		if (slot is not connected to anything){
			continue;
		}

		else if (slot is connected to something) {
			
			//this check for compilation might be problematic once an output is connected to many inputs
			if (the connected node has not been compiled already)
			{
			  Call compileGraphfunction for that node();
			}

		}
	
	}
	
	--after all inputs have been checked then
	Compile this Node();

	if (this is a shadernode) {
				do nothing -> default shader code will take care of it
			}
			else {
				use default value 			
			}
	
	*/

	//Traverse all the Inputs list of this node
	for (std::vector<InputConnection>::iterator it = CurrentNode->Input.begin(); it != CurrentNode->Input.end(); ++it) {


		//if the connection 
		if (it->ConnectedNode == nullptr) { continue; }
		else {
			if (!it->ConnectedNode->HasCompiled) {
				CompileGraph(it->ConnectedNode,rootNode);
			}
			
		
		}

		//if the connected node's inputs are empty 
		//it means that it is an input node only
		
		//if (it->ConnectedNode->Input.empty() && !it->ConnectedNode->HasCompiled) {
		//	//if that node hasn't been compiled, compile it and continue to this node's next input
		//	it->ConnectedNode->Compile(ShaderCode);
		//	
		//	continue;
		//}
		//// if it isn't empty and the node hasn't compiled  move to that node and call this function again
		//else if (!it->ConnectedNode->HasCompiled) {
		//	
		//}
	}

	CurrentNode->Compile(rootNode);
}

void Graph::ChangeShader(Shader* shader)
{
	//change shader will edit the shadercodes existing in the root "fragment shader" 
	//YOU MONSTER

	//shader->EditShader(dynamic_cast<OutputNode&>(*root).shaderCode[0], dynamic_cast<OutputNode&>(*root).shaderCode[1], dynamic_cast<OutputNode&>(*root).shaderCode[2]) ;
	shader->EditShader(dynamic_cast<OutputNode&>(*root).shaderCode[0], dynamic_cast<OutputNode&>(*root).shaderCode[1], dynamic_cast<OutputNode&>(*root).shaderCode[2], dynamic_cast<OutputNode&>(*root).shaderCode[3], dynamic_cast<OutputNode&>(*root).shaderCode[4]);
}





void Graph::PrintConnections()
{
	std::cout << "-------------------Print Graph Start -----------------" << std::endl;
	for (auto node : NodeList) {
		std::cout << " Node : " << node->UniqueID << " is connected with :" << std::endl;
		int counter = 0;
		for (auto  Outslot: node->Output) {
			
			for (int i = 0; i < Outslot.ConnectedNode.size(); i ++) {
					
				std::cout << "-- Node : " << Outslot.ConnectedNode[i]->UniqueID << " From slot -> " << counter << " To slot -> " << Outslot.ConnectionIndex[i] << std::endl;
					
			}
			
			counter++;
			
		}
		std::cout << "------------------------------------------------------" << std::endl;
	}

	std::cout << "-------------------Print End -----------------" << std::endl;
}

void Graph::ResetGraph()
{
	for (auto &node : NodeList) {
		node->HasCompiled = false;
	}
	//ResetNameCounter();
	//SlotToVariableMap.clear();
	//VarToSlotMap.clear();
	
}

void Graph::UpdateGraph()
{
	PrintConnections();

	//ClearShaderCode();

	//first compile all shadernodes except the root

	for (int i = 0; i < ShaderNodeList.size(); i ++) {
		CompileGraph(ShaderNodeList.at(i),root);
	}

	//then start from the root
	CompileGraph(root,root);
	dynamic_cast<OutputNode&>(*root).AssembleShaderCode();
	ChangeShader(daShader);
	dynamic_cast<OutputNode&>(*root).ClearShaderCode();
	//UpdateUniforms();
	ResetGraph();

}

std::string Graph::AssignUniqueName(std::string initName, std::string slotName)
{

	std::string finalName = initName;

	// CHANGE THIS WITH COUNT INSTEAD OF EXCEPTION AND ALSO RETHINK IF THERE ARE CASES
	// WERE A NAME ISNT UNIQUE BUT IS SHARED BY MULTIPLE SLOTNAMES e.g Transformation matrix
	//if the unique slot name already is in the map then retrieve that name
	try {

		finalName = VarToSlotMap.at(slotName);
		
		////if the name exists in the map, then create a unique new name and insert that into the map.
		
	}
	//otherwise check if the name of the variable already exists in the other map
	//if it does generate a new name, otherwise simply push the pair in both maps to establish the name
	catch (std::out_of_range) {
		
		try {
			SlotToVariableMap.at(finalName);
			//Since we are reseting the namecounter and the map, there is no reason for that
			//Output.at(0).Name = name + "_"+  Graph::getInstance()->GiveName();
			finalName = initName + "_" + GiveName();
			SlotToVariableMap.insert(std::pair<std::string, std::string>(finalName, slotName));
			VarToSlotMap.insert(std::pair<std::string, std::string>(slotName, finalName));

		}
		catch (std::out_of_range){
			SlotToVariableMap.insert(std::pair<std::string, std::string>(finalName, slotName));
			VarToSlotMap.insert(std::pair<std::string, std::string>(slotName, finalName));
		}

		
	}


	return finalName;
}

//instead of returning the unique name that is connected to the slot, it will replace it
void Graph::ReplaceUniqueName(std::string newName, std::string slotName)
{
	std::string finalName;
	//if the unique slot name already is in the map then replace it with the new name 
	if (VarToSlotMap.count(slotName) > 0) {
		VarToSlotMap[slotName] = newName;
		//Will there be an issue with the opposite map? Probably there will be an issue ATTENTION
	}
	//if the slotname doesn't have a name associated yet , simply insert it
	else {
		VarToSlotMap[slotName] = newName;
	}
	
}

std::string Graph::ReplaceVarNames(std::string code, std::string oldName, std::string newName)
{

	if (oldName.empty())
		return "";
	size_t start_pos = 0;
	while ((start_pos = code.find(oldName, start_pos)) != std::string::npos) {
		code.replace(start_pos, oldName.length(), "");
		code.insert(start_pos, newName);
		start_pos += newName.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return code;
}

std::string Graph::ReplaceLine(std::string shaderCode, std::string identifier, std::string newLine)
{
	int endLineSize = 2;
	int posInit = shaderCode.find(identifier);
	int identifierEndline = shaderCode.find("\n", posInit);
	int posEnd = shaderCode.find("\n",identifierEndline+1);

	

	shaderCode.replace(identifierEndline + 1, (posEnd ) - (identifierEndline +1), newLine);

	return shaderCode;
}

//usecase : 0 -> read
//		  : 1 -> write
std::string Graph::GetShaderPrefix(ShaderType type, bool usecase)
{
	switch (type) {

	case(VERTEX): {
		//Vertex shader appends v in both cases 
		return "v";
		break;
	}
	case(TESSELATION_CONTROL): {
		return  usecase ? "tc" : "v";
		break;
	}
	case(TESSELATION_EVALUATION): {
		return  usecase ? "te" : "tc";
		break;
	}
	case(GEOMETRY): {
		//this will change 
		return  usecase ? "g" : "te";
		break;
	}
	case(FRAGMENT): {
		
		return "g";
		break;
	}
	
	
	default:
		break;
	}
		
	return std::string();
}

void Graph::CreateProgramUniform(std::string Varname)
{
	dynamic_cast<OutputNode&>(*root).WriteToShaderCode(Varname, UniformSeg, VERTEX);
	dynamic_cast<OutputNode&>(*root).WriteToShaderCode(Varname, UniformSeg, TESSELATION_CONTROL);
	dynamic_cast<OutputNode&>(*root).WriteToShaderCode(Varname, UniformSeg, TESSELATION_EVALUATION);
	dynamic_cast<OutputNode&>(*root).WriteToShaderCode(Varname, UniformSeg, GEOMETRY);
	dynamic_cast<OutputNode&>(*root).WriteToShaderCode(Varname, UniformSeg, FRAGMENT);
}

void Graph::UpdateUniforms()
{
	for (auto unode : UniformNodes) {

		if (typeid(*unode) == typeid(InputNode) || typeid(*unode) == typeid(TextureNode)) {
			dynamic_cast<InputNode&>(*unode).EditUniform();
		}
		else if (typeid(*unode) == typeid(TimeNode)) {
			dynamic_cast<TimeNode&>(*unode).EditUniform();
		}

		
	}

}



