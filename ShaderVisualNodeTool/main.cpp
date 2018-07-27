
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
#include "NodeSystem/InputNode.h"


//imgui and UI related

#include "GUI/GUIManager.h"

//Misc
#include "Utility.h"
#include <direct.h> 



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


union utest {

	float f;
	int i;
	//glm::vec2 vec_2;

};

//struct Test {
//	utest un;
//	
//	Test() { un.f = 1.0f; };
//};

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


	//Shader otherShader("Shaders/SimpleVertexShader.vert","Shaders/SimpleFragmentShader.fs");
	//Shader shader("SimpleVertexShader.vs", "SimpleFragmentShader.fs","TrianglesToPoints.gs");
	//Shader shader("SimpleVertexShader.vs", "SimpleFragmentShader.fs", "TrianglesToPoints.gs", "SimpleTesControl.tcs", "SimpleTesEval.tes");


	//LOAD TEXTURE 
	

	

//UNION TESTS
	utest example = {1.0f};
	utest example1 = {2};
	

	bool testbool = true;

	std::cout << std::to_string(example.f) << std::endl;
	testbool = false;
	std::cout << std::to_string(testbool) << std::endl;
	example1= example;
	std::cout << example1.f << std::endl;
	
	std::string str = "test";
	std::cout << str + " Const char" << std::endl;
	str.clear();
	std::cout << str << std::endl;
	std::cout << str.compare("");



//------------------------------------------

	////is it even beneficial to convert to vec3? 
	//Mesh test(util::Arrays3ToVec3(verts),indices);

	// LOAD MODEL
	//Make Path relevant
	char path[] = "D:/Graphics Libraries/Resources/Cube.obj";
	//char path[] = "D:/Graphics Libraries/Resources/Suzanne.obj";
	//Shader shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.fs");
	Shader shader("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.fs","Shaders/GeometryPassTriangles.gs");
	/*Shader shader("Shaders/SimpleVertexShader.vert",
		"Shaders/SimpleFragmentShader.fs",
		"Shaders/GeometryPassTriangles.gs",
		"Shaders/SimpleTesControl.tcs",
		"Shaders/SimpleTesEval.tes");*/
	
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
	 


	 //first shader
	 shader.Use();

	 glm::mat4 model;
	 unsigned int modelLoc = glGetUniformLocation(shader.ID, "Model");
	 glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	 glm::mat4 MVP = projection * view * model;
	 unsigned int MVPLoc = glGetUniformLocation(shader.ID, "MVP");
	 glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	 //// retrieve the matrix uniform locations

	 //unsigned int viewLoc = glGetUniformLocation(shader.ID, "View");
	 //unsigned int projLoc = glGetUniformLocation(shader.ID, "Projection");
	 //// pass them to the shaders (3 different ways)

	 //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	 //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	 float color[] = { 1.0,0.0,0.0,1.0 };
	 int vertexColorLocation = glGetUniformLocation(shader.ID, "InputCol");
	 glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
	 //shader.setFloat("test",0.5);
	 shader.SetFloat("TessLevelInner", 2.0);
	 shader.SetFloat("TessLevelOuter", 2.0);
	 shader.SetFloat("time", glfwGetTime());
	 glUseProgram(0);


	


	//otherShader.Use();
	//// create transformations
	//model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
	////// retrieve the matrix uniform locations
	//unsigned int model1Loc = glGetUniformLocation(otherShader.ID, "Model");
	////unsigned int view1Loc = glGetUniformLocation(otherShader.ID, "View");
	////unsigned int proj1Loc = glGetUniformLocation(otherShader.ID, "Projection");
	////// pass them to the shaders (3 different ways)
	//glUniformMatrix4fv(model1Loc, 1, GL_FALSE, glm::value_ptr(model));
	////glUniformMatrix4fv(view1Loc, 1, GL_FALSE, glm::value_ptr(view));
	////glUniformMatrix4fv(proj1Loc, 1, GL_FALSE, glm::value_ptr(projection));


	// float color1[] = { 0.0,1.0,0.0,1.0 };
	//vertexColorLocation = glGetUniformLocation(otherShader.ID, "InputCol");
	//glUniform4f(vertexColorLocation, color1[0], color1[1], color1[2], color1[3]);
	////shader.setFloat("test",0.5);
	
	glUseProgram(0);



	//General GL options
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	//glPatchParameteri(GL_PATCH_VERTICES, 3);



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


	Graph* graph = Graph::getInstance();
	std::string code;

	


	bool windowOpen = true;
	
	ImVec2 NodePos(100, 100); //position relevant to the nodeview Window

	ImVec2 NodeSize(200, 75);
	float NodeValue = 5.0f;
	int NodeID = 90;
	std::string NodeName = "Constant";

	//probably need singleton
	GUIManager* GUI = GUIManager::getInstance();

	GUI->SetupGUI(window);

	/*GUI->CreateNode(ImVec2(100,100),InputNode,"float");
	GUI->CreateNode(ImVec2(100,200),InputNode,"float");
	GUI->CreateNode(ImVec2(100,300), InputNode,"float");
	GUI->CreateNode(ImVec2(100,400), InputNode, "float");*/
	//GUI->CreateNode(ImVec2(250,200),FunctionNode,"Addition");



	graph->daShader = &shader;


	//relevant path but still don't like this. it assumes we are in main root of project ask morten
	graph->ReadNodeTypes("NodeSystem/NodeTypes");


	GUI->CreateNode(ImVec2(400, 300), OutputnodeT,"Fragment");
	//GUI->CreateNode(ImVec2(200, 300 ) , ShadernodeT,"Vertex");
	graph->root = graph->NodeList.at(0);

	



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
			graph->time = time ;
			//std::cout << graph->time << std::endl;


			//Render GUI
			GUI->RenderGUI();

			//remove the calls from the visual part probably since this is called everyframe
			graph->UpdateUniforms();

			// OpenGL Rendering in scene
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

			//SET UNIFORMS AGAIN
			
		
			glm::mat4 view;
			glm::mat4 projection;
		

			view = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f), //camera pos
				glm::vec3(0.0f, 0.0f, 0.0f), //camera target
				glm::vec3(0.0f, 1.0f, 0.0f)); //up

			projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

			unsigned int UBO;
			glGenBuffers(1, &UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			//the actual memory allocation here 
			glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW); // allocate 3xmat4 bytes of memory
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			//Bind the UBO in binding point 0 
			//we can set a subset of the buffer object if we want - here it is the whole thing
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));

			//upload data
			//Buffer View mat
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
			//Buffer Projection mat
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));

			shader.Use();
			
			glm::mat4 model = glm::translate(glm::vec3(4.5f, 0.0f, 0.0f));// * glm::rotate(glm::radians(45.0f),glm::vec3(1.0f, 0.0f, 0.0f));
			unsigned int modelLoc = glGetUniformLocation(shader.ID, "Model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glm::mat4 MVP = projection * view * model;
			unsigned int MVPLoc = glGetUniformLocation(shader.ID, "MVP");
			glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

			glUniform1i(glGetUniformLocation(shader.ID, "myTexture1"), 0);

			//// retrieve the matrix uniform locations

			//unsigned int viewLoc = glGetUniformLocation(shader.ID, "View");
			//unsigned int projLoc = glGetUniformLocation(shader.ID, "Projection");
			//// pass them to the shaders (3 different ways)

			//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


			float color[] = { 1.0,0.0,0.0,1.0 };
			int vertexColorLocation = glGetUniformLocation(shader.ID, "InputCol");
			glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
			//shader.setFloat("test",0.5);
			shader.SetFloat("TessLevelInner", 1.0);
			shader.SetFloat("TessLevelOuter", 1.0);
			shader.SetFloat("time", glfwGetTime());
			glUseProgram(0);

			ourModel.Draw(shader);
			
			//std::cout << GL_GEOMETRY_OUTPUT_TYPE << std::endl;
			
			//ourModel.Draw(otherShader);
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
