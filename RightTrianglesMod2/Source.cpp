#include <iostream> // cout, cerr
#include <cstdlib> // EXIT_FAILURE
#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW

using namespace std; // use standard namespace

// unnamed namespace
namespace {
	const char* const WINDOW_TITLE = "Zachary Meisner"; // window title

	// window width and height
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	// stores GL data relative to given mesh
	struct GLMesh {
		GLuint vao; // Handle for vertex array object
		GLuint vbos[2]; // Handle for vertex buffer object
		GLuint nIndices; // number of vertices in mesh
	};

	// main window
	GLFWwindow* gWindow = nullptr;
	// Triangle mesh data
	GLMesh gMesh;
	// shader program
	GLuint gProgramId;
}

/* User-defined Function prototypes to:
* initialize program, set window size,
* redraw graphics on window when resized,
* render graphics on screen
*/
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh &mesh);
void UDestroyMesh(GLMesh &mesh);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

// Vertex Shader Program Source Code
const char *vertexShaderSource = "#version 330 core\n" // 330 is the latest Mac supports
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 colorFromVBO;\n"
"out vec4 colorFromVS;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   colorFromVS = colorFromVBO;\n"
"}\n\0";

// Fragment Shader Program Source Code
const char* fragmentShaderSource = "#version 330 core\n" // 330 is the latest Mac supports
"in vec4 colorFromVS; \n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = colorFromVS;\n"
"}\n\0";


// game loop
int main(int argc, char* argv[]) {
	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;

	// Create mesh
	UCreateMesh(gMesh); // Calls function to create VBO

	// create shader program
	if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
		return EXIT_FAILURE;

	// set background color to black (used by render loop below)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// render loop
	while (!glfwWindowShouldClose(gWindow)) {

		// input
		UProcessInput(gWindow);

		// render this frame
		URender();

		glfwPollEvents();
	}

	// release mesh data
	UDestroyMesh(gMesh);

	// release shader program
	UDestroyShaderProgram(gProgramId);

	exit(EXIT_SUCCESS); // terminates program successfully
}

// Initialize GLFW, GLEW, and create window
bool UInitialize(int argc, char* argv[], GLFWwindow** window) {

	// GLFW: Initialize and configure (desired OpenGL version)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// GLFW: window creation
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (*window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	glfwSetFramebufferSizeCallback(*window, UResizeWindow);

	// GLEW: initialize
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	// Display GPU OpenGL version
	cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

	return true;

}

// process input: query GLFW whether relevant keys pressed/released this frame and act accordingly
void UProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever window size changed this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void URender() {

	// clear background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// set shader to be used
	glUseProgram(gProgramId);

	// activate VBOs contained within the mesh's VAO
	glBindVertexArray(gMesh.vao);
	
	// draw triangle
	glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	// deactivate VAO
	glBindVertexArray(0);

	// swap buffers and poll IO events (keys pressed/released, mouse movement, etc.)
	glfwSwapBuffers(gWindow);
}

// implements UCreateMesh function
void UCreateMesh(GLMesh &mesh) {
	// specifies normalized device coordinates for triangle vertices
	GLfloat verts[] =
	{

		// the two triangles will be drawn using indices
		// Left triangle indices: 0, 1, 2
		// Right triangle indices: 3, 2, 4

		// index 0
		-1.0f, 1.0f, 0.0f, // top left corner
		1.0f, 0.0f, 0.0f, 1.0f, // red

		// index 1
		-1.0f, 0.0f, 0.0f,       // inside middle left
		0.0f,  0.0f, 1.0f, 1.0f,  //blue

		// index 2
		-0.5f, 0.0f, 0.0f,        // two triangles touching
		0.0f, 1.0f, 0.0f, 1.0f,  // green

		// index 3
	   0.0f, 0.0f, 0.0f,       // outer middle point
		1.0f, 0.0f, 0.0f, 1.0f,  // red
		
		// index 4
	   0.0f, -1.0f, 0.0f,       // bottom middle
	    0.0f, 1.0f, 0.0f, 1.0f,  // green
	};


	glGenVertexArrays(1, &mesh.vao); // can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.vao);

	// create 2 buffers: first for vertex data second for indices
	glGenBuffers(2, mesh.vbos); // Creates 2 buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // sends vertex or coordinate data
	
	// Creates a buffer object for indices
	GLushort indices[] = { 0, 1, 2, 3, 2, 4 }; // using index 2 twice
	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// creates vertex attribute pointer
	const GLuint floatsPerVertex = 3; // number of coordinates per vertex
	const GLuint floatsPerColor = 4; // (r, g, b, a)

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
	glDeleteBuffers(1, mesh.vbos);
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


