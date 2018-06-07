#include "Graph.h"
#include <iostream>
#include <fstream>
#include "../json.hpp"
#include "../Utility.h"
#include "InputNode.h"
#include "TimeNode.h"


Graph::Graph()
{
	ID = 0;
	NameCounter = 0;

	//populate the vector of string with 5 empty ones
	for (int i =0; i < 5 ; i++){
		CodeSections.push_back("");
	}
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
	if (typeid(*node) == typeid(TimeNode) ){
		UniformNodes.push_back(node);
	}
}

void Graph::ReadNodeTypes(std::string FilePath)
{
	//Read the json file in a json object
	using json = nlohmann::json;
	json j;

	

	std::ifstream ifs(FilePath.c_str(),std::ifstream::in);

	if (!ifs.is_open()) {
		// Debug Log this and try catch
		std::cout << "Error with opening Json File" << std::endl;
	}
	ifs >> j;
	ifs.close();
	//------------------------------------

	//Parsing json information and building nodetypes in graph containers

	if (j.find("NodeTypes") != j.end()) {
		json info = j["NodeTypes"];
		

		for (int i = 0; i < info.size(); i++) {
			
			FunctionNodeType tempNode;
			json objectRead = info.at(i);

			std::string type = objectRead["NodeType"];
			std::string name = objectRead["Name"];

			tempNode.FilterNodeType = type;
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

			//code 
		
			auto codeArray = objectRead["Code"];
			std::string codeLines = "";
			for (std::string codeline : codeArray){
				
				codeLines.append(codeline + "\n");
			}
			
			tempNode.Code = codeLines;


			//ADD TO GRAPH INFO
			FunctionNodes.insert(std::pair<std::string, FunctionNodeType>(tempNode.Name,tempNode));
		}
		

	}





}

void Graph::CompileGraph(std::shared_ptr<Node> CurrentNode , std::string* ShaderCode)
{
	

	//Traverse all the Inputs list of this node
	for (std::vector<Connection>::iterator it = CurrentNode->Input.begin(); it != CurrentNode->Input.end(); ++it) {


		//if the connection 
		if (it->ConnectedNode == nullptr) { continue; }

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

void Graph::ChangeShader(Shader* shader)
{

	shader->EditShader(*ShaderCode, FRAGMENT);
}

void Graph::PrintConnections()
{
	std::cout << "-------------------Print Graph Start -----------------" << std::endl;
	for (auto node : NodeList) {
		std::cout << " Node : " << node->UniqueID << " is connected with :" << std::endl;
		int counter = 0;
		for (auto in : node->Output) {

			if (in.ConnectedNode != nullptr){
				std::cout << "-- Node : " << in.ConnectedNode->UniqueID << " From slot -> "<< counter++<< " To slot -> " << in.ConnectionIndex << std::endl;
			}
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

	ClearShaderCode();
	CompileGraph(root, ShaderCode);
	ChangeShader(daShader);
	//UpdateUniforms();
	ResetGraph();

}

std::string Graph::AssignUniqueName(std::string initName, std::string slotName)
{
	std::string finalName = initName;

	//if the unique slot name already is in the map then retrieve that name
	try {

		finalName = VarToSlotMap.at(slotName);
		
		////if the name exists in the map, then create a unique new name and insert that into the map.
		
		

	}
	//otherwise check if the name of the variable already exists
	//if it does generate a new name, otherwise simply push the pair in both maps
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

void Graph::WriteToShaderCode(std::string code, ShaderSection section)
{
	switch (section) {
		case (FragVersion): {
			CodeSections[0].append("\n" + code);
			//(*ShaderCode).insert((*ShaderCode).find("$Version$") + 1, "\n" + code);
			break;
		}
		case (FragVarying): {
			CodeSections[1].append("\n" + code);
			//(*ShaderCode).insert((*ShaderCode).find("$Varyings$") + 1, "\n" + code);
			break;
		}
		case (FragUniform): {
			CodeSections[2].append("\n" + code);
		//	(*ShaderCode).insert((*ShaderCode).find("$Uniforms$") + 1, "\n" + code);
			break;
		}
		case (FragConstant): {
			CodeSections[3].append("\n" + code);
			//(*ShaderCode).insert((*ShaderCode).find("$Constants$") + 1, "\n" + code);
			break;
		}
		case (FragMain): {
			CodeSections[4].append("\n" + code);
			//(*ShaderCode).insert((*ShaderCode).find("$Main$") , "\n" + code);
			break;
		}
		default: {
			CodeSections[4].append("\n" + code);
			//(*ShaderCode).insert((*ShaderCode).find("$Main$") + 6, "\n" + code);
			break;
		}




	}

}

void Graph::AssembleShaderCode()
{
	for (int i = 0; i < 5; i++) {
		(*ShaderCode).insert((*ShaderCode).find(Identifiers[i]) + Identifiers[i].length(), "\n" + CodeSections[i]);
	}

}

void Graph::ClearShaderCode()
{
	//clear shaderCode -- can skip this i think
	ShaderCode->clear();
	//clear sections
	for (int i = 0; i < 5; i++) {
		CodeSections[i].clear();
	}
	//initialize shadercode to skeleton code
	(*ShaderCode) = daShader->fragCode;
}

void Graph::UpdateUniforms()
{
	for (auto unode : UniformNodes) {
		if (typeid(*unode) == typeid(InputNode)) {
			dynamic_cast<InputNode&>(*unode).EditUniform();
		}
		else if (typeid(*unode) == typeid(TimeNode)) {
			dynamic_cast<TimeNode&>(*unode).EditUniform();
		}

		
	}

}

