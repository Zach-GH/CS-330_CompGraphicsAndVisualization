// GLM Math Header inclusions
#include <create.h>

 //====================================
 // MAIN
 //====================================

int main(int argc, char* argv[]) {

	if (!UInitialize(argc, argv, &gWindow))
		return EXIT_FAILURE;
	
	//========================
	// Star
	//========================
	UCreateCube(gMesh);
	if (!UCreateShaderProgram(lightVertexShaderSource, greenLightFragmentShaderSource, gStarProgramId))
		return EXIT_FAILURE;
	if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
		return EXIT_FAILURE;
	const char* cubeTexture = "assets/star.png";
	if (!UCreateTexture(cubeTexture, gStarTextureId))
	{
		cout << "Failed to load texture " << cubeTexture << endl;
		return EXIT_FAILURE;
	}
	cubeTexture = "assets/star_eyes.png";
	if (!UCreateTexture(cubeTexture, gTextureIdHat))
	{
		cout << "Failed to load texture " << cubeTexture << endl;
		return EXIT_FAILURE;
	}
	glUseProgram(gStarProgramId);
	glUniform1i(glGetUniformLocation(gStarProgramId, "uTexture"), 4);
	glUniform1i(glGetUniformLocation(gStarProgramId, "uTextureExtra"), 5);

	//========================
	// PLANE 
	//========================
	UCreatePlane(gMesh);
	if (!UCreateShaderProgram(lightVertexShaderSource, lightFragmentShaderSource, gPlaneProgramId))
		return EXIT_FAILURE;
	if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampPyramidProgramId))
		return EXIT_FAILURE;
	const char* planeTexture = "assets/grass.png";
	if (!UCreateTexture(planeTexture, gPlaneTextureId))
	{
		cout << "Failed to load texture " << planeTexture << endl;
		return EXIT_FAILURE;
	}
	glUseProgram(gPlaneProgramId);
	glUniform1i(glGetUniformLocation(gPlaneProgramId, "uTexture"), 1);

	//========================
	// Skybox
	//========================
	glUseProgram(gSkyBoxProgramId);
	UCreateSkybox();
	UCreateShaderProgram(skyboxVertexShaderSource, skyboxFragmentShaderSource, gSkyBoxProgramId);
	
	//========================
	// Circle
	//========================
	glUseProgram(gNonTexCubeId);
	UCreateColoredCircle(gMesh);
	if (!UCreateShaderProgram(lightVertexShaderSource, lightFragmentShaderSource, gNonTexCubeId))
		return EXIT_FAILURE;
	const char* circleTexture = "assets/wario_coin/coin_front.png";
	if (!UCreateTexture(circleTexture, gCircleTextureId))
	{
		cout << "Failed to load texture " << circleTexture << endl;
		return EXIT_FAILURE;
	}
	circleTexture = "assets/wario_coin/wario_coin.png";
	if (!UCreateTexture(circleTexture, gTextureIdFace))
	{
		cout << "Failed to load texture " << circleTexture << endl;
		return EXIT_FAILURE;
	}
	
	glUniform1i(glGetUniformLocation(gNonTexCubeId, "uTexture"), 9);
	glUniform1i(glGetUniformLocation(gNonTexCubeId, "uTextureExtra"), 10);

	projection = glm::perspective(glm::radians(gCamera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	perspective = true;
	
	//========================
	// GAME LOOP 
	//========================
	while (!glfwWindowShouldClose(gWindow)) {
		// per frame timing
		float currentFrame = glfwGetTime();
		gDeltaTime = currentFrame - gLastFrame;
		gLastFrame = currentFrame;
		// input
		UProcessInput(gWindow);
		// Render this frame
		URender(); // This breaks the skybox for some reason
	}

	//========================
	// RELEASE DATA 
	//========================
	
	// mesh
	UDestroyMesh(gMesh);

	// texture
	UDestroyTexture(gCubeTextureId);
	UDestroyTexture(gPlaneTextureId);
	// shader
	UDestroyShaderProgram(gCubeProgramId);
	UDestroyShaderProgram(gPlaneProgramId);
	UDestroyShaderProgram(gLampProgramId);
	UDestroyShaderProgram(gPyramidProgramId);
	UDestroyShaderProgram(gNonTexCubeId);

	
	// exit
	exit(EXIT_SUCCESS); // terminates program successfully
}


 //====================================
 // RENDER
 //====================================

void URender()
{
	
	const float angularVelocity = glm::radians(45.0f);
	if (gIsLampOrbiting)
	{
		float angle = angularVelocity * gDeltaTime;
		glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
		glm::vec4 newPosition = glm::rotate(angle, rotationAxis) * glm::vec4(gLightPosition, 1.0f);
		gLightPosition.x = newPosition.x;
		gLightPosition.y = newPosition.y;
		gLightPosition.z = newPosition.z;
		
		glm::vec3 pRotationAxis(0.0f, 1.0f, 0.0f);
		glm::vec4 newPyrLampPos = glm::rotate(angle, pRotationAxis) * glm::vec4(gPyramidLampPosition, 1.0f);
		gPyramidLampPosition.x = newPyrLampPos.x;
		gPyramidLampPosition.y = newPyrLampPos.y;
		gPyramidLampPosition.z = newPyrLampPos.z;
	}
	
	// Enable z-depth
	glEnable(GL_DEPTH_TEST);
	// Clear the frame and z buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//====================================
	// Star
	//====================================
	// Activate the VBOs contained within the mesh's VAO
	glBindVertexArray(gMesh.cubeVao); //added
	// Set the shader to be used
	glUseProgram(gStarProgramId);
	glm::mat4 starModel = glm::translate(gStarPosition) * glm::scale(gStarScale);
	// camera/view transformation
	glm::mat4 starView = gCamera.GetViewMatrix();
	// Creates a perspective projection
	glm::mat4 starProjection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	// Retrieves and passes transform matrices to the Shader program
	GLint starModelLoc = glGetUniformLocation(gStarProgramId, "model");
	GLint starViewLoc = glGetUniformLocation(gStarProgramId, "view");
	GLint starProjLoc = glGetUniformLocation(gStarProgramId, "projection");
	glUniformMatrix4fv(starModelLoc, 1, GL_FALSE, glm::value_ptr(starModel));
	glUniformMatrix4fv(starViewLoc, 1, GL_FALSE, glm::value_ptr(starView));
	glUniformMatrix4fv(starProjLoc, 1, GL_FALSE, glm::value_ptr(starProjection));
	// Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
	GLint starObjectColorLoc = glGetUniformLocation(gStarProgramId, "objectColor"); //added
	GLint starColorLoc = glGetUniformLocation(gStarProgramId, "lightColor"); //added
	GLint starPositionLoc = glGetUniformLocation(gStarProgramId, "lightPos"); //added
	GLint starViewPositionLoc = glGetUniformLocation(gStarProgramId, "viewPosition"); //added
	// Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
	glUniform3f(starObjectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	glUniform3f(starColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
	glUniform3f(starPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
	const glm::vec3 starCameraPosition = gCamera.Position;
	glUniform3f(starViewPositionLoc, starCameraPosition.x, starCameraPosition.y, starCameraPosition.z);
	GLint starUVScaleLoc = glGetUniformLocation(gStarProgramId, "uvScale");
	glUniform2fv(starUVScaleLoc, 1, glm::value_ptr(gUVScale));
	// Sends transform information to the Vertex shader
	GLuint transformLocation = glGetUniformLocation(gStarProgramId, "shaderTransform");
	GLuint multipleTexturesLoc = glGetUniformLocation(gStarProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, gIsHatOn);
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gStarTextureId);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, gTextureIdHat);
	glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

	//====================================
	// Star Lamp
	//====================================
	glUseProgram(gLampProgramId);
	//Transform the smaller cube used as a visual que for the light source
	starModel = glm::translate(gLightPosition) * glm::scale(gLightScale);
	// Reference matrix uniforms from the Lamp Shader program
	starModelLoc = glGetUniformLocation(gLampProgramId, "model");
	starViewLoc = glGetUniformLocation(gLampProgramId, "view");
	starProjLoc = glGetUniformLocation(gLampProgramId, "projection");
	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(starModelLoc, 1, GL_FALSE, glm::value_ptr(starModel));
	glUniformMatrix4fv(starViewLoc, 1, GL_FALSE, glm::value_ptr(starView));
	glUniformMatrix4fv(starProjLoc, 1, GL_FALSE, glm::value_ptr(starProjection));
	glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
	// Deactivate the Vertex Array Object and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	
	//====================================
	// PLANE
	//====================================
	glBindVertexArray(gMesh.planeVao);
	glUseProgram(gPlaneProgramId);
	glm::mat4 planeScale = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 planeRotation = glm::rotate(4.7f, glm::vec3(1.0, 0.0f, 0.0f));
	glm::mat4 planeTranslation = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));
	glm::mat4 planeModel = planeTranslation * planeRotation * planeScale;
	glm::mat4 planeView = gCamera.GetViewMatrix();
	glm::mat4 planeProjection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	GLint planeModelLoc = glGetUniformLocation(gPlaneProgramId, "model");
	GLint planeViewLoc = glGetUniformLocation(gPlaneProgramId, "view");
	GLint planeProjLoc = glGetUniformLocation(gPlaneProgramId, "projection");
	glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniformMatrix4fv(planeViewLoc, 1, GL_FALSE, glm::value_ptr(planeView));
	glUniformMatrix4fv(planeProjLoc, 1, GL_FALSE, glm::value_ptr(planeProjection));
	GLint planeObjectColorLoc = glGetUniformLocation(gPlaneProgramId, "objectColor");
	GLint planeLightColorLoc = glGetUniformLocation(gPlaneProgramId, "lightColor");
	GLint planeLightPositionLoc = glGetUniformLocation(gPlaneProgramId, "lightPos");
	GLint planeViewPositionLoc = glGetUniformLocation(gPlaneProgramId, "viewPosition");
	glUniform3f(planeObjectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	glUniform3f(planeLightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
	glUniform3f(planeLightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
	const glm::vec3 planeCameraPosition = gCamera.Position;
	glUniform3f(planeViewPositionLoc, planeCameraPosition.x, planeCameraPosition.y, planeCameraPosition.z);
	GLint planeUVScaleLoc = glGetUniformLocation(gPlaneProgramId, "uvScale");
	glUniform2fv(planeUVScaleLoc, 1, glm::value_ptr(gUVScale));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gPlaneTextureId);
	glDrawElements(GL_TRIANGLES, gMesh.nPlaneIndices, GL_UNSIGNED_SHORT, NULL); 
	
	//====================================
	// Plane Lamp
	//====================================
	glUseProgram(gLampPyramidProgramId);
	//Transform the smaller cube used as a visual que for the light source
	planeModel = glm::translate(gPyramidLampPosition) * glm::scale(gPyramidLampScale);
	// Reference matrix uniforms from the Lamp Shader program
	planeModelLoc = glGetUniformLocation(gLampPyramidProgramId, "model");
	planeViewLoc = glGetUniformLocation(gLampPyramidProgramId, "view");
	planeProjLoc = glGetUniformLocation(gLampPyramidProgramId, "projection");
	// Pass matrix data to the Lamp Shader program's matrix uniforms
	glUniformMatrix4fv(planeModelLoc, 1, GL_FALSE, glm::value_ptr(planeModel));
	glUniformMatrix4fv(planeViewLoc, 1, GL_FALSE, glm::value_ptr(planeView));
	glUniformMatrix4fv(planeProjLoc, 1, GL_FALSE, glm::value_ptr(planeProjection));
	glDrawArrays(GL_TRIANGLES, 0, gMesh.nPlaneIndices);
	// Deactivate the Vertex Array Object and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	
	//====================================
	// Skybox
	//====================================
	glUseProgram(gSkyBoxProgramId);
	// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
	glDepthFunc(GL_LEQUAL);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
	// The last row and column affect the translation of the skybox (which we don't want to affect)
	view = glm::mat4(glm::mat3(glm::lookAt(gCamera.Position, gCamera.Position + gCamera.Front, gCamera.Up)));
	projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(gSkyBoxProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(gSkyBoxProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Switch back to the normal depth function
	glDepthFunc(GL_LESS);

	//====================================
	// Circle
	//====================================
	glBindVertexArray(gMesh.nonTexCubeVao);
	glUseProgram(gNonTexCubeId);
	glm::mat4 ccScale = glm::scale(glm::vec3(0.75f, 0.75f, 0.75f));
	glm::mat4 ccRotation = glm::rotate(0.0f, glm::vec3(1.0, 1.0f, 1.0f));
	glm::mat4 ccTranslation = glm::translate(glm::vec3(-1.0f, 0.0f, 4.0f));
	glm::mat4 ccModel = ccTranslation * ccRotation * ccScale;
	glm::mat4 ccView = gCamera.GetViewMatrix();
	glm::mat4 ccProjection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	GLint modelLoc = glGetUniformLocation(gNonTexCubeId, "model");
	GLint viewLoc = glGetUniformLocation(gNonTexCubeId, "view");
	GLint projLoc = glGetUniformLocation(gNonTexCubeId, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ccModel));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(ccView));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ccProjection));
	
	GLint circleObjectColorLoc = glGetUniformLocation(gNonTexCubeId, "objectColor");
	GLint circleLightColorLoc = glGetUniformLocation(gNonTexCubeId, "lightColor");
	GLint circleLightPositionLoc = glGetUniformLocation(gNonTexCubeId, "lightPos");
	GLint circleViewPositionLoc = glGetUniformLocation(gNonTexCubeId, "viewPosition");
	glUniform3f(circleObjectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	glUniform3f(circleLightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
	glUniform3f(circleLightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
	const glm::vec3 circleCameraPosition = gCamera.Position;
	glUniform3f(circleViewPositionLoc, circleCameraPosition.x, circleCameraPosition.y, circleCameraPosition.z);
	GLint circleUVScaleLoc = glGetUniformLocation(gNonTexCubeId, "uvScale");
	glUniform2fv(circleUVScaleLoc, 1, glm::value_ptr(gUVScale));
	GLuint coinTransformLocation = glGetUniformLocation(gNonTexCubeId, "shaderTransform");
	GLuint coinMultipleTexturesLoc = glGetUniformLocation(gNonTexCubeId, "multipleTextures");
	glUniform1i(coinMultipleTexturesLoc, gIsFaceOn);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, gCircleTextureId);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, gTextureIdFace);
	glDrawElements(GL_TRIANGLES, gMesh.nCubeIndices, GL_UNSIGNED_SHORT, NULL); 
	glBindVertexArray(0);
	
	//====================================
	// End
	//====================================
	glfwPollEvents();
	glfwSwapBuffers(gWindow);
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
	* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
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