#include <iostream>             // cout, cerr
#include <cstdlib>              // EXIT_FAILURE
#include <GL/glew.h>            // GLEW library
#include <GLFW/glfw3.h>         // GLFW library
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES // for M_PI
#include <math.h>
#include <camera.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include<filesystem>

namespace fs = std::filesystem;
using namespace std;

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

/* User-defined Function prototypes to:
* initialize program, set window size,
* redraw graphics on window when resized,
* render graphics on screen
*/
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UCreateSkybox();
unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
unsigned int cubemapTexture;
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

// FUN STUFF
namespace {
	const char* const WINDOW_TITLE = "Zachary Meisner"; // window title

	// window width and height
	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 800;

	// main window
	GLFWwindow* gWindow = nullptr;

	// camera
	Camera gCamera(glm::vec3(0.0f, 0.5f, -2.0f));
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	struct GLMesh
	{
		GLuint vao;
		GLuint vbo;
		GLuint lightVao;
		GLuint lightVbo;
		GLuint nonTexCubeVao;
		GLuint planeVao; // Handle for vertex array object
		GLuint planeVbo;
		GLuint cubeVao;
		GLuint cubeVbo;
		GLuint pyramidVao;
		GLuint pyramidVbo;
		GLuint lightPyramidVao;
		GLuint lightPyramidVbo;
		
		GLuint vbos[2]; // Handle for vertex buffer object
		GLuint cubeVbos[2];
		GLuint planeVbos[2];
		GLuint pyramidVbos[2];
		GLuint nonTexCubeVbos[2];
		
		GLuint nIndices; // number of vertices in mesh
		GLuint nVertices; // added for instancing
		GLuint nLightVertices;
		GLuint nCubeIndices;
		GLuint nPlaneIndices;
		GLuint nLightPyramidVertices;
		
		GLuint smileyVao;
		GLuint smileyVbos[2];
		GLuint nSmileyIndices;

		GLuint starVao;
		GLuint starVbos[2];
		GLuint nStarIndices;
	};

	// Triangle mesh data
	GLMesh gMesh;
	// shader program
	GLuint gCubeProgramId;
	GLuint gLightCubeProgramId;
	GLuint gStarProgramId;
	GLuint gPlaneProgramId;
	GLuint gPyramidProgramId;
	GLuint gSkyBoxProgramId;
	GLuint gLampProgramId;
	GLuint gLampPyramidProgramId;
	GLuint gGreenLampPyramidProgramId;
	GLuint gStarLampProgramId;
	GLuint gLightPyramidProgramId;

	// texture
	GLuint gCubeTextureId;
	GLuint gPyramidTextureId;
	GLuint gLightCubeTextureId;
	GLuint gStarTextureId;
	GLuint gNonTexCubeId;
	GLuint gLightPyramidTextureId;
	GLuint gPlaneTextureId;
	GLuint gCircleTextureId;
	
	glm::vec2 gUVScale(5.0f, 5.0f);
	GLint gTexWrapMode = GL_REPEAT;
	
	GLuint gTextureIdHat;
	bool gIsHatOn = true;
	
	GLuint gTextureIdFace;
	bool gIsFaceOn = true;
	
	// timing
	float gDeltaTime = 0.0f; // time between current frame and last frame
	float gLastFrame = 0.0f;

	glm::vec3 gCubePosition(-1.0f, 4.0f, -6.0f);
	glm::vec3 gCubeScale(1.0f);

	glm::vec3 gStarPosition(1.0f, 1.0f, 4.0f);
	glm::vec3 gStarScale(0.75f);
	
	// LIGHT  
	bool gIsLampOrbiting = true;
	glm::vec3 gLightCubePosition(1.0f, 1.0f, -9.0f);
	glm::vec3 gLightCubeScale(1.0f);
	
	glm::vec3 gLightPyramidPosition(1.0f, 2.5f, -9.0f);
	glm::vec3 gLightPyramidScale(0.5f);
	
	glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);
	glm::vec3 gLightColor(0.2f, 0.0f, 0.8f); // for everything else

	glm::vec3 gLightPosition(0.0f, 2.0f, 0.0f); // CUBE LAMP
	glm::vec3 gLightScale(0.3f);

	glm::vec3 gPyramidLampPosition(-2.0f, 2.5f, 1.0f); // PYRAMID LAMP
	glm::vec3 gPyramidLampScale(0.2f);
}

void URender();
void UCreateCube(GLMesh& mesh);
void UCreateColoredCircle(GLMesh& mesh);
void UCreatePlane(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
bool UCreateTexture(const char* filename, GLuint& textureId);
void UDestroyTexture(GLuint textureId);

bool perspective = true;
// projection matrix
glm::mat4 projection = glm::ortho(glm::radians(gCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);