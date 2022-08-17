#include <controls.h>

// CREATE OBJECTS

void UCreateColoredCircle(GLMesh& mesh)
{
	// Position and Color data
	GLfloat verts[] = {

		 0.0f, 0.0f, 0.0f,   0.0f,  0.0f,  1.0f,    0.5f, 0.5f, // 0 front facing point
		//beginning of circle attempt
		// starting with one triangle at the top of the circle from the point
		// I want to place two points projecting from the point to start the first triangle
		// and then shift it over to make it look like a pizza
		// the more triangles the more spherical it becomes
		0.0f, 0.8f, 0.0f,   0.0f,  0.0f,  1.0f,    0.5f, 0.90f, // 1 sphere top
		0.5f, 0.6f, 0.0f,   0.0f,  0.0f,  1.0f,    0.80f, 0.80f, // 2 sphere right
		-0.5f, 0.6f, 0.0f,   0.0f,  0.0f,  1.0f,    0.20f, 0.80f, // 3 sphere left
		// Now lets try to do the bottom by inversing the Y points
		0.0f, -0.8f, 0.0f,   0.0f,  0.0f,  1.0f,    0.5f, 0.10f, // 4 sphere bottom
		0.5f, -0.6f, 0.0f,   0.0f,  0.0f,  1.0f,    0.90f, 0.30f, // 5 sphere right
		-0.5f, -0.6f, 0.0f,   0.0f,  0.0f,  1.0f,    0.10f, 0.30f, // 6 sphere left
		// Now lets try to do the right side
		0.8f, 0.0f, 0.0f,   1.0f,  1.0f,  1.0f,    0.95f, 0.5f, // 7 sphere right top
		// Now the left side
		-0.8f, 0.0f, 0.0f,   1.0f,  1.0f,  1.0f,    0.05f, 0.5f, // 8 sphere left top

		//=====================
		// Now the z depth
		//=====================
		
		 0.0f, 0.0f, -0.25f,   0.0f,  0.0f, -1.0f,    0.5f, 0.5f, // 9 front facing point
		//beginning of circle attempt
		// starting with one triangle at the top of the circle from the point
		// I want to place two points projecting from the point to start the first triangle
		// and then shift it over to make it look like a pizza
		// the more triangles the more spherical it becomes
		0.0f, 0.8f, -0.25f,   0.0f,  0.0f, -1.0f,    0.5f, 0.90f, // 10 sphere top
		0.5f, 0.6f, -0.25f,   0.0f,  0.0f, -1.0f,    0.80f, 0.80f, // 11 sphere right
		-0.5f, 0.6f, -0.25f,   0.0f,  0.0f, -1.0f,    0.20f, 0.80f, // 12 sphere left
		// Now lets try to do the bottom by inversing the Y points
		0.0f, -0.8f, -0.25f,   0.0f,  0.0f, -1.0f,    0.5f, 0.1f, // 13 sphere bottom
		0.5f, -0.6f, -0.25f,   0.0f,  0.0f, -1.0f,    0.80f, 0.30f, // 14 sphere right
		-0.5f, -0.6f, -0.25f,   0.0f,  0.0f, -1.0f,    0.20f, 0.30f, // 15 sphere left
		// Now lets try to do the right side
		0.8f, 0.0f, -0.25f,  0.0f,  0.0f, -1.0f,    0.95f, 0.5f, // 16 sphere right top
		// Now the left side
		-0.8f, 0.0f, -0.25f,   0.0f,  0.0f, -1.0f,    0.05f, 0.5f, // 17 sphere right top
	};

	// Index data to share position data
	GLushort indices[] = {

		// sphere starts here
		1, 0, 2, // top
		0, 3, 1, // top left
		4, 0, 5, // bottom
		0, 6, 4, // bottom left
		// to start the right side I connect 7 to the center
		// I then connect both of the previous triangles I made
		// to finish the pizza
		7, 0, 2,
		7, 0, 5,
		// now the left
		8, 0, 3,
		8, 0, 6,
		
		//================
		// now the z depth
		//================
		// sphere starts here
		10, 9, 11, // top
		9, 12, 10, // top left
		13, 9, 14, // bottom
		9, 15, 13, // bottom left
		// to start the right side I connect 7 to the center
		// I then connect both of the previous triangles I made
		// to finish the pizza
		16, 9, 11,
		16, 9, 14,
		// now the left
		17, 9, 12,
		17, 9, 15,

		// I have to connect the two sides now
		// 0 is 9 // this is the middle point
		// 1 is 10 // top point
		// 2 is 11 top right
		// 3 is 12 top left
		// 4 is 13 bottom point
		// 5 is 14 bottom right
		// 6 is 15 bottom left
		// 7 is 16 middle right point
		// 8 is 17 sphere left point
		// top right
		1, 10, 2, // inside
		10, 2, 11, // outside
		// top left
		1, 10, 3, // inside
		10, 3, 12, // outside
		// bottom right
		4, 13, 5, // inside
		13, 5, 14, // outside
		// bottom left
		4, 13, 6, // inside
		13, 6, 15, // outside
		// left side
		8, 17, 12,
		8, 12, 3,
		8, 17, 15,
		8, 15, 6,
		// right side
		7, 16, 11,
		7, 11, 2,
		7, 16, 14,
		7, 14, 5,

	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerUV = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.nonTexCubeVao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.nonTexCubeVao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.nonTexCubeVbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.nonTexCubeVbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	mesh.nCubeIndices = sizeof(indices) / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.nonTexCubeVbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void UCreatePlane(GLMesh& mesh)
{
	// Vertex data
	GLfloat verts[] =
	{
		// Positions          // Tex Coords       // normals 
		 0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,// Top Right Vertex 0
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,// Bottom Right Vertex 1
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,// Bottom Left Vertex 2
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,// Top Left Vertex 3
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerUV = 2;
	const GLuint floatsPerNormal = 3;

	glGenVertexArrays(1, &mesh.planeVao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.planeVao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(2, mesh.planeVbos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.planeVbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Data for the indices
	GLushort indices[] = { 0, 1, 3,  // Triangle 1
						   1, 2, 3   // Triangle 2
	};
	
	mesh.nPlaneIndices = sizeof(indices) / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.planeVbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void UCreateCube(GLMesh& mesh)
{
	// Vertex data
	GLfloat verts[] = {
		//Positions          //Texture Coordinates

		// front face of cube
		// bottom left
	   -0.5f, -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, 
		0.5f, -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f, 
		0.5f,  0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 
		
		// top right
		0.5f,  0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f, 
	   -0.5f,  0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f, 
	   -0.5f, -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, 

		// top point
		0.35f,  0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   -0.35f,  0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	
	   // left point
	   0.5f,  0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.5f,  -0.15f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   // right point
	   -0.5f,  0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   -0.5f,  -0.15f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   -1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	
	   // bottom point left 1
		0.5f,  -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.75f,  -0.75f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.5f, -0.15f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   // bottom point left 2
		0.0f,  -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.75f,  -0.75f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	
		// bottom point right 1
		 -0.5f,  -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		-0.75f,  -0.75f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		-0.5f, -0.15f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		// bottom point right 2
		 -0.0f,  -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 -0.75f,  -0.75f, 0.0f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 -0.5f, -0.5f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		

		// back side of star
		-0.5f, -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		0.5f, -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		0.5f,  0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

		// top right
		0.5f,  0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   -0.5f,  0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   -0.5f, -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	   // top point
	   0.35f,  0.45f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  -0.35f,  0.45f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.0f, 1.0f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	  // left point
	  0.5f,  0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.5f,  -0.15f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  1.0f, 0.45f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	  // right point
	  -0.5f,  0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  -0.5f,  -0.15f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  -1.0f, 0.45f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	  // bottom point left 1
	   0.5f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.5f, -0.15f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	  // bottom point left 2
	   0.0f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   0.5f, -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	   // bottom point right 1
		-0.5f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   -0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   -0.5f, -0.15f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	   // bottom point right 2
		-0.0f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		-0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

		
		// connecting both sides
		// bottom right 1
		-0.0f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		-0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
		-0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		
		 // bottom right 2
		-0.0f,  -0.5f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		-0.0f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
		-0.75f,  -0.75f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

		// bottom left 1
		0.0f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		0.75f,  -0.75f, 0.0f,   -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
		0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		
		// bottom left 2
		0.0f,  -0.5f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		0.0f,  -0.5f, 0.25f,   -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
		0.75f,  -0.75f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

		// top left 1
		0.35f,  0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.0f, 1.0f, 0.25f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		
		// top right 1
		-0.35f,  0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.0f, 1.0f, 0.25f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		// top left 2
		0.35f,  0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.0f, 1.0f, 0.25f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.35f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		
		// top right 2
		-0.35f,  0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		0.0f, 1.0f, 0.25f,     0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		-0.35f, 0.45f, 0.0f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		
		// left point
	   0.35f,  0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.35f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   // right point
	   -0.35f,  0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	  -0.35f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   -1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		
		// left point
		1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.35f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   1.0f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   // right point
		-1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	  -0.35f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   -1.0f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		// sides
		// left point
	    1.0f, 0.45f, 0.0f, 0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.5f,  -0.15f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.5f, -0.15f, 0.0f, 0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   // right point
		 -1.0f, 0.45f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	   -0.5f,  -0.15f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   -0.5f, -0.15f, 0.0f, 0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		  // left point
	    1.0f, 0.45f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	   0.5f,  -0.15f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		  1.0f, 0.45f, 0.25f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	   // right point
		 -1.0f, 0.45f, 0.0f,   0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	   -0.5f,  -0.15f, 0.25f,     0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	  -1.0f, 0.45f, 0.25f,     0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		// bottom sides
		
		// bottom point left 1
		   0.5f, -0.15f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.5f, -0.15f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,

	   // bottom point right 1
		-0.5f, -0.15f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	   -0.75f,  -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   -0.5f, -0.15f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
		
		// bottom point left 2
		   0.5f, -0.15f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.75f,  -0.75f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	  0.75f, -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,


	   // bottom point right 2
		-0.5f, -0.15f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	   -0.75f,  -0.75f, 0.0f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   -0.75f, -0.75f, 0.25f,   -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
	   
	};

	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerNormal = 3;
	const GLuint floatsPerUV = 2;

	mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

	glGenVertexArrays(1, &mesh.cubeVao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(mesh.cubeVao);

	// Create VBO
	glGenBuffers(1, &mesh.cubeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.cubeVbo); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);

	// Create Vertex Attribute Pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}

void UCreateSkybox()
{

	float skyboxVertices[] =
	{
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	unsigned int skyboxIndices[] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};

	// Create VAO, VBO, and EBO for the skybox
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();

	// All the faces of the cubemap (make sure they are in this exact order)
	std::string facesCubemap[6] =
	{
		parentDir + "\\Meisner_CS-330\\assets\\skybox\\sm64skybox_s_4.png",
		parentDir + "\\Meisner_CS-330\\assets\\skybox\\sm64skybox_s_2.png",
		parentDir + "\\Meisner_CS-330\\assets\\skybox\\sm64skybox_t.png",
		parentDir + "\\Meisner_CS-330\\assets\\skybox\\sm64skybox_b.png",
		parentDir + "\\Meisner_CS-330\\assets\\skybox\\sm64skybox_s_3.png",
		parentDir + "\\Meisner_CS-330\\assets\\skybox\\sm64skybox_f.png"
	};

	// Creates the cubemap texture object
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 3);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}
}

// OTHER

void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}

void UDestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.cubeVao);
	glDeleteBuffers(2, mesh.cubeVbos);

	glDeleteVertexArrays(1, &mesh.planeVao);
	glDeleteBuffers(2, mesh.planeVbos);
}

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

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, GLuint& textureId)
{
	int width, height, channels;
	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
	if (image)
	{
		flipImageVertically(image, width, height, channels);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			cout << "Not implemented to handle image with " << channels << " channels" << endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		return true;
	}

	// Error loading the image
	return false;
}

void UDestroyTexture(GLuint textureId)
{
	glGenTextures(1, &textureId);
}