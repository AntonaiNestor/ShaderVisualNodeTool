
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

	/* 
	 ConstantNode a(2.0);
	 ConstantNode b(3.0);
	 ConstantNode c(4.0);
	 ConstantNode d(5.0);*/
	
	/* AddNode add;
	 MultNode mult;
	 PowNode power;*/

	 Graph* graph = Graph::getInstance();
	 std::string code;


	 //add 2 and 3
	//x->CreateConnection(&a, &add, 0 , 0 );
	// x->CreateConnection(&b, &add, 0 , 1 );
	
	 //mult 4 and 5
	 //x->CreateConnectionOutIn(&c, &mult, 0, 0);
	 //x->CreateConnectionInOut(&mult, &d, 1, 0);
	
	 //// power and add the results from the above outputs
	 ////x->CreateConnection(&add, &power, 0, 0);
	 //
	 //x->CreateConnectionOutIn(&mult, &power, 0, 1);
	 //x->CreateConnectionInOut(&power, &a, 0, 0);
	
	 ////assign current root to graph
	 //x->root = &power;
	 ////Traverse and compile graph
	 //x->CompileGraph(x->root,&code);

	 //std::cout << "(2+3)^2 + (4*5)^2= " << power.CalculateValue() << std::endl;
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

	//probably need singleton
	GUIManager* GUI = GUIManager::getInstance();

	GUI->SetupGUI(window);

	GUI->CreateNode(ImVec2(100,100));
	GUI->CreateNode(ImVec2(400, 100));
	GUI->CreateNode(ImVec2(100, 300));
	GUI->CreateNode(ImVec2(400, 300));
	
	/*
	VisualNode Visual1(&a, ImVec2(100, 100));
	VisualNode Visual2(&b, ImVec2(300, 100));
	VisualNode Visual3(&c, ImVec2(100, 300));
	VisualNode Visual4(&d, ImVec2(300, 300));

	GUI->VNodeList.push_back(&Visual1);
	GUI->VNodeList.push_back(&Visual2);
	GUI->VNodeList.push_back(&Visual3);
	GUI->VNodeList.push_back(&Visual4);*/
	//GUI->vnode = &Visual;


	bool show_demo_window = true;
	bool show_another_window = false;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//ImVec2 initPos;
	//ImVec2 endPos;

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

			//Render GUI
			GUI->RenderGUI();
		

			// OpenGL Rendering in scene
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


			GUI->RenderCall();
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