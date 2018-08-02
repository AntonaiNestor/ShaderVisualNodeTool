#include "OutputNode.h"
#include "InputNode.h"


OutputNode::OutputNode(ShaderNodeInformation nodeInfo)
{


	auto graph = Graph::getInstance();
	//general node attributes
	Name = nodeInfo.Name;
	Type = BaseNodeType::OutputnodeT;
	CurrShaderType = 4;
	Stype = nodeInfo.ShadeType;
	UniqueID = graph->AssignID();

	//default shader code
	DefaultCode[0] = nodeInfo.DefaultCode[0];
	DefaultCode[1] = nodeInfo.DefaultCode[1];
	DefaultCode[2] = nodeInfo.DefaultCode[2];
	DefaultCode[3] = nodeInfo.DefaultCode[3];
	DefaultCode[4] = nodeInfo.DefaultCode[4];
	shaderCode[0] = DefaultCode[0];
	shaderCode[1] = DefaultCode[1];
	shaderCode[2] = DefaultCode[2];
	shaderCode[3] = DefaultCode[3];
	shaderCode[4] = DefaultCode[4];
	HasCompiled = false;


	//Initialise the codesections vector
	for (int i = 0; i <graph->Identifiers.size() ; i++) {
		CodeSections[0].push_back("");
		CodeSections[1].push_back("");
		CodeSections[2].push_back("");
		CodeSections[3].push_back("");
		CodeSections[4].push_back("");
	}


	//output struct creation

	//these should be initialised either by json or depending on type , even though they will be standarised more or less
	// However it will be easier to create from json, except from the default code. That should be in the application once
	for (auto slot : nodeInfo.Slots) {

		

		//Input slot
		if (!slot.SlotType) {
			InputConnection newConnection;

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

			Input.push_back(newConnection);
		}
		//Output Slot
		else {
			OutputConnection newConnection;

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
			Output.push_back(newConnection);
		}
	}

}


OutputNode::~OutputNode()
{
}

//MOVE THIS TO RENDERER CHOLO
std::string OutputNode::CreateVaryingPipeline(ShaderType type,std::string varType, std::string varName, std::string assignValue)
{
	//the varying string that need to be written in the VS and the FS
	std::string varyingNameVS = "out " + varType + " v" + varName + " ;";
	std::string varyingNameFS = "in " + varType + " g" + varName + " ;";
	std::string varyingNameGSin = "in " + varType + " te" + varName + "[]" + " ;";
	std::string varyingNameGSout = "out " + varType + " g" + varName + " ;";

	std::string varyingDeclGS = "g" + varName + " = " + " te" + varName + "[i]" + " ;";
	std::string varyingDeclGSE = "g" + varName  + assignValue + " ;"; //what is this?

	//pretty hardcoded with the gl_invocationID but it is pretty standard
	std::string varyingDeclTCS = "tc" + varName + " [gl_InvocationID]" +" = " + " v" + varName + "[gl_InvocationID]" + " ;";
	std::string varyingDeclTCSE = "tc" + varName+ "[]" + assignValue + " ;"; //what is this?

	//this is problematic, I need to think of a better way
	std::string varyingDeclTES = "te" + varName + "+= "+ " tc" + varName + "[i]" +" * gl_TessCoord[i];";
	std::string varyingDeclTESinit = "te" + varName + "= " + util::GetStringZeroValueType(util::stringToValueType(varType)) +  ";";
	//std::string varyingDeclTES = "te" + varName + "= " + "tc" + varName + "[i]" + ";";
	std::string varyingDeclTESE = "te" + varName + assignValue + " ;"; //what is this?

	std::string varyingNameTCSin = "in " + varType + " v" + varName + "[]" + " ;"; //dealing with structs as input output
	std::string varyingNameTCSout = "out " + varType + " tc" + varName + "[]" + " ;";
	std::string varyingNameTESin = "in " + varType + " tc" + varName + "[]" + " ;"; //only input is a struct
	std::string varyingNameTESout = "out " + varType + " te" + varName + " ;";

	//assign value contains the = 
	//write in VS ,GS and  FS

	//If you are creating a varying from the vertex shader then do all the rest 
	if (type == VERTEX) {
		//Vertex
		WriteToShaderCode(varyingNameVS, VaryingSeg, VERTEX);

		//In function nodes, the actual declaration in the main segment is handled in their compile function. 
		WriteToShaderCode(assignValue, MainSeg, VERTEX);
		//TCS
		WriteToShaderCode(varyingNameTCSin, VaryingSeg, TESSELATION_CONTROL);
		WriteToShaderCode(varyingNameTCSout, VaryingSeg, TESSELATION_CONTROL);
		WriteToShaderCode(varyingDeclTCS, MainSeg, TESSELATION_CONTROL);
		//TES
		WriteToShaderCode(varyingNameTESin, VaryingSeg, TESSELATION_EVALUATION);
		WriteToShaderCode(varyingNameTESout, VaryingSeg, TESSELATION_EVALUATION);
		WriteToShaderCode(varyingDeclTESinit, MainSeg, TESSELATION_EVALUATION);
		WriteToShaderCode(varyingDeclTES, MainGeomSeg, TESSELATION_EVALUATION);
		//Geometry
		WriteToShaderCode(varyingNameGSin, VaryingSeg, GEOMETRY);
		WriteToShaderCode(varyingNameGSout, VaryingSeg, GEOMETRY);
		WriteToShaderCode(varyingDeclGS, MainGeomSeg, GEOMETRY);

		
	}
	else if (type == TESSELATION_CONTROL) {
		WriteToShaderCode(varyingNameTCSout, VaryingSeg, TESSELATION_CONTROL);
		WriteToShaderCode(varyingDeclTCSE, MainSeg, TESSELATION_CONTROL);
		//TES
		WriteToShaderCode(varyingNameTESin, VaryingSeg, TESSELATION_EVALUATION);
		WriteToShaderCode(varyingNameTESout, VaryingSeg, TESSELATION_EVALUATION);
		WriteToShaderCode(varyingDeclTES, MainSeg, TESSELATION_EVALUATION);
		//Geometry
		WriteToShaderCode(varyingNameGSin, VaryingSeg, GEOMETRY);
		WriteToShaderCode(varyingNameGSout, VaryingSeg, GEOMETRY);
		WriteToShaderCode(varyingDeclGS, MainGeomSeg, GEOMETRY);
	}

	else if (type == TESSELATION_EVALUATION) {
		WriteToShaderCode(varyingNameTESout, VaryingSeg, TESSELATION_EVALUATION);
		WriteToShaderCode(varyingDeclTESE, MainSeg, TESSELATION_EVALUATION);
		//Geometry
		WriteToShaderCode(varyingNameGSin, VaryingSeg, GEOMETRY);
		WriteToShaderCode(varyingNameGSout, VaryingSeg, GEOMETRY);
		WriteToShaderCode(varyingDeclGS, MainGeomSeg, GEOMETRY);
	}
	else if (type == GEOMETRY) {
		WriteToShaderCode(varyingNameGSout, VaryingSeg, GEOMETRY);
		//write it in the main geom seg. The floats are ok but the gl_position isn't if you write them in the main seg
		WriteToShaderCode(varyingDeclGSE, MainGeomSeg, GEOMETRY);
	}

	//you will always retrieve a varying from the Geometry shader to the fragment no matter what
	WriteToShaderCode(varyingNameFS, VaryingSeg, FRAGMENT);
	
    
	return ""; //return the new name? no because depending on the shader it will be used in then 
}

void OutputNode::WriteToShaderCode(std::string code, ShaderSection section , ShaderType type)
{

	int shaderIndex = 0;
	switch (type) {
	
		case(VERTEX):{
			shaderIndex = 0;
			break;
		}
		case(TESSELATION_CONTROL): {
			shaderIndex = 1;
			break;
		}
		case(TESSELATION_EVALUATION): {
			shaderIndex = 2;
			break;
		}
		case(GEOMETRY): {
			shaderIndex = 3;
			break;
		}
		case(FRAGMENT): {
			shaderIndex = 4;
			break;
		}
	
	}

	//Neat! way to go enums!
	CodeSections[shaderIndex][section].append("\n" + code);


	//maybe I can loop the enum for this
	//switch (section) {
	//case (VersionSeg): {
	//	CodeSections[shaderIndex][0].append("\n" + code);
	//	//(*ShaderCode).insert((*ShaderCode).find("$Version$") + 1, "\n" + code);
	//	break;
	//}
	//case (VaryingSeg): {
	//	CodeSections[shaderIndex][1].append("\n" + code);
	//	//(*ShaderCode).insert((*ShaderCode).find("$Varyings$") + 1, "\n" + code);
	//	break;
	//}
	//case (UniformSeg): {
	//	CodeSections[shaderIndex][2].append("\n" + code);
	//	//	(*ShaderCode).insert((*ShaderCode).find("$Uniforms$") + 1, "\n" + code);
	//	break;
	//}
	//case (ConstantSeg): {
	//	CodeSections[shaderIndex][3].append("\n" + code);
	//	//(*ShaderCode).insert((*ShaderCode).find("$Constants$") + 1, "\n" + code);
	//	break;
	//}
	//case (MainSeg): {
	//	//WRITE IN THE MAIN SEGMENT IN MULTIPLE OCCASIONS
	//	CodeSections[shaderIndex][4].append("\n" + code);
	//	//(*ShaderCode).insert((*ShaderCode).find("$Main$") , "\n" + code);
	//	break;
	//}
	//case (MainGeomSeg): {
	//	//WRITE IN THE MAIN SEGMENT IN MULTIPLE OCCASIONS
	//	CodeSections[shaderIndex][5].append("\n" + code);
	//	//(*ShaderCode).insert((*ShaderCode).find("$Main$") , "\n" + code);
	//	break;
	//}
	//default: {
	//	CodeSections[shaderIndex][4].append("\n" + code);
	//	//(*ShaderCode).insert((*ShaderCode).find("$Main$") + 6, "\n" + code);
	//	break;
	//}

	//}

}

void OutputNode::AssembleShaderCode()
{

	
	auto graph = Graph::getInstance();

	//Overall same thing
	//6 
	for (int i = 0; i < graph->Identifiers.size() ; i++) {
		
		// && 
		for (int j = 0; j < 5; j++) {
			//if the identifier exists then 
			
			if (((shaderCode[j]).find(graph->Identifiers[i]) != std::string::npos) && CodeSections[j][i].compare("")) {
				shaderCode[j].insert((shaderCode[j]).find(graph->Identifiers[i]) + graph->Identifiers[i].length(), "\n" + CodeSections[j][i]);
			}

			//shaderCode[j].insert((shaderCode[j]).find(graph->Identifiers[i]) + graph->Identifiers[i].length(), "\n" + CodeSections[j][i]);
		}
		
	}

	EditLayoutQualifiers();

	//this should go in the loop and cout all shaders
	std::cout << "--Vertex shader--- : " << std::endl;
	std::cout << shaderCode[0] << std::endl;
	std::cout << "------------------ " << std::endl;

	std::cout << "--TC shader--- : " << std::endl;
	std::cout << shaderCode[1] << std::endl;
	std::cout << "------------------ " << std::endl;

	std::cout << "--TE shader--- : " << std::endl;
	std::cout << shaderCode[2] << std::endl;
	std::cout << "------------------ " << std::endl;

	//this should go in the loop and cout all shaders
	std::cout << "--Geometry shader--- : " << std::endl;
	std::cout << shaderCode[3] << std::endl;
	std::cout << "------------------ " << std::endl;

	std::cout << "----Fragment shader ---- : " << std::endl;
	std::cout << shaderCode[4] << std::endl;
	std::cout << "------------------ " << std::endl;
}

void OutputNode::EditLayoutQualifiers()
{

	auto graph = Graph::getInstance();
	//Precompile change layouts and replace other options
	//Maybe this should be part of the assemble code in the graph
	std::string TCSlayout = "layout( vertices = " + std::to_string(graph->TCSVertNumberOut) + " ) out ;";
	shaderCode[1] = graph->ReplaceLine(shaderCode[1], "@", TCSlayout);


	//WHAT THE HELL?
	//You need to have at least a std::string in your + appends when initing for it to accept it
	// std::string TESlayout ="layout(" + (std::string)(",") + ",";
	std::string TESlayout = "layout(";
	TESlayout += graph->TESGenPrimMode;
	TESlayout += ",";
	TESlayout += graph->TESGenSpacing;
	TESlayout += ",";
	TESlayout += graph->TESVertexOrder;
	TESlayout += ") out ;";

	shaderCode[2] = graph->ReplaceLine(shaderCode[2], "@", TESlayout);


}

void OutputNode::ClearShaderCode()
{

	//clear shaderCode -- can skip this i think

	for (int i = 0; i < 5; i++) {
		//reset shadercode to default shader code
		shaderCode[i].clear();
		shaderCode[i] = DefaultCode[i];

		//clear all the codesections for each shader
		for (int j = 0; j < 6; j++) {
			CodeSections[i][j].clear();
		}
		
	}



}


void OutputNode::Compile(std::shared_ptr<Node> root)
{
	int counter = 0;
	
	//std::string strArray[]{ "$color" };
	auto Manager = Graph::getInstance();
	


	for (int i = 0; i < Input.size(); i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		if (Input.at(i).ConnectedNode) {
			
			//Here I am checking where the connected variable is coming from. If it is a varying then I need to append v or g.
			std::string shaderNamePrefix = "";

			//If the incoming shadertype is not the same OR for input nodes if it is attribute variable, it means it is a varying
			auto tempP = Input.at(i).ConnectedNode;
			if ((tempP->Type == InputnodeT && dynamic_cast<InputNode&>(*tempP).inputType == AttributeVariable)
				|| (tempP->Type == FunctionnodeT && tempP->CurrShaderType<CurrShaderType)) {
					
				shaderNamePrefix = Manager->GetShaderPrefix(ShaderType(CurrShaderType),false);
			
			}

			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(i).ConnectionIndex); 
			auto newName = shaderNamePrefix + Manager->VarToSlotMap[SlotName];

			//this is not correct. The final fragment outputs need to be accounted for
			std::string tempCode = "FragColor = " + newName + " ;";
			
			//prefix + unique name associated with slot
			

			//tempCode = Manager->ReplaceVarNames(tempCode, Input.at(i).Name, newName);
			//Input.at(i).Value = Input.at(i).ConnectedNode->Output.at(0).Value;

			WriteToShaderCode(tempCode + CodeString(),MainSeg,FRAGMENT);

	
		}
		else {
			//If you are not connected with anything don't append anything

			//tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[i], "vec4(1.0f,1.0f,1.0f,1.0f)");
		}

	}
	

	HasCompiled = true;
}

std::string OutputNode::CodeString()
{

	return std::string(" \n //End of appending \n ");
}
