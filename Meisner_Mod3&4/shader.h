#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream> // cout, cerr

// ---------------
// OPENGL TUTORIAL
// ---------------

class Shader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//2. compile shaders
		unsigned int vertex, fragment;

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete shaders as they are linked into program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	// use/activate shader
	void use() {
		glUseProgram(ID);
	}
	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	// utility function for checking shader compilation/linking errors
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR:SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;

			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

// ----------------------------------------
// COMP GRAPHICS AND VISUALIZATION TUTORIAL
// ----------------------------------------

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

namespace {
	// stores GL data relative to given mesh
	struct GLMesh
	{
		GLuint vao; // Handle for vertex array object
		GLuint vbos[2]; // Handle for vertex buffer object
		GLuint nIndices; // number of vertices in mesh
	};

	// Triangle mesh data
	GLMesh gMesh;
	// shader program
	GLuint gProgramId;

	// timing
	float gDeltaTime = 0.0f; // time between current frame and last frame
	float gLastFrame = 0.0f;
}

void UCreateCube(GLMesh& mesh);
void UCreatePyramid(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);


// 3D Vertex Shader Program Source Code
const GLchar* vertexShaderSource = GLSL(330,
	layout(location = 0) in vec3 position; // vertex data from vertex attrib pointer 0 
	layout(location = 1) in vec4 color; // Color data from Vertex Attrib Pointer 1

out vec4 vertexColor; // variable to transfer color data to fragment shader

// Global variables for the transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 3.0f); // transforms vertex data using matrix
	vertexColor = color; // references incoming color data
	
}
);



// 3D Fragment Shader Program Source Code
const GLchar* fragmentShaderSource = GLSL(330,
	in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(vertexColor);
}
);

void increaseColor() // INCREASE COLOR
{
	UDestroyShaderProgram(gProgramId);
	const GLchar* vertexShaderSource = GLSL(330,
		layout(location = 0) in vec3 position; // vertex data from vertex attrib pointer 0 
	layout(location = 1) in vec4 color; // Color data from Vertex Attrib Pointer 1

	out vec4 vertexColor; // variable to transfer color data to fragment shader

	// Global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 3.0f); // transforms vertex data using matrix
		//vertexColor = color; // references incoming color data
		vertexColor = color * 2;
	}
	);
	UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId);
}

void decreaseColor() // DEECREASE COLOR
{
	UDestroyShaderProgram(gProgramId);
	const GLchar* vertexShaderSource = GLSL(330,
		layout(location = 0) in vec3 position; // vertex data from vertex attrib pointer 0 
	layout(location = 1) in vec4 color; // Color data from Vertex Attrib Pointer 1

	out vec4 vertexColor; // variable to transfer color data to fragment shader

	// Global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 3.0f); // transforms vertex data using matrix
		//vertexColor = color; // references incoming color data
		vertexColor = color * 0.5;
	}
	);
	UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId);
}

void changeColor() // CHANGE COLOR
{

	UDestroyShaderProgram(gProgramId);

	const GLchar* fragmentShaderSource = GLSL(330,

	in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

	out vec4 fragmentColor;

	void main()
	{
		if (gl_FragCoord.x < 500)
			fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
		else
			fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
	);

	UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId);
}

void restoreColor() // RESTORE COLOR
{

	UDestroyShaderProgram(gProgramId);

	const GLchar* fragmentShaderSource = GLSL(330,
		in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

	out vec4 fragmentColor;

	void main()
	{
		fragmentColor = vec4(vertexColor);
	}
	);

	UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId);
}


// implements UCreateMesh function
void UCreatePyramid(GLMesh& mesh) {
	// Specifies normalized device coordinates (x, y, z) and color for square vertices 
	GLfloat pyramidVerts[] =
	{
		// PYRAMID
		 0.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, // 0
		-1.0f, -1.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f, // 1
		 1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f, 1.0f, // 2

		// right triangle
		 0.0f,  1.0f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f, // 3
		 1.0f, -1.0f,  1.0f,   0.5f, 0.5f, 1.0f, 1.0f, // 4 
		 1.0f, -1.0f, -1.0f,   0.2f, 0.2f, 0.5f, 1.0f, // 5 

		 // back triangle
		 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f, 1.0f, // 6  
		 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // 7 
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // 8

		// left triangle
		0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  // 9
	   -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // 10
	   -1.0f, -1.0f,  1.0f,  0.5f, 0.5f, 1.0f, 1.0f,  // 11

	};

	// PYRAMID
	GLushort pyramidIndices[] = {
		0, 1, 2,     // Triangle 1
		3, 4, 5,     // Triangle 2
		6, 7, 8,     // Triangle 3
		9, 10, 11,   // Triangle 4
		1, 2, 7,     // bottom 1
		7, 8, 11     // bottom 2

	};

	// creates vertex attribute pointer 
	const GLuint floatsPerVertex = 3; // number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a)

	glGenVertexArrays(1, &mesh.vao); // can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// create 2 buffers: first for vertex data second for indices
	glGenBuffers(2, mesh.vbos); // Creates 2 buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerts), pyramidVerts, GL_STATIC_DRAW); // sends vertex or coordinate data

	mesh.nIndices = sizeof(pyramidIndices) / sizeof(pyramidIndices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);


	// Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor); // number of floats before each

	// Instructs the GPU on how to handle vertex buffer object data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, i.e., x and y | data type | deactivate normalization | 0 strides | 0 offset
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0); // specifies initial position of coordinates in buffer

	// Parameters: attribPointerPosition 1 | floats per color is 4, i.e., rgba | data type | deactivate normalization | 6 strides until the next color, i.e., rgbaxy | 2 floats until the beginning of each color, i.e,. xy
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);
}

void UCreateCube(GLMesh& mesh) {

	GLfloat cubeVerts[] =
	{
		// Vertex Positions   // Colors
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, // Top Right Vertex 0
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, // Bottom Right Vertex 1
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f, // Bottom Left Vertex 2
			-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f, // Top Left Vertex 3

			 0.5f, -0.5f, -1.0f,  0.5f, 0.5f, 1.0f, 1.0f, // Top Right Vertex 0.2
			 0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.5f, 1.0f, // Bottom Right Vertex 1.2
			-0.5f,  0.5f, -1.0f,  0.2f, 0.2f, 0.5f, 1.0f, // Bottom Left Vertex 2.2
			-0.5f, -0.5f, -1.0f,  1.0f, 0.0f, 1.0f, 1.0f  // Top Left Vertex 3.2
	};

	GLushort cubeIndices[] = {
			0, 1, 3, // triangle 1
			1, 2, 3, // triangle 2
			0, 1, 4, // triangle 3
			0, 4, 5, // triangle 4
			0, 5, 6, // triangle 5
			0, 3, 6, // triangle 6
			4, 5, 6, // triangle 7
			4, 6, 7, // triangle 8
			2, 3, 6, // triangle 9
			2, 6, 7, // triangle 10
			1, 4, 7, // triangle 11
			1, 2, 7  // triangle 12

	};

	// creates vertex attribute pointer 
	const GLuint floatsPerVertex = 3; // number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a)

	glGenVertexArrays(1, &mesh.vao); // can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// create 2 buffers: first for vertex data second for indices
	glGenBuffers(2, mesh.vbos); // Creates 2 buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW); // sends vertex or coordinate data

	mesh.nIndices = sizeof(cubeIndices) / sizeof(cubeIndices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);


	// Strides between vertex coordinates is 6 (x, y, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor); // number of floats before each

	// Instructs the GPU on how to handle vertex buffer object data
	// Parameters: attribPointerPosition | coordinates per vertex is 2, i.e., x and y | data type | deactivate normalization | 0 strides | 0 offset
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0); // specifies initial position of coordinates in buffer

	// Parameters: attribPointerPosition 1 | floats per color is 4, i.e., rgba | data type | deactivate normalization | 6 strides until the next color, i.e., rgbaxy | 2 floats until the beginning of each color, i.e,. xy
	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);
}

void UDestroyMesh(GLMesh& mesh) {
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}

// implements UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId) {

	int success = 0;
	char infoLog[512];

	// create shader program object
	programId = glCreateProgram();

	// create vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// retrieve shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// compile vertex shader and print compilation errors (if any)
	glCompileShader(vertexShaderId); // compile vertex shader
	// check for compilation errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glCompileShader(fragmentShaderId); // compile fragment shader
	// check for shader compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	// attached compiled shaders to shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	glLinkProgram(programId); // link shader program
	// check for linking errors
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(programId); // uses shader program

	return true;
}

void UDestroyShaderProgram(GLuint programId) {
	glDeleteProgram(programId);
}



#endif