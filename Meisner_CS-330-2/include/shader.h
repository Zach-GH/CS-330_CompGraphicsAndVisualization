#ifndef SHADER_H
#define SHADER_H
#include <deps.h>
void UDestroyMesh(GLMesh& mesh);

//===========================
// SKYBOX TEXTURE STARTS HERE
//===========================

const GLchar* skyboxVertexShaderSource = GLSL(330,
	layout(location = 0)
	in vec3 aPos;
out vec3 texCoords;
uniform mat4 projection;
uniform mat4 view;
void main()
{
	vec4 pos = projection * view * vec4(aPos, 1.0f);
	// Having z equal w will always result in a depth of 1.0f
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	// We want to flip the z axis due to the different coordinate systems (left hand vs right hand)
	texCoords = vec3(aPos.x, aPos.y, -aPos.z);
}

);

// 3D Fragment Shader Program Source Code
const GLchar* skyboxFragmentShaderSource = GLSL(330,
	in vec4 vertexColor; // Variable to hold incoming color data from vertex shader
out vec4 fragmentColor;
out vec4 FragColor;
in vec3 texCoords;
uniform samplerCube skybox;
void main()
{
	FragColor = texture(skybox, texCoords);
}
);

//=========================
// TEXTURE
//=========================

/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(330,
	layout(location = 0) in vec3 position;
layout(location = 2) in vec2 textureCoordinate;

out vec2 vertexTextureCoordinate;

//Global variables for the transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
	vertexTextureCoordinate = textureCoordinate;
}
);

/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(330,
	in vec2 vertexTextureCoordinate;

out vec4 fragmentColor;

uniform sampler2D uTexture;
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;

void main()
{
	fragmentColor = texture(uTexture, vertexTextureCoordinate); // Sends texture to the GPU for rendering
	if (multipleTextures)
	{
		vec4 extraTexture = texture(uTextureExtra, vertexTextureCoordinate);
		if (extraTexture.a != 0.0)
			fragmentColor = extraTexture;
	}
}
);

//=================
// LIGHT 
//=================

/* Cube Vertex Shader Source Code*/
const GLchar* lightVertexShaderSource = GLSL(330,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

	vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

	vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
	vertexTextureCoordinate = textureCoordinate;
}
);

/* Cube Fragment Shader Source Code*/
const GLchar* lightFragmentShaderSource = GLSL(330,

	in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU


// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 gGreenLightPosition;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
//uniform vec2 uvScale; // makes the textures duplicate
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;

vec4 phongLight()
{
	/*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

	//Calculate Ambient lighting*/
	float ambientStrength = 1.0f; // Set ambient or global lighting strength
	vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

	//Calculate Diffuse lighting*/
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection = normalize(gGreenLightPosition - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
	float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse = impact * lightColor; // Generate diffuse light color

	//Calculate Specular lighting*/
	float specularIntensity = 1.0f; // Set specular light strength
	float highlightSize = 16.0f; // Set specular highlight size
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
	//Calculate specular component
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * lightColor;

	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate); //* uvScale); // makes the textures duplicate

	// Calculate phong result
	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

	return vec4(phong, 1.0);
}

vec4 pointLight()
{
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - vertexFragmentPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 10.0f;

	// diffuse lighting
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 10.0f;
	vec3 viewDirection = normalize(viewPosition - vertexFragmentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(uTexture, vertexTextureCoordinate) * (diffuse * inten + ambient) * specular * inten);
}

vec4 direcLight()
{
	vec3 lightVec = lightPos - vertexFragmentPos;
	// ambient lighting
	float ambient = 0.50f;

	// diffuse lighting
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(viewPosition - vertexFragmentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(uTexture, vertexTextureCoordinate) * (diffuse + ambient)); //+ texture(tex1, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.40f;
	float innerCone = 0.45f;

	// ambient lighting
	float ambient = 0.90f;

	// diffuse lighting
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(viewPosition - vertexFragmentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(uTexture, vertexTextureCoordinate) * (diffuse * inten + ambient)); //+ texture(tex1, texCoord).r * specular * inten) * lightColor;
}


void main()
{
	//fragmentColor = phongLight(); // Phong Light
	//fragmentColor = pointLight(); // Point Light
	//fragmentColor = direcLight(); // Direc Light
	fragmentColor = spotLight(); // Spot Light
	if (multipleTextures)
	{
		vec4 extraTexture = texture(uTextureExtra, vertexTextureCoordinate);
		if (extraTexture.a != 0.0)
			//fragmentColor = (extraTexture * phongLight()); // Phong Light
			//fragmentColor = (extraTexture * pointLight()); // Point Light
			fragmentColor = (extraTexture * direcLight()); // Direc Light
			//fragmentColor = (extraTexture * spotLight()); // Spot Light
	}
}
);

/* Cube Vertex Shader Source Code*/
const GLchar* invisibleVertexShaderSource = GLSL(330,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
layout(location = 1) in vec3 normal; // VAP position 1 for normals
layout(location = 2) in vec2 textureCoordinate;

out vec3 vertexNormal; // For outgoing normals to fragment shader
out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
out vec2 vertexTextureCoordinate;

//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

	vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

	vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
	vertexTextureCoordinate = textureCoordinate;
}
);

/* Cube Fragment Shader Source Code*/
const GLchar* invisibleFragmentShaderSource = GLSL(330,

	in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU


// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 gGreenLightPosition;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
//uniform vec2 uvScale; // makes the textures duplicate
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;

vec4 phongLight()
{
	/*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

	//Calculate Ambient lighting*/
	float ambientStrength = 1.0f; // Set ambient or global lighting strength
	vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

	//Calculate Diffuse lighting*/
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection = normalize(gGreenLightPosition - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
	float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse = impact * lightColor; // Generate diffuse light color

	//Calculate Specular lighting*/
	float specularIntensity = 1.0f; // Set specular light strength
	float highlightSize = 16.0f; // Set specular highlight size
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
	//Calculate specular component
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * lightColor;

	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate); //* uvScale); // makes the textures duplicate

	// Calculate phong result
	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

	return vec4(phong, 1.0);
}

vec4 pointLight()
{
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - vertexFragmentPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 10.0f;

	// diffuse lighting
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 10.0f;
	vec3 viewDirection = normalize(viewPosition - vertexFragmentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(uTexture, vertexTextureCoordinate) * (diffuse * inten + ambient) * specular * inten);
}

vec4 direcLight()
{
	vec3 lightVec = lightPos - vertexFragmentPos;
	// ambient lighting
	float ambient = 0.50f;

	// diffuse lighting
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(viewPosition - vertexFragmentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(uTexture, vertexTextureCoordinate) * (diffuse + ambient)); //+ texture(tex1, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.40f;
	float innerCone = 0.45f;

	// ambient lighting
	float ambient = 0.90f;

	// diffuse lighting
	vec3 normal = normalize(vertexNormal);
	vec3 lightDirection = normalize(lightPos - vertexFragmentPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(viewPosition - vertexFragmentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(uTexture, vertexTextureCoordinate) * (diffuse * inten + ambient)); //+ texture(tex1, texCoord).r * specular * inten) * lightColor;
}


void main()
{
	//fragmentColor = phongLight(); // Phong Light
	//fragmentColor = pointLight(); // Point Light
	//fragmentColor = direcLight(); // Direc Light
	//fragmentColor = spotLight(); // Spot Light
	//if (multipleTextures)
	//{
		//vec4 extraTexture = texture(uTextureExtra, vertexTextureCoordinate);
		//if (extraTexture.a != 0.0)
			//fragmentColor = (extraTexture * phongLight()); // Phong Light
			//fragmentColor = (extraTexture * pointLight()); // Point Light
			//fragmentColor = (extraTexture * direcLight()); // Direc Light
			//fragmentColor = (extraTexture * spotLight()); // Spot Light
	//}
}
);

//=================
// GREEN LIGHT 
//=================

/* Cube Fragment Shader Source Code*/
const GLchar* greenLightFragmentShaderSource = GLSL(330,

	in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
//uniform vec2 uvScale; // makes the textures duplicate
uniform sampler2D uTextureExtra;
uniform bool multipleTextures;

vec4 phongLight()
{
	/*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

	//Calculate Ambient lighting*/
	float ambientStrength = 1.0f; // Set ambient or global lighting strength
	vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

	//Calculate Diffuse lighting*/
	vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
	vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
	float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
	vec3 diffuse = impact * lightColor; // Generate diffuse light color

	//Calculate Specular lighting*/
	float specularIntensity = 1.0f; // Set specular light strength
	float highlightSize = 16.0f; // Set specular highlight size
	vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
	vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
	//Calculate specular component
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * lightColor;

	// Texture holds the color to be used for all three components
	vec4 textureColor = texture(uTexture, vertexTextureCoordinate); //* uvScale); // makes the textures duplicate

	// Calculate phong result
	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

	return vec4(phong, 1.0);
}

void main()
{
	fragmentColor = phongLight();
	if (multipleTextures)
	{
		vec4 extraTexture = texture(uTextureExtra, vertexTextureCoordinate);
		if (extraTexture.a != 0.0)
			fragmentColor = (extraTexture * phongLight());
	}
}
);


//=================
// LAMP
//=================

/* Lamp Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(330,

	layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

		//Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);


/* Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(330,

	out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

void main()
{
	fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);

//=================
// Non textured
//=================

/* Vertex Shader Source Code*/
const GLchar* nonTexturedVertexShaderSource = GLSL(330,
	layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1

out vec4 vertexColor; // variable to transfer color data to the fragment shader

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
	vertexColor = color; // references incoming color data
}
);


/* Fragment Shader Source Code*/
const GLchar* nonTexturedFragmentShaderSource = GLSL(330,
	in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(vertexColor);
}
);

#endif