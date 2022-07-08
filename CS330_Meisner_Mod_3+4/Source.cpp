#include <iostream> // cout, cerr
#include <cstdlib> // EXIT_FAILURE
#include <GL/glew.h> // GLEW
#include <GLFW/glfw3.h> // GLFW


// GLM Math Header inclusions
#include <glm/glm.hpp> 
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <camera.h>
#include <shader.h>

using namespace std; // use standard namespace

// Shader program Macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif


// unnamed namespace
namespace {
	const char* const WINDOW_TITLE = "Zachary Meisner"; // window title

	// window width and height
	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 800;

	// main window
	GLFWwindow* gWindow = nullptr;
	

	// camera
	Camera gCamera(glm::vec3(-1.0f, 0.5f, 3.0f));
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;
}

/* User-defined Function prototypes to:
* initialize program, set window size,
* redraw graphics on window when resized,
* render graphics on screen
*/
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void URender();
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// game loop
int main(int argc, char* argv[]) {

	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;

	// Create mesh
	UCreatePyramid(gMesh); // Calls function to create VBO

	// create shader program
	if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
		return EXIT_FAILURE;

	// set background color to black (used by render loop below)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	

	// render loop
	while (!glfwWindowShouldClose(gWindow)) {

		// per frame timing
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;

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
	glfwSetCursorPosCallback(*window, UMousePositionCallback);
	glfwSetScrollCallback(*window, UMouseScrollCallback);
	glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

	// GLEW: initialize
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) 
	{
		std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
		return false;
	}

	// Display GPU OpenGL version
	cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

	return true;

}

// glfw: whenever window size changed this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

/*
* glfw: whenever the mouse moves, this callback is called
*/
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos) 
{

	if (gFirstMouse)
	{
		gLastX = xpos;
		gLastY = ypos;
		gFirstMouse = false;
	}

	float xoffset = xpos - gLastX;
	float yoffset = gLastY - ypos; // Reversed since y-coordinates go from bottom to top

	gLastX = xpos;
	gLastY = ypos;

	gCamera.ProcessMouseMovement(xoffset, yoffset);
}

/*
* glfw: whenever the mouse scroll wheel scrolls, this callback is called
*/
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
{
	gCamera.ProcessMouseScroll(yoffset);
}

/*
* glfw: whenever the mouse moves, this callback is called
*/
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	switch (button) {

	case GLFW_MOUSE_BUTTON_LEFT: 
	{
		if (action == GLFW_PRESS)
			cout << "Left mouse button pressed" << endl;
		else
			cout << "Left mouse button released" << endl;
	}
	break;

	case GLFW_MOUSE_BUTTON_MIDDLE: 
	{
		if (action == GLFW_PRESS)
			cout << "Middle mouse button pressed" << endl;
		else
			cout << "Middle Mouse button released" << endl;
	}
	break;

			
case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (action == GLFW_PRESS)
			cout << "Right mouse button pressed" << endl;
		else
			cout << "Right mouse button released" << endl;
	}
	break;

	default:
		cout << "Unhandled mouse button event" << endl;
		break;
	}
}



// process input: query GLFW whether relevant keys pressed/released this frame and act accordingly
void UProcessInput(GLFWwindow* window) 
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gCamera.ProcessKeyboard(FORWARD, gDeltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		gCamera.ProcessKeyboard(UP, gDeltaTime);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		gCamera.ProcessKeyboard(DOWN, gDeltaTime);
	
	/*************************************************************************************
	CHANGE THE COLOR OF THE OBJECT
	**************************************************************************************/

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
	{ 
		changeColor();
		cout << "Color changed" << endl;
	}
	
	/*******************************************************************=******************
	RESTORE THE COLOR OF THE OBJECT
	**************************************************************************************/

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
	{ 
		restoreColor();
		cout << "Color restored" << endl;
	}

	/*************************************************************************************
	INCREASE THE COLOR OF THE OBJECT
	**************************************************************************************/

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		increaseColor();
		cout << "Color increased" << endl;
	}

	/*************************************************************************************
	DECREASE THE COLOR OF THE OBJECT
	**************************************************************************************/

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		decreaseColor();
		cout << "Color decreased" << endl;
	}

	/*************************************************************************************
	PYRAMID
	**************************************************************************************/

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		UDestroyMesh(gMesh);
		UCreatePyramid(gMesh); // Calls function to create VBO
		cout << "Pyramid!" << endl;
	}

	/*************************************************************************************
	CUBE
	**************************************************************************************/

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		UDestroyMesh(gMesh);
		UCreateCube(gMesh); // Calls function to create VBO
		cout << "Cube!" << endl;
	}

}


void URender()
{

	// Configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
		
	// clear background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// 1. scales object by 2
	glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));

	// 2. Rotates the shape by the degrees relevant to the x, y, and z scale
	glm::mat4 rotation = glm::rotate(17.8f, glm::vec3(0.0f, 1.0f, 0.0f));

	// 3. Places object at the origin
	glm::mat4 translation = glm::translate(glm::vec3(-1.0f, 0.5f, 0.0f));

	// Model Matrix: Transformations are applied right-to-left order
	glm::mat4 model = translation * rotation * scale;

	// camera view transformation
	glm::mat4 view = gCamera.GetViewMatrix();
	
	// Projection Matrix: 45 degree field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	
	// set shader to be used
	glUseProgram(gProgramId);

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// activate VBOs contained within the mesh's VAO
	glBindVertexArray(gMesh.vao);
	

	// stencilFunc and Mask have to be before the draw function

	// draw triangle
	glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	
	glDisable(GL_DEPTH_TEST); // disables everything and keeps the outline
	// deactivate VAO
	glBindVertexArray(0);

	// swap buffers and poll IO events (keys pressed/released, mouse movement, etc.)
	glfwSwapBuffers(gWindow); // flips the back buffer with the front buffer every frame
}




