#include "InputNode.h"
#include "OutputNode.h"
#include <iostream>

InputNode::InputNode()
{
	//general node attributes
	Name = "Float";
	Type = BaseNodeType::InputnodeT;
	inputType = InputNodeType::ConstantVariable;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	//varName = "ConstantFloat";
	//value = DefaultValue;
	HasCompiled = false;


	//output struct creation
	Connection connect;
	
	//connect.Value = 1.0f;
	connect.VariableType = Float;
	connect.Name = "FloatVar";

	Output.push_back(connect);
	//Input.push_back(connect);


}



InputNode::InputNode(InputNodeInformation nodeInfo)
{

	auto graph = Graph::getInstance();
	Type = BaseNodeType::InputnodeT;
	inputType = InputNodeType(nodeInfo.InitInputType);
	UniqueID = graph->AssignID();
	
	HasCompiled = false;
	Name = nodeInfo.Name;
	
	//output  slots
	for (int i = 0 ; i<nodeInfo.SlotNames.size();i++){

		//output struct creation
		Connection connect;
		connect.ConnectedNode = nullptr;
		connect.ConnectionIndex = -1;
		connect.VariableType = nodeInfo.VarTypes[i];
		connect.Enabled = true;
		connect.Name = nodeInfo.SlotNames[i];

		switch (connect.VariableType) {
			case (Float): {
				connect.Value.f_var = graph->DefaultFloat;
				value.f_var = graph->DefaultFloat;
				break;
			}
			case (Int): {

				connect.Value.i_var = graph->DefaultInt;
				value.i_var = graph->DefaultInt;
				break;
			}
			case (Vec2): {

				connect.Value.vec2_var = graph->DefaultVec2;
				value.vec2_var = graph->DefaultVec2;
				break;
			}
			case (Vec3): {

				connect.Value.vec3_var = graph->DefaultVec3;
				value.vec3_var = graph->DefaultVec3;
				break;
			}
			case (Vec4): {

				connect.Value.vec4_var = graph->DefaultVec4;

				value.vec4_var = graph->DefaultVec4;
				break;
			}
			case (Mat4): {

				connect.Value.mat4_var = graph->DefaultMat4;
				value.mat4_var = graph->DefaultMat4;
				break;
			}
			case (Sampler2D): {
				//Name = "Texture";

				break;
			}
			case(SamplerCube): {
				//Name = "SamplerCube";
				break;
			}
			default: {
				//Name = "Float";
				break;
			}
		}
		Output.push_back(connect);
	
	}
}


InputNode::~InputNode()
{
	std::cout << " Destroying Constant Node " << std::endl;
}


//
// Compiling constant nodes means only appending a simple var in the shadercode
// The actual value of the node is also passed to the children nodes on connection
void InputNode::Compile(std::shared_ptr<Node> root) {
	
	
	auto Manager = Graph::getInstance();

	//A uniform is automatically writen in all shader types  
	if (inputType == UniformVariable) {
	
		Manager->CreateProgramUniform(CodeString());
	}

	//Vertex Attributes : SPECIAL CASE
	else if (inputType == AttributeVariable){

		//do nothing in regards of writing a declaration of the variable
		// We also know that this will execute uniquely for the VS. It CANNOT run for anything else

		//TODO INTERACE BLOCKS
		//The varying maybe should be done with interface blocks.

		//Go through all outputs of the attributes and create the necessary name declarations as varyings or not 
		for (int i = 0; i < Output.size(); i++) {
			
			if (Output[i].ConnectedNode!=nullptr)
			{
				//Assign unique name for this slot output
				std::string name = Output.at(i).Name;
				std::string slotName = std::to_string(this->UniqueID) + "->" + std::to_string(i);
				ValueType outputType = Output.at(i).VariableType;
				
				name = Manager->AssignUniqueName(name, slotName);
				

				std::string VSattrDecl;

				//if the output is connected to a type of shader that is greater than VERTEX, Input nodes do not have a specific shadertype.
				// They either write to whatever the connected node is or to all or special cases likes this

				//TODO : When we have multiple outs, then I will need to check all of the connected node types
				if (Output[i].ConnectedNode->CurrShaderType > VERTEX){

					//These attributes are always handled in the vertex shader so prefix is always "v"
					std::string prefName = "v" + name;

					//depending on the variable name, write the appropriate match between the varying and the attribute
					//Hardcoded , maybe there is a way to detach this from here?
					//Also there might be more attributes in the future
					//Attribute names are hardcoded as well e.g aPos;
					switch (i) {
						case (0):
							VSattrDecl = prefName + DeclareAttribute(i, ShaderType(CurrShaderType));// " = aPos ;";
							break;
						case(1):
							VSattrDecl = prefName + " = aNormal ;";
							break;
						case(2):
							VSattrDecl = prefName + " = aTexCoords;";
							break;
						case(3):
							VSattrDecl = prefName + " = aTangents;";
							break;
						case(4):
							VSattrDecl = prefName + " = aBitangents;";
							break;
						default:
							VSattrDecl = prefName + " = aPos ;";
							break;
					}

					dynamic_cast<OutputNode&>(*root).CreateVaryingPipeline(VERTEX,util::GetStringValueType(outputType, false),name,VSattrDecl);

				
				
				
				}
				//if the attribute is connected to a vertexshader execution node then do not have a prefix. Just write the name 
				// However since this is isn't a varying anymore we need to declare the valuetype as well
				else {
					switch (i) {
					case (0):
						VSattrDecl = util::GetStringValueType(outputType, false) +  name + " = aPos ;";
						break;
					case(1):
						VSattrDecl = util::GetStringValueType(outputType, false) + name + " = aNormal ;";
						break;
					case(2):
						VSattrDecl = util::GetStringValueType(outputType, false) + name + " = aTexCoords;";
						break;
					case(3):
						VSattrDecl = util::GetStringValueType(outputType, false) + name + " = aTangents;";
						break;
					case(4):
						VSattrDecl = util::GetStringValueType(outputType, false) + name + " = aBitangents;";
						break;
					default:
						VSattrDecl = util::GetStringValueType(outputType, false) + name + " = aPos ;";
						break;
					}
					dynamic_cast<OutputNode&>(*root).WriteToShaderCode(VSattrDecl, MainSeg, VERTEX);
				}

			}
		}
		

	}
	else if (inputType == ConstantVariable){
		//Constant nodes will write to the immediate connected node type.
		// TODO : This must repeat for all connected nodes don't forget
		ShaderType writeType = ShaderType(Output[0].ConnectedNode->CurrShaderType);
		dynamic_cast<OutputNode&>(*root).WriteToShaderCode(CodeString(), ConstantSeg, writeType);
	}
	// Also unique case but they are UniformMatrices
	else if (inputType == TransformationMatrix) {
		//The transformation matrix nodes do not need to do anything on compile
		// They are essentially only here to provide a name, and only the visual part 
		//changes the out name

		//Put cases here for uniforms and whatnot

		std::string name = Output.at(0).Name;
		std::string slotName = std::to_string(this->UniqueID) + "->" + std::to_string(0);
		ValueType outputType = Output.at(0).VariableType;
		Manager->ReplaceUniqueName(name, slotName);
		//However maybe here I should consider declaring the more complex matrices like 
		// ModelView or ProjectionView instead of having to create them from the application
		// Problem with this is that you cannot have them as uniforms and calculate them in place
	}
	
	HasCompiled = true;
	
}

std::string InputNode::CodeString()
{
	// TODO this probably needs to check if there are more than one outputs and append that much text
	// Also for now we do not add the "$"
	
	auto ManagerInstance = Graph::getInstance();
	std::string StringVal;


	//for now just declare them in the main
	if (inputType == UniformVariable) {


		std::string name = Output.at(0).Name;
		std::string slotName = std::to_string(this->UniqueID) + "->0";
		ValueType outputType = Output.at(0).VariableType;
		name = ManagerInstance->AssignUniqueName(name, slotName);

		StringVal += "uniform ";
		switch (outputType) {


		case(Bool): {
			StringVal += "bool " + name + ";";
			break;
		}

		case(Float): {
			StringVal += "float " + name +  ";" ;
			break;
		}
		case(Int): {
			StringVal += "int " + name +  ";";
			break;
		}
		case(Vec2): {
			StringVal += "vec2 " + name + ";";
			break;

		}
		case(Vec3): {
			StringVal += "vec3 " + name + ";";
			break;

		}
		case(Vec4): {
			StringVal += "vec4 " + name + " ;";
			break;

		}
		case(Mat4): {
			StringVal += "mat4 " + name + " ;";
			break;
		}

		case(Sampler2D): {
			StringVal += "Sampler ";
			break;

		}
		case(SamplerCube): {
			StringVal += "SamplerCube ";
			break;

		}
		default:

			break;
		}
		
	}
	else if (inputType == ConstantVariable){

		std::string name = Output.at(0).Name;
		std::string slotName = std::to_string(this->UniqueID) + "->0";
		ValueType outputType = Output.at(0).VariableType;
		name = ManagerInstance->AssignUniqueName(name, slotName);

		Datatype val = value;
		switch (outputType) {


		case(Bool): {
			StringVal += "bool " + name + " = " + std::to_string(val.b_var) + ";";
			break;
		}

		case(Float): {
			StringVal += "float " + name + " = " + std::to_string(val.f_var) + ";";
			break;
		}
		case(Int): {
			StringVal += "int " + name + " = " + std::to_string(val.i_var) + ";";;
			break;
		}
		case(Vec2): {
			StringVal += "vec2 " + name + " =  vec2(" + std::to_string(val.vec2_var.x) + " , " + std::to_string(val.vec2_var.y) + ") ;";
			break;

		}
		case(Vec3): {
			StringVal += "vec3 " + name + " =  vec3(" + std::to_string(val.vec3_var.x) + " , " + std::to_string(val.vec3_var.y) + " , " + std::to_string(val.vec3_var.z) + ") ;";
			break;

		}
		case(Vec4): {
			StringVal += "vec4 " + name + " =  vec4(" + std::to_string(val.vec4_var.x) + " , " + std::to_string(val.vec4_var.y) + " , " + std::to_string(val.vec4_var.z) + " , " + std::to_string(val.vec4_var.w) + ") ;";
			break;

		}
		case(Mat4): {
			//this should be taking the values from the value of the node
			StringVal += "mat4 " + name + "= mat4 (1.0f);";
			break;
		}

		case(Sampler2D): {
			StringVal += "Sampler ";
			break;

		}
		case(SamplerCube): {
			StringVal += "SamplerCube ";
			break;

		}
		default:

			break;
		}
	}
	else if (inputType == AttributeVariable) {


		for (int i = 0; i < Output.size(); i++) {
			//if the output is connected to something
			if (Output[i].ConnectedNode != nullptr)
			{

				//Put cases here for uniforms and whatnot

				std::string name = Output.at(i).Name;
				std::string slotName = std::to_string(this->UniqueID) + "->" + std::to_string(i);
				ValueType outputType = Output.at(i).VariableType;


				name = ManagerInstance->AssignUniqueName(name, slotName);
			}
		}
	
	}
	

	return StringVal;
}

void InputNode::EditUniform()
{
	//Program ID , this needs to be passed as a parameter maybe
	auto ManagerInstance = Graph::getInstance();
	Shader* program = ManagerInstance->daShader;
	ValueType outputType = Output.at(0).VariableType;
	std::string name = Output.at(0).Name;
	std::string slotName = std::to_string(this->UniqueID) + "->0";
	std::string uniqueOutName = ManagerInstance->AssignUniqueName(name,slotName);

	program->Use();

	unsigned int uloc = glGetUniformLocation(program->ID, uniqueOutName.c_str());

	switch (outputType) {


	case(Bool): {
		//bool value = Output[0].Value.b_var;
		glUniform1i(uloc,value.b_var);
		
		break;
	}

	case(Float): {
		//float value = Output[0].Value.f_var;
		glUniform1f(uloc, value.f_var);
		break;
	}
	case(Int): {
		//int value = Output[0].Value.i_var;
		glUniform1i(uloc, value.i_var);
		break;
	}
	case(Vec2): {
		//glm::vec2 value= Output[0].Value.vec2_var;
		glUniform2f(uloc,value.vec2_var.x,value.vec2_var.y);
		break;

	}
	case(Vec3): {
		//glm::vec3 value = Output[0].Value.vec3_var;
		glUniform3f(uloc, value.vec3_var.x, value.vec3_var.y,value.vec3_var.z);
		break;

	}
	case(Vec4): {
		//glm::vec4 value = Output[0].Value.vec4_var;
		glUniform4f(uloc, value.vec4_var.x, value.vec4_var.y, value.vec4_var.z,value.vec4_var.w);
		break;

	}
	case(Mat4): {
		//glm::mat4 value = Output[0].Value.mat4_var;
		glUniformMatrix4fv(uloc,1,GL_FALSE ,glm::value_ptr(value.mat4_var));
		break;
	}

	case(Sampler2D): {
		break;

	}
	case(SamplerCube): {
		break;

	}
	default:

		break;
	}

	glUseProgram(0);

}

std::string InputNode::DeclareAttribute(int index, ShaderType type)
{
	//5 cases of types. Each type has only a certain amount of attributes
	switch (type) {
	case(VERTEX): {
	
	}
		break;

	case(GEOMETRY): {
	
	}
		break;

	default:
		break;
	}

}





