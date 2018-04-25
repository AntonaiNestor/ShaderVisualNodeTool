
//OpenGL related
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

//General C++ and data structures
#include <fstream>
#include <iostream>

//Shader -- Rendering 
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

//Importing / Loading
#include "Rendering/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Nodes and graph
#include "NodeSystem/Graph.h"
#include "NodeSystem/ConstantNode.h"
#include "NodeSystem/AddNode.h"
#include "NodeSystem/MultNode.h"
#include "NodeSystem/PowNode.h"

//imgui and UI related

#include "GUI/GUIManager.h"

//Misc
#include "Utility.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,Shader *shader);

char* ReadFileData(const char* fileName);

void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS);



// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
bool pressed=false;
//testing, these should not be global
float InnerTesselationLevel = 1.0;
float OuterTesselationLevel = 1.0;
const char* shaderStageName = nullptr;
int Options = 0;


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	//glfwInit();
	if (!glfwInit())
	{
		std::cout << "What broke?" << std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ShaderNodeTool", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	Shader shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.fs");
	Shader otherShader("Shaders/SimpleVertexShader.vert","Shaders/SimpleFragmentShader.fs");
	//Shader shader("SimpleVertexShader.vs", "SimpleFragmentShader.fs","TrianglesToPoints.gs");
	//Shader shader("SimpleVertexShader.vs", "SimpleFragmentShader.fs", "TrianglesToPoints.gs", "SimpleTesControl.tcs", "SimpleTesEval.tes");


	//LOAD TEXTURE 
	

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//std::vector<float>  verts = {
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f,  0.5f, 0.0f,   // top left 
	//	0.5f,  0.5f, 0.0f,  // top right
	//	
	//};

	//std::vector<float> verts = {
	//	
	//	// front
	////  |---Position---|-TexCoords-|
	//	-0.5, -0.5,  0.5, 0.0, 0.0,	 //bot left 
	//	0.5, -0.5, 0.5, 1.0, 0.0,	// bot right
	//	0.5,  0.5,  0.5, 1.0, 1.0,	// top right
	//	-0.5, 0.5, 0.5, 0.0, 1.0,	// top left
	//	// back
	//	-0.5, -0.5, -0.5, 1.0, 0.0, //bot left
	//	0.5, -0.5, -0.5,  0.0, 0.0, // bot right
	//	0.5,  0.5, -0.5, 0.0, 1.0,//top right
	//	-0.5,  0.5, -0.5, 1.0, 1.0// top left
	//
	//};
	//std::vector<float> verts = {


	//	//  |---Position---|-TexCoords-|

	//	// front bottom left 
	//	-0.5, -0.5,  0.5, 0.0, 0.0,  //0
	//	-0.5, -0.5,  0.5, 1.0, 0.0, // 1
	//	-0.5, -0.5,  0.5, 0.0, 1.0, // 2

	//	//front bot right
	//	0.5, -0.5, 0.5, 1.0, 0.0,//3	
	//	0.5, -0.5, 0.5, 0.0, 0.0,//4
	//	0.5, -0.5, 0.5, 1.0, 1.0,//5
	//	
	//	//front top left
	//	-0.5, 0.5, 0.5, 0.0, 1.0, //6
	//	-0.5, 0.5, 0.5, 1.0, 1.0, //7
	//	-0.5, 0.5, 0.5, 0.0, 0.0, //8


	//	//front top right
	//	0.5,  0.5,  0.5, 1.0, 1.0, //9
	//	0.5,  0.5,  0.5, 0.0, 1.0, //10
	//	0.5,  0.5,  0.5, 1.0, 0.0, //11

	//	//back bot right
	//	0.5, -0.5, -0.5,  0.0, 0.0, //12
	//	0.5, -0.5, -0.5,  1.0, 0.0, //13
	//	0.5, -0.5, -0.5,  1.0, 0.0, //14
	//
	//	// back bot left
	//	-0.5, -0.5, -0.5, 1.0, 0.0, //15
	//	-0.5, -0.5, -0.5, 0.0, 0.0, //16
	//	-0.5, -0.5, -0.5, 0.0, 0.0, //17
	//	
	//	//back top left
	//	-0.5,  0.5, -0.5, 1.0, 1.0, //18
	//	-0.5,  0.5, -0.5, 0.0, 1.0, //19
	//	-0.5,  0.5, -0.5, 0.0, 1.0, //20

	//	//back top right

	//	0.5,  0.5, -0.5, 0.0, 1.0, //21
	//	0.5,  0.5, -0.5, 1.0, 1.0, //22
	//	0.5,  0.5, -0.5, 1.0, 1.0  //23
	//};

	/*std::vector<float> verts = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};*/
	//std::vector<float> normals = {

	//	// front
	//	0.0, 0.0,  1.0,
	//	0.5, -0.5, 0.5,
	//	0.5, 0.5,  0.5,
	//	-0.5,  0.5,  0.5,
	//	// back
	//	-0.5, -0.5, -0.5,
	//	0.5, -0.5, -0.5,
	//	0.5,  0.5, -0.5,
	//	-0.5,  0.5, -0.5,

	//};


	
	//std::vector<unsigned int> indices = {  
	//	// note that we start from 0
	//	// front
	//	0, 1, 2,
	//	2, 3, 0,
	//	// top
	//	1, 5, 6,
	//	6, 2, 1,
	//	// back
	//	7, 6, 5,
	//	5, 4, 7,
	//	// bottom
	//	4, 0, 3,
	//	3, 7, 4,
	//	// left
	//	4, 5, 1,
	//	1, 0, 4,
	//	// right
	//	3, 2, 6,
	//	6, 7, 3,  
	//};

	//std::vector<unsigned int> indices = {
	//	//front 
	//	0, 3, 6,
	//	3, 9, 6,

	//	//back
	//	12,15,21,
	//	15,18,21,

	//	//left
	//	16,1,19,
	//	1,7,19,

	//	//right
	//	4,13,10,
	//	13,22,10,

	//	//top
	//	8,11,20,
	//	11,23,20,
	//	//bot
	//	17,14,2,
	//	14,5,2

	//}; 

	////is it even beneficial to convert to vec3? 
	//Mesh test(util::Arrays3ToVec3(verts),indices);

	// LOAD MODEL
	//Make Path relevant
	char path[] = "D:/GithubProjects/ShaderNodeTool/ShaderNodeTool/Resources/Models/Suzanne.obj";
	

	//// return a mesh object created from the extracted mesh data
	// Mesh test1(vertices, indices);

	 Model ourModel(path);
	//setup uniforms

	 //First we create the UBO for the matrices required in the vertex shader which is 
	 //shared between the programs



	 // create transformations
	
	 glm::mat4 view;
	 glm::mat4 projection;

	 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), //camera pos
		 glm::vec3(0.0f, 0.0f, 0.0f), //camera target
		 glm::vec3(0.0f, 1.0f, 0.0f)); //up
									   
	 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

	 unsigned int UBO;
	 glGenBuffers(1, &UBO);
	 glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	 //the actual memory allocation here 
	 glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4)*2 , NULL, GL_STATIC_DRAW); // allocate 3xmat4 bytes of memory
	 glBindBuffer(GL_UNIFORM_BUFFER, 0);

	 //Bind the UBO in binding point 0 
	 //we can set a subset of the buffer object if we want - here it is the whole thing
	 glBindBufferRange(GL_UNIFORM_BUFFER, 0,UBO, 0, 2 * sizeof(glm::mat4));

	 //upload data
	 //Buffer View mat
	 glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	 //Buffer Projection mat
	 glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	 


	 // Testing graph and nodes

	 //Graph graph;

	 
	 ConstantNode a(2.0,100,100);
	 ConstantNode b(3.0,200, 200);
	 ConstantNode c(4.0, 300, 300);
	 ConstantNode d(5.0 , 400, 400);
	
	 AddNode add;
	 MultNode mult;
	 PowNode power;

	 Graph* x = Graph::getInstance();
	 std::string code;


	 //add 2 and 3
	//x->CreateConnection(&a, &add, 0 , 0 );
	// x->CreateConnection(&b, &add, 0 , 1 );
	
	 //mult 4 and 5
	 x->CreateConnectionOutIn(&c, &mult, 0, 0);
	 x->CreateConnectionInOut(&mult, &d, 1, 0);
	
	 // power and add the results from the above outputs
	 //x->CreateConnection(&add, &power, 0, 0);
	 
	 x->CreateConnectionOutIn(&mult, &power, 0, 1);
	 x->CreateConnectionInOut(&power, &a, 0, 0);
	
	 //assign current root to graph
	 x->root = &power;
	 //Traverse and compile graph
	 x->CompileGraph(x->root,&code);

	 std::cout << "(2+3)^2 + (4*5)^2= " << power.CalculateValue() << std::endl;
	 std::cout << code << std::endl;


	//-------------glslang test
	// glslang::InitializeProcess();
	// glslang::TShader langShader(EShLangVertex);

	//// //always pass true as the second parameter to check and find the suffix
	////// FindLanguage("SimpleVertexShader.vert", true);
	//// //std::cout <<  << std::endl;
	// std::string filename = "Shaders/SimpleVertexShader.vert";
	// ShaderCompUnit unit(FindLanguage(filename, true));
	// char* fileText = ReadFileData(filename.c_str());
	// EProfile profile= EProfile::ECoreProfile;
	// unit.addString(filename, fileText);
	// //langShader.setStrings((const char* const *)"SimpleVertexShader.vert",);
	// EShMessages messages = EShMsgDefault;
	// const int defaultVersion = Options & EOptionDefaultDesktop ? 110 : 100;
	// langShader.setStringsWithLengthsAndNames(unit.text, NULL, unit.fileNameList, unit.count);
	//bool parseTest=langShader.parse(&Resources, defaultVersion, profile, true,
	//	 true, messages);
	 




	 //first shader
	shader.Use();

	glm::mat4 model;
	
	//// retrieve the matrix uniform locations
	unsigned int modelLoc = glGetUniformLocation(shader.ID, "Model");
	//unsigned int viewLoc = glGetUniformLocation(shader.ID, "View");
	//unsigned int projLoc = glGetUniformLocation(shader.ID, "Projection");
	//// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)		);
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	float color[] = {1.0,0.0,0.0,1.0};
	int vertexColorLocation = glGetUniformLocation(shader.ID, "InputCol");
	glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
	//shader.setFloat("test",0.5);
	shader.SetFloat("TessLevelInner", 1.0);
	shader.SetFloat("TessLevelOuter", 1.0);
	
	glUseProgram(0);

	otherShader.Use();
	// create transformations
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
	//// retrieve the matrix uniform locations
	unsigned int model1Loc = glGetUniformLocation(otherShader.ID, "Model");
	//unsigned int view1Loc = glGetUniformLocation(otherShader.ID, "View");
	//unsigned int proj1Loc = glGetUniformLocation(otherShader.ID, "Projection");
	//// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(model1Loc, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(view1Loc, 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(proj1Loc, 1, GL_FALSE, glm::value_ptr(projection));


	 float color1[] = { 0.0,1.0,0.0,1.0 };
	vertexColorLocation = glGetUniformLocation(otherShader.ID, "InputCol");
	glUniform4f(vertexColorLocation, color1[0], color1[1], color1[2], color1[3]);
	//shader.setFloat("test",0.5);
	
	glUseProgram(0);



	//General GL options
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glPatchParameteri(GL_PATCH_VERTICES, 3);



	// INIT IMGUI ///////////////////////////


	// Setup ImGui binding
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	//ImGui_ImplGlfwGL3_Init(window, true);



	// Setup style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();


	bool windowOpen = true;
	
	ImVec2 NodePos(100, 100); //position relevant to the nodeview Window

	ImVec2 NodeSize(200, 75);
	float NodeValue = 5.0f;
	int NodeID = 90;
	std::string NodeName = "Constant";


	GUIManager GUI;

	GUI.SetupGUI(window);

	VisualNode Visual(&GUI,&a, ImVec2(100, 100));
	GUI.vnode = &Visual;


	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImVec2 initPos;
	ImVec2 endPos;

	bool drawPermant = false;
	bool drawingLine = false;
	//---------------------------------------------------


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	 /*glEnable(GL_CULL_FACE);
	 glCullFace(GL_BACK);
	 glFrontFace(GL_CW);*/

	const double maxFPS = 60.0;
	const double maxPeriod = 1.0 / maxFPS;
	// approx ~ 16.666 mS
	double lastTime = glfwGetTime();
	double initTime = glfwGetTime();
	int framerate = 0;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		double time = glfwGetTime();
		double deltaTime = time - lastTime;

		bool flag = true;
		glfwPollEvents();

		/////////IMGUI///////
		
		
	
		////////////////////////////////////////////////////////////////////////////


		if (time - initTime >= 1.0) {
			//std::cout << "FPS: " << framerate << std::endl;
			framerate = 0;
			initTime = time;
		}

		if (deltaTime >= maxPeriod) {
			lastTime = time;
			framerate++;
			// code here gets called with max FPS
		
			// input
			// -----
			processInput(window,&shader);
			
			//new


			GUI.RenderGUI();
			
			//ImGui_ImplGlfwGL3_NewFrame();

			
			//ImGui::ShowDemoWindow();
			//x->DrawConstantNodes();
			
			//-non resizable window
			//-specific dimensions that change depending on the type of constant node
			//-at  Position NodePos
			

			// Call next window pos
			
			//ImVec2 NodeViewPos(500,200);
			//ImVec2 NodeViewSize(800,600);
			////Call next window size
			//ImGui::SetNextWindowPos(NodeViewPos, 0 ,ImVec2(0.0,0.0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
			//ImGui::SetNextWindowSize(NodeViewSize, 0);    // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
			//ImGui::Begin("Node View", &windowOpen,2);
			////the drawlist in the main GUI Manager for the node view
			//ImDrawList* drawList = ImGui::GetWindowDrawList();




				//Node 1
			
			////draw info
			//float titleBoxHeight = 20;
			//float contentPaddingX = 20;
			//float contentPaddingY = 15;
			//ImVec2 titlePads(30, 5);
			////draw Node box
			//drawList->ChannelsSetCurrent(0); // Background

		 //   ImGui::SetCursorScreenPos(NodePos+NodeViewPos);

			////Moveable node by title 
			//ImGui::InvisibleButton("node", ImVec2(NodeSize.x, titleBoxHeight));
			//if (ImGui::IsItemHovered())
			//{

			//}
			//bool node_moving_active = ImGui::IsItemActive();

			//if (node_moving_active && ImGui::IsMouseDragging(0))
			//	NodePos = NodePos + ImGui::GetIO().MouseDelta;


			////this values need to be relevant to the zooming, for now everything will be fixed
			//drawList->AddRectFilled(NodePos + NodeViewPos, NodePos + NodeViewPos+ NodeSize, ImColor(40, 40, 40),10.0);
			//drawList->AddRectFilled(NodePos + NodeViewPos, NodePos + NodeViewPos + ImVec2(NodeSize.x, titleBoxHeight), ImColor(150, 50, 150), 10, ImDrawCornerFlags_Top);
			//drawList->AddRect(NodePos + NodeViewPos, NodePos + NodeViewPos + NodeSize, ImColor(255,255,255),10.0);
			//drawList->AddLine(NodePos + NodeViewPos + ImVec2(NodeSize.x *3/4, titleBoxHeight), NodePos + NodeViewPos + ImVec2(NodeSize.x * 3 / 4, NodeSize.y), ImColor(255, 255, 255));
			//
			//

			////Title text
			//ImGui::SetCursorScreenPos(NodePos + NodeViewPos+ titlePads);
			//ImGui::Text( "Constant :  ID - %d", NodeID);

			////Main Body
			////ImGui::Spacing();
			//ImGui::SetCursorScreenPos(NodePos + NodeViewPos + ImVec2(contentPaddingX,titleBoxHeight+contentPaddingY));
			//ImGui::BeginGroup(); // Lock horizontal position
			//ImGui::Text("Float :");
			//ImGui::SameLine();
			//ImGui::PushItemWidth(50);
			//ImGui::InputFloat("", &NodeValue,  0.0f,  0.0f, 3, 0);

			//drawList->AddCircleFilled(ImVec2(NodeViewPos.x+NodePos.x+NodeSize.x ,NodeViewPos.y+NodePos.y+NodeSize.y*0.6) , 5, ImColor(255,255,255),12);
			//ImGui::EndGroup();

			// output button(s)

			
			//drawList->AddLine(NodePos + NodeViewPos +ImVec2(0,titlePadding), NodePos + NodeViewPos + ImVec2(NodeSize.x,titlePadding),ImColor(255, 255, 255));
			//ImVec2 NodePos1(1000, 400);
			//	
			//	ImGui::Text("Node ID:  %d", NodeID);
			//	//List of node's constant values outputs.
			//	// For now only one

			//	ImGui::Spacing();
			//	ImGui::Text("Node Value:");
			//	ImGui::SameLine();
			//	ImGui::InputFloat("", &NodeValue );
			//	
			//

			////	drawList->ChannelsSetCurrent(0); // Background

			//	//ImGui::SetCursorScreenPos(NodePos);
			//	ImVec2 test(1250, 450);
			//	ImVec2 test1(1400, 550);
			//	ImVec2 OutputPos(NodePos.x + NodeSize.x, NodePos.y + NodeSize.y / 2);
			//	OutputPos = test;

			//	ImVec2 OutputPos1(NodePos1.x + NodeSize.x, NodePos1.y + NodeSize.y / 2);
			//	OutputPos1 = test1;


			//	//SLOT 1 

			//	//circle filled = input/output with invisible button on top
			//	// we can add that to the draw list at a specific location
			//	drawList->AddCircleFilled(OutputPos, 10.0f, ImColor(75.0f, 75.0f, 75.0f, 1.0f), 12);
			//	//for the invisible button i have to change the cursor location and then 
			//	//create the button  with specific dimensions
			//	ImGui::SetCursorScreenPos(ImVec2(OutputPos.x-10.0f,OutputPos.y-10));
			//	if (ImGui::InvisibleButton("Button",ImVec2(20.0f,20.0f))) {
			//		//std::cout << "Button Clicked" << std::endl;
			//		//drawingLine = true;
			//		
			//	}
			//	//if the last button is held down
			//	if (ImGui::IsItemActive() && !drawingLine) {
			//		//drawHermite(drawList, OutputPos, ImGui::GetMousePos(), 20);
			//		drawingLine = true;
			//		initPos = OutputPos;
			//	}

			//	//if you release the mouse while drawing a line on top of this button
			//	if (ImGui::IsItemHovered() && drawingLine && ImGui::IsMouseReleased(0)) {
			//		// create a permanent connection between them 
			//		// add to the list of permanent connections

			//		//for now we only make this theendPos for the drawing part
			//		//NEED TO CHECK HERE IF IT IS THE SAME AS THE STARTING POSITION
			//		endPos = OutputPos;
			//		drawingLine = false;
			//		drawPermant = true;

			//	}

			//	

			//	// SLOT 2

			//	//circle filled = input/output with invisible button on top
			//	// we can add that to the draw list at a specific location
			//	drawList->AddCircleFilled(OutputPos1, 10.0f, ImColor(75.0f, 75.0f, 75.0f, 1.0f), 12);
			//	//for the invisible button i have to change the cursor location and then 
			//	//create the button  with specific dimensions
			//	ImGui::SetCursorScreenPos(ImVec2(OutputPos1.x - 10.0f, OutputPos1.y - 10));
			//	if (ImGui::InvisibleButton("Button1", ImVec2(20.0f, 20.0f))) {
			//		std::cout << "Button1 Clicked" << std::endl;
			//		//mouseDrag = true;
			//	}
			//	//if the last button is held down
			//	if (ImGui::IsItemActive() && !drawingLine) {
			//		//drawHermite(drawList, OutputPos, ImGui::GetMousePos(), 20);
			//		drawingLine = true;
			//		initPos = OutputPos1;
			//	}

			//	//if you release the mouse while drawing a line on top of this button
			//	if (ImGui::IsItemHovered() && drawingLine && ImGui::IsMouseReleased(0)) {
			//		// create a permanent connection between them 
			//		// add to the list of permanent connections

			//		//for now we only make this theendPos for the drawing part
			//		//NEED TO CHECK HERE IF IT IS THE SAME AS THE STARTING POSITION
			//		endPos = OutputPos1;
			//		drawingLine = false;
			//		drawPermant = true;

			//	}



			//	//DRAWING

			//	//releasing the mouse not when ontop of a slot
			//	if (ImGui::IsMouseReleased(0) && drawingLine) {
			//		drawingLine = false;

			//		//std::cout << "MouseReleased" << std::endl;
			//	}

			//	//draw the visual trace of a line
			//	if (drawingLine) {
			//		drawHermite(drawList, initPos, ImGui::GetMousePos(), 20);
			//	}

			//	//draw a permanent connection
			//	if (drawPermant) {
			//		drawHermite(drawList, initPos,endPos, 20);
			//	}

			//	drawList->ChannelsMerge();
			//	

			//	//ImGui::ArrowButton("Button");
			//	//ImGui::NewLine();

			//ImGui::End();



			/*float wrap_width = 100;
			ImGui::Text("Test paragraph 1:");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x+ wrap_width, pos.y), ImVec2(pos.x  + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255, 0, 255, 255));
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x+ wrap_width);
			ImGui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
			ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
			ImGui::PopTextWrapPos();*/


			//static bool inited = false;
			//static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
			//static bool show_grid = true;
			//static int node_selected = -1;
			//glm::vec2 NodePos(300.0, 300.0);
			//float value = 2.0f;
			//ImDrawList* draw_list = ImGui::GetWindowDrawList();

			//ImGui::PushID(2);
			//ImVec2 node_rect_min = ImVec2(NodePos.x, NodePos.y); //node pos = mouse pos on creation -> pass it initially from constructor
			//ImVec2 node_rect_max = ImVec2(node_rect_min.x + 100, node_rect_min.y + 100);
			//// Display node contents first
			//draw_list->ChannelsSetCurrent(1); // Foreground
			//bool old_any_active = ImGui::IsAnyItemActive();
			////ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);

			//ImGui::BeginGroup(); // Lock horizontal position
			//ImGui::Text("%s", "TestNode"); // display name 
			//ImGui::SliderFloat("##value", &value, 0.0f, 1.0f, "Value: %.2f"); //display value of the node
			//																  //ImGui::ColorEdit3("##color", &node->Color.x);
			//ImGui::EndGroup();

			//// Save the size of what we have emitted and whether any of the widgets are being used
			//bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
			////node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
			////ImVec2 node_rect_max = node_rect_min + node->Size;

			//// Display node box
			//draw_list->ChannelsSetCurrent(0); // Background
			//ImGui::SetCursorScreenPos(node_rect_min);
			////ImGui::InvisibleButton("node", node->Size);

			//bool node_moving_active = ImGui::IsItemActive();
			//if (node_widgets_active || node_moving_active)
			//	node_selected = 1;
			//if (node_moving_active && ImGui::IsMouseDragging(0))
			//	//node->Pos = node->Pos + ImGui::GetIO().MouseDelta;*/

			//	//ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
			//	draw_list->AddRectFilled(node_rect_min, node_rect_min, IM_COL32(75, 75, 75, 255), 4.0f);
			//draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
			///*for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
			//draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
			//for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
			//draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
			//*/
			//ImGui::PopID();

			//draw_list->ChannelsMerge();



			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// draw our first triangle
			//glUseProgram(shaderProgram);
			//shader.use();
			

			ourModel.Draw(shader);
			
			ourModel.Draw(otherShader);
			//ourModel.Draw(shader);
			//test1.Draw(shader);
			// glBindVertexArray(0); // no need to unbind it every time 


			GUI.RenderCall();
			//ImGui::Render();
			//ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			
		}


		
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------


	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window,Shader *shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	
		if (!pressed) {
			shader->AddToProgram("SimpleFragmentShader1.fs", ShaderType::FRAGMENT);
			//shader->AddToProgram("TrianglesToPoints.gs", ShaderType::GEOMETRY);
			shader->Use();
			// reupload uniforms that are unique to the new shader program
			glm::mat4 model;
			unsigned int modelLoc = glGetUniformLocation(shader->ID, "Model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			float color[] = { 0.0,0.0,1.0,1.0 };
			int vertexColorLocation = glGetUniformLocation(shader->ID, "InputCol");
			glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);


			
			pressed = true;
		}
		
	}
		


	//inrease Inner tess level
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE) {
		InnerTesselationLevel++;
		shader->SetFloat("TessLevelInner", InnerTesselationLevel);
	}
		
	//decrease Inner tess level
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
		if (InnerTesselationLevel>1.0)
			InnerTesselationLevel--;
		shader->SetFloat("TessLevelInner", InnerTesselationLevel);
	}
		
	//increase Outer tess level
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
	{
		OuterTesselationLevel++;
		shader->SetFloat("TessLevelOuter", OuterTesselationLevel);
	}
	//decreate Outer Tess Level
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
		if (OuterTesselationLevel>1.0)
			OuterTesselationLevel--;
		shader->SetFloat("TessLevelOuter", OuterTesselationLevel);
	}
		
}


ImVec2 AddImVec2(ImVec2 a, ImVec2 b) {
	return ImVec2(a.x + b.x, a.y + b.y);
}

void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS)
{
	ImVec2 t1 = ImVec2(+80.0f, 0.0f);
	ImVec2 t2 = ImVec2(+80.0f, 0.0f);

	for (int step = 0; step <= STEPS; step++)
	{
		float t = (float)step / (float)STEPS;
		float h1 = +2 * t*t*t - 3 * t*t + 1.0f;
		float h2 = -2 * t*t*t + 3 * t*t;
		float h3 = t * t*t - 2 * t*t + t;
		float h4 = t * t*t - t * t;
		drawList->PathLineTo(ImVec2(h1*p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x, h1*p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y));
	}

	drawList->PathStroke(ImColor(200, 200, 100), false, 3.0f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



char* ReadFileData(const char* fileName)
{
	FILE *in = nullptr;
	int errorCode = fopen_s(&in, fileName, "r");
	if (errorCode || in == nullptr)
		//Error("unable to open input file");
		std::cout << "Error error" << std::endl;

	int count = 0;
	while (fgetc(in) != EOF)
		count++;

	fseek(in, 0, SEEK_SET);

	char* return_data = (char*)malloc(count + 1);  // freed in FreeFileData()
	if ((int)fread(return_data, 1, count, in) != count) {
		free(return_data);
		//Error("can't read input file");
	}

	return_data[count] = '\0';
	fclose(in);

	return return_data;
}
