#include "GeometryApp.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"

#include <fstream>

#include <stb_image.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 TexCoord;
};

GeometryApp::GeometryApp() : m_camera(nullptr)
{
}

GeometryApp::~GeometryApp()
{
	delete LucyModel;
	delete BunnyModel;
}

bool GeometryApp::startup()
{
	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));

	//===================//Load OBJ models//==============================//
	Shader* CoolShader = new Shader("./data/vshader.vert", "./data/fshader.frag");
	Shader* NormalShader = new Shader("./data/vs_standardmodel.vert", "./data/fs_standardmodel.frag");
	Texture* earthTexture = new Texture("./data/earth_diffuse.jpg");

	Shader* TextureShader = new Shader("./data/vs_texture.vert", "./data/fs_texture.frag", earthTexture);

	LucyModel = new Model("./data/Lucy.obj", CoolShader, glm::vec3(10,0,0), glm::vec3(0.1,0.1,0.1));
	BunnyModel = new Model("./data/Bunny.obj", TextureShader, glm::vec3(0, -10, 0));

	return true;
}



void GeometryApp::shutdown()
{
	// delete our camera and cleanup gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool GeometryApp::update(float deltaTime)
{
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;
	
	//update camera
	m_camera->update(deltaTime);

	// clear the gizmos out for this frame
	Gizmos::clear();


	//generate the grid
	generateGrid(64, 64);

	/*
	// ...for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	}
	*/


	return true;
}

void GeometryApp::generateGrid(unsigned int rows, unsigned int cols)
{
	m_rows = rows;
	m_cols = cols;

	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);

			vec3 color = glm::vec3(sin((glfwGetTime() + aoVertices[r * cols + c].position.x) * 1.5f) * 1.f);
			vec3 color2 = glm::vec3(sin((glfwGetTime() + aoVertices[r * cols + c].position.z) * 1.5f) * 1.f);

			aoVertices[r * cols + c].color = glm::vec4(color, 1) + glm::vec4(color2, 1);
		}
	}

	//defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = (r * cols) + (c + 1);
		}
	}

	//Generates GL Buffers
	//Generate them together
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	//Add the following to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//create and bind buffers to a vertex array object
	//glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//IBO 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(UINT32), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] auiIndices;
	delete[] aoVertices;
}

void GeometryApp::draw()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	LucyModel->Draw(m_camera);
	BunnyModel->Draw(m_camera);

	glBindVertexArray(m_VAO);
	unsigned int indexCount = (m_rows - 1) * (m_cols - 1) * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(m_camera->getProjectionView());
}



