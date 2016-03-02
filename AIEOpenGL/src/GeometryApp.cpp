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



GeometryApp::GeometryApp() : m_camera(nullptr)
{
}

GeometryApp::~GeometryApp()
{
	delete LucyModel;
	delete BunnyModel;
	delete mdl_Sponza;

	TwDeleteAllBars();
	TwTerminate();
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

	m_testLight = new Light(glm::vec3(0, 1, 0), glm::vec3(0,10,0), glm::vec4(1,1,1,1), 1.f);


	//=================================//Sphere Bounding Test//=============================//
	m_testSphere = BoundingObj(glm::vec3(0, 0, 0), 0.5f);



	//=================================//Render Target//====================================//

	//setup frambuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//render target
	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);

	//depth buffer
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer Error!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//===================//Load OBJ models//==============================//

	mdl_Sponza = new Model("./data/characters/Marksman/Marksman.fbx", 1, true, glm::vec3(0), glm::vec3(0.003f, 0.003f, 0.003f));
	mdl_Sponza->ModelShaders[0]->m_light = m_testLight;

	SwordModel = new Model("./data/soulspear/soulspear.fbx", 1, false, vec3(0, 5, 0));
	SwordModel->ModelShaders[0]->m_light = m_testLight;

	m_testEmitter = new ParticleEmitter();
	m_testEmitter->initalise(500, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 1, 1, 1), glm::vec4(1, 0, 0, 1));
	m_testEmitter->m_ParticleShader->m_light = m_testLight;

	tr_TerrainTest = new Terrain(m_camera);
	tr_TerrainTest->generateGrid(512, 512);
	tr_TerrainTest->m_TerrainShader->m_light = m_testLight;

	GeneratePostProcessQuad();


	//============================================//AntTweakBar//==========================================//
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	glfwSetMouseButtonCallback(m_window, OnMouseButton);
	glfwSetCursorPosCallback(m_window, OnMousePosition);
	glfwSetScrollCallback(m_window, OnMouseScroll);
	glfwSetKeyCallback(m_window, OnKey);
	glfwSetCharCallback(m_window, OnChar);
	glfwSetWindowSizeCallback(m_window, OnWindowResize);

	m_MainTweakBar = TwNewBar("Settings");
	TwAddVarRW(m_MainTweakBar, "Clear Color", TW_TYPE_COLOR4F, &m_ClearColor[0], "");
	TwAddVarRW(m_MainTweakBar, "Enable PostProcessing", TW_TYPE_BOOLCPP, &m_EnablePostProcess, "");

	//tweak bar camera settings
	TwAddVarRW(m_MainTweakBar, "Camera Speed", TW_TYPE_FLOAT, &m_camera->m_speed, "group='Camera Settings'");


	//Terrain Shader Settings
	TwAddVarRW(m_MainTweakBar, "Terrain Intensity", TW_TYPE_FLOAT, &tr_TerrainTest->m_TerrainIntensity, "group='Terrain Settings'");
	TwAddVarRW(m_MainTweakBar, "SizeCols", TW_TYPE_INT32, &tr_TerrainTest->m_cols, "group='Terrain Settings'");
	TwAddVarRW(m_MainTweakBar, "SizeRows", TW_TYPE_INT32, &tr_TerrainTest->m_rows, "group='Terrain Settings'");
	TwAddVarRW(m_MainTweakBar, "Scale", TW_TYPE_FLOAT, &tr_TerrainTest->m_scale, "group='Terrain Settings'");
	TwAddVarRW(m_MainTweakBar, "Refresh", TW_TYPE_BOOLCPP, &tr_TerrainTest->m_RefreshTerrain, "group='Terrain Settings'");

	return true;
}

void GeometryApp::GeneratePostProcessQuad()
{
	m_postProcessShader = new Shader("./data/Shaders/vs_postprocess.vert", "./data/Shaders/fs_postprocess.frag");
	m_postProcessShader->m_rendertargetTexture = m_fboTexture;

	glm::vec2 TexelSize = 1.f / glm::vec2(1280, 720);
	glm::vec2 HalfTexelSize = 1.f / glm::vec2(1280, 720) * 0.5f;

	float vertexData[] =
	{
		-1, -1, 0, 1, HalfTexelSize.x, HalfTexelSize.y,
		1, 1, 0, 1, 1 - HalfTexelSize.x, 1 - HalfTexelSize.y,
		-1, 1, 0, 1, HalfTexelSize.x, 1 - HalfTexelSize.y,

		-1, -1, 0, 1, HalfTexelSize.x, HalfTexelSize.y,
		1, -1, 0, 1, 1 - HalfTexelSize.x, HalfTexelSize.y,
		1, 1, 0, 1, 1 - HalfTexelSize.x, 1 - HalfTexelSize.y,
	};

	glGenVertexArrays(1, &m_VAO_PPQ);
	glBindVertexArray(m_VAO_PPQ);

	glGenBuffers(1, &m_VBO_PPQ);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_PPQ);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GeometryApp::shutdown()
{
	// delete our camera and cleanup gizmos
	delete m_camera;
	Gizmos::destroy();

	//mdl_Sponza->CleanUpFBX(mdl_Sponza->m_FBXModel);

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

	//=================================//Sphere Bounding Test//=============================//
	vec4 plane(0, 1, 0, -1);

	m_testSphere.m_Sphere.m_center.y = cosf((float)glfwGetTime()) + 5;
	
	float d = glm::dot(glm::vec3(plane), m_testSphere.m_Sphere.m_center) + plane.w;

	Gizmos::addSphere(m_testSphere.m_Sphere.m_center, m_testSphere.m_Sphere.m_radius, 8, 8, Color_Blue);

	glm::vec4 m_planeColor = Color_Blue;
	if (d > m_testSphere.m_Sphere.m_radius)
	{
		std::cout << "front" << std::endl;
		m_planeColor = Color_Green;
	}
	else if (d < -m_testSphere.m_Sphere.m_radius)
	{
		std::cout << "back" << std::endl;
		m_planeColor = Color_Red;
	}

	Gizmos::addTri(glm::vec3(4, 1, 4), glm::vec3(-4, 1, -4), glm::vec3(-4, 1, 4), m_planeColor);	Gizmos::addTri(glm::vec3(4, 1, 4), glm::vec3(4, 1, -4), glm::vec3(-4, 1, -4), m_planeColor);
	//=====================================================================================//

	m_testLight->m_lightPos = glm::vec3(cos(glfwGetTime()) * 25.f, 0, sin(glfwGetTime()) * 25.f);

	mdl_Sponza->Update(deltaTime);

	SwordModel->m_RotAxis = glm::vec3(0, 1, 0);
	SwordModel->m_RotAmount = (float)sin(glfwGetTime());
	//
	SwordModel->m_Location.y = (float)cos(glfwGetTime()) + 5;
	
	m_testEmitter->update(deltaTime, m_camera->getTransform());

	//generate the grid
	//generateGrid(64, 64);

	
	// ...for now let's add a grid to the gizmos
	for (int i = 0; i < 21; ++i) 
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
			i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	}
	


	return true;
}



void GeometryApp::draw()
{
	if (m_EnablePostProcess)
	{
		//setup fbo render
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, 1280, 720);
	}

	// clear the screen for this frame
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//tr_TerrainTest->Draw();
	//
	//SwordModel->Draw(m_camera);
	//
	//m_testEmitter->draw(m_camera);
	//
	//mdl_Sponza->Draw(m_camera);

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(m_camera->getProjectionView());

	if (m_EnablePostProcess)
	{
		//return render screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1280, 720);

		// clear the screen for this frame
		glClear(GL_DEPTH_BUFFER_BIT);

		//draw postprocess shader
		m_postProcessShader->DrawShader(m_camera, glm::vec3(0));

		//draw post process quad
		glBindVertexArray(m_VAO_PPQ);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//draw tweak bars
	TwDraw();
}



