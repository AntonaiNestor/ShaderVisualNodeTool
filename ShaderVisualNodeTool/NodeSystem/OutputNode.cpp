#include "OutputNode.h"



OutputNode::OutputNode(ShaderNodeInformation nodeInfo)
{

	//general node attributes
	Name = nodeInfo.Name;
	Type = BaseNodeType::OutputnodeT;
	CurrShaderType = 4;
	Stype = nodeInfo.ShadeType;
	UniqueID = Graph::getInstance()->AssignID();

	//default shader code
	DefaultCode[0] = nodeInfo.DefaultCode[0];
	DefaultCode[1] = nodeInfo.DefaultCode[1];
	DefaultCode[2] = nodeInfo.DefaultCode[2];
	shaderCode[0] = DefaultCode[0];
	shaderCode[1] = DefaultCode[1];
	shaderCode[2] = DefaultCode[2];
	HasCompiled = false;


	//Initialise the codesections vector
	for (int i = 0; i < 5; i++) {
		CodeSections[0].push_back("");
		CodeSections[1].push_back("");
		CodeSections[2].push_back("");
	}


	//std::string strArray[]{ "$color" };
	//output struct creation

	//these should be initialised either by json or depending on type , even though they will be standarised more or less
	// However it will be easier to create from json, except from the default code. That should be in the application once
	auto graph = Graph::getInstance();

	for (auto slot : nodeInfo.Slots) {

		Connection newConnection;

		newConnection.Name = slot.Name;
		newConnection.VariableType = slot.VarType;


		//The value init here might be wrong. These are special cases. The default code will handle everything
		switch (slot.VarType) {

		case (Bool): {
			newConnection.Value.b_var = graph->DefaultBool;
			break;
		}
		case(Float): {
			newConnection.Value.f_var = { graph->DefaultFloat };
			break;
		}
		case(Int): {
			newConnection.Value.i_var = { graph->DefaultInt };
			break;
		}
		case(Vec2): {
			newConnection.Value.vec2_var = { graph->DefaultVec2 };
			break;

		}
		case(Vec3): {
			newConnection.Value.vec3_var = { graph->DefaultVec3 };
			break;

		}
		case(Vec4): {
			newConnection.Value.vec4_var = { graph->DefaultVec4 };
			break;

		}
		case(Mat4): {
			newConnection.Value.mat4_var = { graph->DefaultMat4 };
			break;
		}

		default:

			break;
		}

		//Input slot
		if (!slot.SlotType) {
			Input.push_back(newConnection);
		}
		//Output Slot
		else {
			Output.push_back(newConnection);
		}
	}

	
}


OutputNode::~OutputNode()
{
}


void OutputNode::WriteToShaderCode(std::string code, ShaderSection section , ShaderType type)
{

	int shaderIndex = 0;
	switch (type) {
	
		case(VERTEX):{
			shaderIndex = 0;
			break;
		}
		case(GEOMETRY): {
			shaderIndex = 1;
			break;
		}
		case(FRAGMENT): {
			shaderIndex = 2;
			break;
		}
	}


	switch (section) {
	case (VersionSeg): {
		CodeSections[shaderIndex][0].append("\n" + code);
		//(*ShaderCode).insert((*ShaderCode).find("$Version$") + 1, "\n" + code);
		break;
	}
	case (VaryingSeg): {
		CodeSections[shaderIndex][1].append("\n" + code);
		//(*ShaderCode).insert((*ShaderCode).find("$Varyings$") + 1, "\n" + code);
		break;
	}
	case (UniformSeg): {
		CodeSections[shaderIndex][2].append("\n" + code);
		//	(*ShaderCode).insert((*ShaderCode).find("$Uniforms$") + 1, "\n" + code);
		break;
	}
	case (ConstantSeg): {
		CodeSections[shaderIndex][3].append("\n" + code);
		//(*ShaderCode).insert((*ShaderCode).find("$Constants$") + 1, "\n" + code);
		break;
	}
	case (MainSeg): {
		CodeSections[shaderIndex][4].append("\n" + code);
		//(*ShaderCode).insert((*ShaderCode).find("$Main$") , "\n" + code);
		break;
	}
	default: {
		CodeSections[shaderIndex][4].append("\n" + code);
		//(*ShaderCode).insert((*ShaderCode).find("$Main$") + 6, "\n" + code);
		break;
	}

	}

}

void OutputNode::AssembleShaderCode()
{

	
	auto graph = Graph::getInstance();

	for (int i = 0; i < 5; i++) {

		shaderCode[0].insert((shaderCode[0]).find(graph->Identifiers[i]) + graph->Identifiers[i].length(), "\n" + CodeSections[0][i]);
		shaderCode[1].insert((shaderCode[1]).find(graph->Identifiers[i]) + graph->Identifiers[i].length(), "\n" + CodeSections[1][i]);
		shaderCode[2].insert((shaderCode[2]).find(graph->Identifiers[i]) + graph->Identifiers[i].length(), "\n" + CodeSections[2][i]);
	}
	//this should go in the loop and cout all shaders
	std::cout << "--Vertex shader--- : " << std::endl;
	std::cout << shaderCode[0] << std::endl;
	std::cout << "------------------ " << std::endl;

	//this should go in the loop and cout all shaders
	std::cout << "--Geometry shader--- : " << std::endl;
	std::cout << shaderCode[1] << std::endl;
	std::cout << "------------------ " << std::endl;

	std::cout << "----Fragment shader ---- : " << std::endl;
	std::cout << shaderCode[2] << std::endl;
	std::cout << "------------------ " << std::endl;
}

void OutputNode::ClearShaderCode()
{
	
	//clear shaderCode -- can skip this i think
	shaderCode[0].clear();
	shaderCode[1].clear();
	shaderCode[2].clear();
	//clear sections
	for (int i = 0; i < 5; i++) {
		CodeSections[0][i].clear();
		CodeSections[1][i].clear();
		CodeSections[2][i].clear();
	}
	//initialize shadercode to skeleton code
	shaderCode[0] = DefaultCode[0];
	shaderCode[1] = DefaultCode[1];
	shaderCode[2] = DefaultCode[2];

}


void OutputNode::Compile(std::shared_ptr<Node> root)
{
	int counter = 0;
	
	//std::string strArray[]{ "$color" };
	auto Manager = Graph::getInstance();
	


	for (int i = 0; i < Input.size(); i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		if (Input.at(i).ConnectedNode) {
			//this is not correct. The final fragment outputs need to be accounted for
			std::string tempCode = "FragColor = " + Input.at(i).Name + " ;";
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Manager->ReplaceVarNames(tempCode, Input.at(i).Name, Manager->VarToSlotMap[SlotName]);
			Input.at(i).Value = Input.at(i).ConnectedNode->Output.at(0).Value;

			WriteToShaderCode(tempCode + CodeString(),MainSeg,FRAGMENT);

		
			//ShaderCode->append("\n" + tempCode + CodeString());
			//ShaderCode->append("\n" +Input.at(i).Name + " = " + std::to_string(Input.at(0).Value) + ";");
		}
		else {
			//If you are not connected with anything don't append anything

			//tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[i], "vec4(1.0f,1.0f,1.0f,1.0f)");
		}

	}
	

	//ShaderCode->append("\n" + tempCode + CodeString());



	HasCompiled = true;
}

std::string OutputNode::CodeString()
{

	return std::string(" \n //End of appending \n ");
}
