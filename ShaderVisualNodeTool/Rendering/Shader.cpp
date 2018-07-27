#include "Shader.h"
#include "../NodeSystem/Graph.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	//not sure if init of the array made everything to 0
	// - 1 indicates that there is no shader available
	for (int i = 0; i < 5; i++) {

		ShaderObjIDs[i] = -1;
	}
	
	// 1. retrieve the vertex/fragment source code from filePaths
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		//Debugger entry here
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	//convert back to char arrays
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	fragCode = fragmentCode;
	vertCode = vertexCode;
	
	
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	//save the shader objects
	ShaderObjIDs[0] = vertex;
	ShaderObjIDs[4] = fragment;

	// shader Program creation and shader attachments
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	
	//not sure if the detach is necessary now that we are not deleting
	glDetachShader(ID, vertex);
	glDetachShader(ID, fragment);
	
	//we skip the deletion of the shaders, instead we saved them in the array for future use
	//glDeleteShader(vertex);
	//glDeleteShader(fragment);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, fShaderStream, gShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			geometryCode = gShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		const char * gShaderCode = geometryCode.c_str();



		// 2. compile shaders
		unsigned int vertex, fragment, geometry;
		int success;
		char infoLog[512];
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");
		// fragment Shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");

		fragCode = fragmentCode;
		vertCode = vertexCode;
		geomCode = geometryCode;


		//save the shader objects
		ShaderObjIDs[0] = vertex;
		ShaderObjIDs[3] = geometry;
		ShaderObjIDs[4] = fragment;

		// shader Program
		ID = glCreateProgram();
		//Geometry out set
		//glProgramParameteri(ID,  , Graph::getInstance()->VertexTopologyOut);
		glProgramParameteri(ID, GL_GEOMETRY_VERTICES_OUT, Graph::getInstance()->GeomMaxVerticesOut);
		glProgramParameteri(ID, GL_GEOMETRY_INPUT_TYPE, Graph::getInstance()->VertexTopologyOut);
		glProgramParameteri(ID, GL_GEOMETRY_OUTPUT_TYPE, Graph::getInstance()->GeomTopologyOut);
		

		glAttachShader(ID, vertex);
		glAttachShader(ID, geometry);
		glAttachShader(ID, fragment);

		glLinkProgram(ID);

		//CheckCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
	}
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tcsPath, const char* tesPath) 
{

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::string tcsCode;
		std::string tesCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		std::ifstream tcsShaderFile;
		std::ifstream tesShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		tcsShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		tesShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::stringstream vShaderStream, fShaderStream, tcsShaderStream, tesShaderStream, gShaderStream;
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			gShaderFile.open(geometryPath);

			//std::stringstream vShaderStream, fShaderStream,tcsShaderStream,tesShaderStream;//gShaderStream
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			geometryCode = gShaderStream.str();

		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : VS+FS+GS" << e.what() << std::endl;
		}

		try {
			tcsShaderFile.open(tcsPath);
			tesShaderFile.open(tesPath);

			tcsShaderStream << tcsShaderFile.rdbuf();
			tesShaderStream << tesShaderFile.rdbuf();


			tcsShaderFile.close();
			tesShaderFile.close();

			tcsCode = tcsShaderStream.str();
			tesCode = tesShaderStream.str();



		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "eof: " << tcsShaderFile.eof() << " fail: " << tcsShaderFile.fail() << " bad: " << tcsShaderFile.bad() << '\n';
			std::cout << "eof: " << tesShaderFile.eof() << " fail: " << tesShaderFile.fail() << " bad: " << tesShaderFile.bad() << '\n';
			//std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : TCS + TES:"  << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		const char * gShaderCode = geometryCode.c_str();
		const char * tcsShaderCode = tcsCode.c_str();
		const char * tesShaderCode = tesCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment, geometry, tcs, tes;
		int success;
		char infoLog[512];
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");
		// tesselation control Shader
		tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcs, 1, &tcsShaderCode, NULL);
		glCompileShader(tcs);
		CheckCompileErrors(tcs, "TESSELATION CONTROL");
		// tesselation evaluation shader
		tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tes, 1, &tesShaderCode, NULL);
		glCompileShader(tes);
		CheckCompileErrors(tes, "TESSELATION EVALUATION");
		// geometry Shader
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");
		



		fragCode = fragmentCode;
		vertCode = vertexCode;
		geomCode = geometryCode;
		tessControlCode = tcsCode;
		tessEvalCode = tesCode;

		//save the shader objects
		ShaderObjIDs[0] = vertex;
		ShaderObjIDs[1] = tcs;
		ShaderObjIDs[2] = tes;
		ShaderObjIDs[3] = geometry;
		ShaderObjIDs[4] = fragment;

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, tcs);
		glAttachShader(ID, tes);
		glAttachShader(ID, geometry);
		glAttachShader(ID, fragment);

		glProgramParameteri(ID, GL_GEOMETRY_VERTICES_OUT, Graph::getInstance()->GeomMaxVerticesOut);
		glProgramParameteri(ID, GL_GEOMETRY_INPUT_TYPE, Graph::getInstance()->VertexTopologyOut);
		glProgramParameteri(ID, GL_GEOMETRY_OUTPUT_TYPE, Graph::getInstance()->GeomTopologyOut);
		//glPatchParameteri(GL_PATCH_VERTICES, Graph::getInstance()->PatchSize);



		glLinkProgram(ID);
		CheckCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
		glDeleteShader(tcs);
		glDeleteShader(tes);
	
}


// activate the shader
// ------------------------------------------------------------------------
void Shader::Use()
{
	glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::SetBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string &name, glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ChangeShaders(const char * vertexPath, const char * fragmentPath)
{
	//not sure if init of the array made everything to 0
	// - 1 indicates that there is no shader available


	// 1. retrieve the vertex/fragment source code from filePaths
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		//Debugger entry here
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	//convert back to char arrays
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//ShaderCodes[0] = vertexCode;
	//ShaderCodes[4] = fragmentCode;

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");


	// shader Program creation and shader attachments
	glDeleteProgram(ID);
	ID  = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary


	glDetachShader(ID, vertex);
	glDetachShader(ID, fragment);
	//vertex and fragment, nothing in between
	//ProgramShaders[0] = vertex;
	//ProgramShaders[4] = fragment;
	//we skip the deletion of the shaders, instead we save them in the array for future use
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


//the changed shader will receive vertex and 
void Shader::EditShader(std::string newVertex, std::string newGeometry, std::string newFragment)
{
	//this should be more generic, but for now all we do is just 

	std::string tempVert = newVertex;
	std::string tempGeom = newGeometry;
	std::string tempFrag = newFragment;


	const char* vShaderCode = tempVert.c_str();

	const char* fShaderCode = tempFrag.c_str();
	const char* gShaderCode = tempGeom.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment, geometry, tcs, tes;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");
	// geometry Shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	CheckCompileErrors(geometry, "GEOMETRY");




	// shader Program creation and shader attachments
	glDeleteProgram(ID);
	ID = glCreateProgram();


	//Geometry out set
	//std::cout << Graph::getInstance()->GeomTopologyOut << std::endl;
	glProgramParameteri(ID, GL_GEOMETRY_VERTICES_OUT, Graph::getInstance()->GeomMaxVerticesOut);
	glProgramParameteri(ID, GL_GEOMETRY_INPUT_TYPE, Graph::getInstance()->VertexTopologyOut);
	glProgramParameteri(ID, GL_GEOMETRY_OUTPUT_TYPE, Graph::getInstance()->GeomTopologyOut);
	//glPatchParameteri(GL_PATCH_VERTICES, Graph::getInstance()->PatchSize);

	 glAttachShader(ID, vertex); 
	
	
		glAttachShader(ID, geometry); 
	
	
		glAttachShader(ID, fragment); 
	
	/*glAttachShader(ID, tcs);
	glAttachShader(ID, tes);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);*/


	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary


	//glDetachShader(ID, vertex);
	//glDetachShader(ID, fragment);
	//vertex and fragment, nothing in between
	//ProgramShaders[0] = vertex;
	//ProgramShaders[4] = fragment;
	//we skip the deletion of the shaders, instead we save them in the array for future use
	//glDeleteShader(vertex);
	//glDeleteShader(fragment);


}


//the changed shader will receive vertex and 
void Shader::EditShader(std::string newVertex, std::string newTcs, std::string newTes, std::string newGeometry, std::string newFragment)
{
	//this should be more generic, but for now all we do is just 

	std::string tempVert = newVertex;
	std::string tempTcs = newTcs;
	std::string tempTes = newTes;
	std::string tempGeom = newGeometry;
	std::string tempFrag = newFragment;
	

	const char* vShaderCode = tempVert.c_str();
	const char * tcsShaderCode = tempTcs.c_str();
	const char * tesShaderCode = tempTes.c_str();
	const char* fShaderCode = tempFrag.c_str();
	const char* gShaderCode = tempGeom.c_str();
	
	// 2. compile shaders
	unsigned int vertex, fragment, geometry, tcs, tes;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// tesselation control Shader
	tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tcs, 1, &tcsShaderCode, NULL);
	glCompileShader(tcs);
	CheckCompileErrors(tcs, "TESSELATION CONTROL");
	// tesselation evaluation shader
	tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tes, 1, &tesShaderCode, NULL);
	glCompileShader(tes);
	CheckCompileErrors(tes, "TESSELATION EVALUATION");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");
	// geometry Shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	CheckCompileErrors(geometry, "GEOMETRY");


	

	// shader Program creation and shader attachments
	glDeleteProgram(ID);
	ID = glCreateProgram();


	//Geometry out set
	//std::cout << Graph::getInstance()->GeomTopologyOut << std::endl;
	glProgramParameteri(ID, GL_GEOMETRY_VERTICES_OUT, Graph::getInstance()->GeomMaxVerticesOut);
	glProgramParameteri(ID, GL_GEOMETRY_INPUT_TYPE, Graph::getInstance()->VertexTopologyOut);
	glProgramParameteri(ID, GL_GEOMETRY_OUTPUT_TYPE, Graph::getInstance()->GeomTopologyOut);
	//glPatchParameteri(GL_PATCH_VERTICES, Graph::getInstance()->PatchSize);

	if (CheckCompileErrors(vertex, "VERTEX")) { glAttachShader(ID, vertex); std::cout << "Attaching vertex" << std::endl; }
	if (CheckCompileErrors(vertex, "TESSELATION CONTROL")) { glAttachShader(ID, tcs); std::cout << "Attaching tcs" << std::endl;
	}
	if (CheckCompileErrors(vertex, "TESSELATION EVALUATION")) { glAttachShader(ID, tes); std::cout << "Attaching tes" << std::endl;
	}
	if (CheckCompileErrors(vertex, "FRAGMENT")) { glAttachShader(ID, geometry); std::cout << "Attaching fragment" << std::endl;
	}
	if (CheckCompileErrors(vertex, "GEOMETRY")) { glAttachShader(ID, fragment); std::cout << "Attaching geometry" << std::endl;
	}
	/*glAttachShader(ID, tcs);
	glAttachShader(ID, tes);
	glAttachShader(ID, geometry);
	glAttachShader(ID, fragment);*/


	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary


	//glDetachShader(ID, vertex);
	//glDetachShader(ID, fragment);
	//vertex and fragment, nothing in between
	//ProgramShaders[0] = vertex;
	//ProgramShaders[4] = fragment;
	//we skip the deletion of the shaders, instead we save them in the array for future use
	//glDeleteShader(vertex);
	//glDeleteShader(fragment);


}

void Shader::AddToProgram(const char * shaderPath, ShaderType type)
{

	
	std::string shaderCode;
	std::ifstream ShaderFile;
	
	// ensure ifstream objects can throw exceptions:
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		// open files
		ShaderFile.open(shaderPath);
		
		std::stringstream ShaderStream;
		// read file's buffer contents into streams
		ShaderStream << ShaderFile.rdbuf();
		
		// close file handlers
		ShaderFile.close();
		

		// convert stream into string
		shaderCode = ShaderStream.str();
		
	}
	catch (std::ifstream::failure e)
	{
		//Debugger entry here
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	//convert back to char arrays
	const char* ShaderCode = shaderCode.c_str();
	

	// 2. compile shaders
	unsigned int newShader;
	int success;
	//error vars, will prolly need for debugger class
	char infoLog[512];
	std::string stringType;


	int shaderID;
	
	// try putting this in a function, it is reused prominently
	switch (type) {
		
		case ShaderType::VERTEX:
			newShader = glCreateShader(GL_VERTEX_SHADER);
			shaderID = 0;
			stringType = "VERTEX";
			break;       // and exits the switch
		case ShaderType::TESSELATION_CONTROL:
			newShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			stringType = "TCS";
			shaderID = 1;
			break;
		case ShaderType::TESSELATION_EVALUATION:
			newShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			stringType = "TES";
			shaderID = 2;
			break;
		case ShaderType::GEOMETRY:
			newShader = glCreateShader(GL_GEOMETRY_SHADER);
			stringType = "GEOMETRY";
			shaderID = 3;
			break;
	     // and exits the switch
		case ShaderType::FRAGMENT:
			newShader = glCreateShader(GL_FRAGMENT_SHADER);
			stringType = "FRAGMENT";
			shaderID = 4;
			break;       // and exits the switch
		default:
			newShader = glCreateShader(GL_VERTEX_SHADER);
			break;
		
	
	}
	

	glShaderSource(newShader, 1, &ShaderCode, NULL);
	glCompileShader(newShader);

	//replace shader with the correct slot
	ShaderObjIDs[shaderID] = newShader;
	CheckCompileErrors(newShader, stringType);

	//remake the program by attaching all current shader objs and relinking

	for (int i = 0; i < 5; i++) {

		if (ShaderObjIDs[i] != -1) {
			glAttachShader(ID, ShaderObjIDs[i]);
		}
	}

	
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");

	
	
	
}

bool Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
		else {
			return true;
		}
	
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
		else {
			return true;
		}

	
	}
}
