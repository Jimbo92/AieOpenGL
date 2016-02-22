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

	//===================//Load OBJ models//==============================//
	
	/*
	Texture* earthTexture = new Texture("./data/ruinedtank/ground_diff.jpg");
	Shader* CoolShader = new Shader("./data/Shaders/vs_texture_wave.vert", "./data/Shaders/fs_texture.frag", earthTexture);
	CoolShader->m_light = m_testLight;

	Shader* NormalShader = new Shader("./data/vs_standardmodel.vert", "./data/fs_standardmodel.frag");

	Texture* groundText = new Texture("./data/ruinedtank/ground_diff.jpg");
	Shader* groundShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture.frag", groundText);
	groundShader->m_specpow = 1.1f;
	groundShader->m_light = m_testLight;

	Texture* engineText = new Texture("./data/ruinedtank/left_engine_diff.jpg");
	Texture* engineText_N = new Texture("./data/ruinedtank/left_engine_norm.jpg");
	Shader* engineShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture_norm.frag", engineText, engineText_N);
	engineShader->m_light = m_testLight;

	Texture* turretText = new Texture("./data/ruinedtank/turret_diff.jpg");
	Texture* turretText_N = new Texture("./data/ruinedtank/turret_norm.jpg");
	Shader* turretShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture_norm.frag", turretText, turretText_N);
	turretShader->m_light = m_testLight;

	BunnyModel = new Model("./data/ruinedtank/tank");
	BunnyModel->ModelShaders.push_back(groundShader);
	BunnyModel->ModelShaders.push_back(engineShader);
	BunnyModel->ModelShaders.push_back(groundShader);
	BunnyModel->ModelShaders.push_back(groundShader);
	BunnyModel->ModelShaders.push_back(groundShader);
	BunnyModel->ModelShaders.push_back(groundShader);
	BunnyModel->ModelShaders.push_back(engineShader);
	BunnyModel->ModelShaders.push_back(engineShader);
	BunnyModel->ModelShaders.push_back(turretShader);
	BunnyModel->ModelShaders.push_back(turretShader);

	LucyModel = new Model("./data/Lucy", 0, glm::vec3(10,0,0), glm::vec3(0.1,0.1,0.1));


	Texture* SwordTexture = new Texture("./data/soulspear/soulspear_diffuse.tga");
	Texture* SwordTexture_N = new Texture("./data/soulspear/soulspear_normal.tga");
	Texture* SwordTexture_Spec = new Texture("./data/soulspear/soulspear_specular.tga");
	Shader* SwordShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture_norm_spec.frag", SwordTexture, SwordTexture_N, SwordTexture_Spec);
	SwordShader->m_light = m_testLight;
	SwordShader->m_specpow = 1.2f;
	SwordModel = new Model("./data/soulspear/soulspear", 0, vec3(0, 20, 0));
	SwordModel->ModelShaders.push_back(SwordShader);
	
	//water
	Texture* WaterTexture = new Texture("./data/water/water_diffuse.jpg");
	Texture* WaterTexture_Norm = new Texture("./data/water/water_normal.jpg");
	WaterShader = new Shader("./data/Shaders/vs_texture_wave.vert", "./data/Shaders/fs_texture_wave.frag", WaterTexture, WaterTexture_Norm);
	WaterShader->m_light = m_testLight;
	WaterShader->m_alpha = 0.5f;
	WaterShader->m_specpow = 10.0f;
	WaterModelTest = new Model("./data/water/waterplane", 0, vec3(5, 0.3f, -3));
	WaterModelTest->ModelShaders.push_back(WaterShader);

	//grass
	Texture* GrassTexture = new Texture("./data/grass/grass_diff.tga");
	Texture* GrassTexture_N = new Texture("./data/grass/grass_norm.tga");
	Texture* GrassTexture_Spec = new Texture("./data/grass/grass_spec.tga");
	Shader* GrassShader = new Shader("./data/Shaders/vs_grass.vert", "./data/Shaders/fs_texture_norm_spec.frag", GrassTexture, GrassTexture_N, GrassTexture_Spec);
	GrassShader->m_light = m_testLight;
	GrassShader->m_specpow = 1.1f;
	mdl_GrassChunk1 = new Model("./data/grass/Grass_01", 0, vec3(-5.f, 6.6f, 18.f));
	mdl_GrassChunk1->ModelShaders.push_back(GrassShader);
	mdl_GrassChunk2 = new Model("./data/grass/Grass_02", 0, vec3(-5.f, 6.5f, 18.f), glm::vec3(0.5f, 0.5f, 0.5f));
	mdl_GrassChunk2->ModelShaders.push_back(GrassShader);
	mdl_GrassChunk3 = new Model("./data/grass/Grass_03", 0, vec3(-5.f, 6.6f, 18.f));
	mdl_GrassChunk3->ModelShaders.push_back(GrassShader);

	//palmTree
	Texture* PalmTreeTrunkTexture = new Texture("./data/palmtree/ENV_MP_Iraq_palm_tree_01_D.tga");
	Texture* PalmTreeTrunkTexture_N = new Texture("./data/palmtree/ENV_MP_Iraq_palm_tree_01_N.tga");
	Shader* PalmTreeTrunkShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture_norm.frag", PalmTreeTrunkTexture, PalmTreeTrunkTexture_N);
	PalmTreeTrunkShader->m_light = m_testLight;

	Texture* PalmTreeBranchTexture = new Texture("./data/palmtree/ENV_MP_Iraq_PlantsSansTrans_D.tga");
	Texture* PalmTreeBranchTexture_Spec = new Texture("./data/palmtree/ENV_MP_Iraq_PlantsSansTrans_S.tga");
	Shader* PalmTreeBranchShader = new Shader("./data/Shaders/vs_branch.vert", "./data/Shaders/fs_texture_norm_spec.frag", PalmTreeBranchTexture, nullptr, PalmTreeBranchTexture_Spec);
	PalmTreeBranchShader->m_light = m_testLight;
	PalmTreeBranchShader->m_specpow = 1.f;

	mdl_PalmTree = new Model("./data/palmtree/Palm_Tree", 0, vec3(-5.f, 6.6f, 18.f), glm::vec3(0.1f, 0.1f, 0.1f));
	mdl_PalmTree->m_RotAxis = glm::vec3(1, 0, 0);
	mdl_PalmTree->m_RotAmount = glm::radians(-90.f);
	mdl_PalmTree->ModelShaders.push_back(PalmTreeBranchShader);
	mdl_PalmTree->ModelShaders.push_back(PalmTreeBranchShader);
	mdl_PalmTree->ModelShaders.push_back(PalmTreeBranchShader);
	mdl_PalmTree->ModelShaders.push_back(PalmTreeTrunkShader);
	*/

	//Texture* pyro_Texture = new Texture("./data/characters/Enemytank/EnemyTank_D.tga");
	//Texture* pyro_Texture_N = new Texture("./data/characters/Enemytank/EnemyTank_N.tga");
	//Texture* pyro_Texture_S = new Texture("./data/characters/Enemytank/EnemyTank_S.tga");
	//
	//Texture* pyroGun_Texture = new Texture("./data/characters/Pyro/Flamethrower_D.png");
	//Texture* pyroGun_Texture_N = new Texture("./data/characters/Pyro/Flamethrower_N.png");
	//Texture* pyroGun_Texture_S = new Texture("./data/characters/Pyro/Flamethrower_S.png");
	//
	//Shader* pyro_Shader = new Shader("./data/Shaders/vs_animation.vert", "./data/Shaders/fs_texture_norm_spec.frag", pyro_Texture, pyro_Texture_N, pyro_Texture_S);
	//pyro_Shader->m_light = m_testLight;
	//pyro_Shader->m_specpow = 0.5f;
	//
	//Shader* pyroGun_Shader = new Shader("./data/Shaders/vs_animation.vert", "./data/Shaders/fs_texture_norm_spec.frag", pyroGun_Texture, pyroGun_Texture_N, pyroGun_Texture_S);
	//pyroGun_Shader->m_light = m_testLight;
	//pyroGun_Shader->m_specpow = 1.5f;

	mdl_Sponza = new Model("./data/characters/Marksman/Marksman.fbx", 1, true, glm::vec3(0), glm::vec3(0.003f, 0.003f, 0.003f));
	mdl_Sponza->ModelShaders[0]->m_light = m_testLight;
	//mdl_Sponza->ModelShaders.push_back(pyro_Shader);
	//mdl_Sponza->ModelShaders.push_back(pyroGun_Shader);

	//Texture* SwordTexture = new Texture("./data/soulspear/soulspear_diffuse.tga");
	//Texture* SwordTexture_N = new Texture("./data/soulspear/soulspear_normal.tga");
	//Texture* SwordTexture_Spec = new Texture("./data/soulspear/soulspear_specular.tga");
	//Shader* SwordShader = new Shader("./data/Shaders/vs_texture.vert", "./data/Shaders/fs_texture_norm_spec.frag", SwordTexture, SwordTexture_N, SwordTexture_Spec);
	//SwordShader->m_light = m_testLight;
	//SwordShader->m_specpow = 5.5f;
	SwordModel = new Model("./data/soulspear/soulspear.fbx", 1, false, vec3(0, 5, 0));
	SwordModel->ModelShaders[0]->m_light = m_testLight;
	//SwordModel->ModelShaders.push_back(SwordShader);

	m_testEmitter = new ParticleEmitter();
	m_testEmitter->initalise(500, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 1, 1, 1), glm::vec4(1, 0, 0, 1));
	m_testEmitter->m_ParticleShader->m_light = m_testLight;

	tr_TerrainTest = new Terrain(m_camera);
	tr_TerrainTest->generateGrid(128, 128);

	return true;
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


	m_testLight->m_lightPos = glm::vec3(cos(glfwGetTime()) * 25.f, 0, sin(glfwGetTime()) * 25.f);

	mdl_Sponza->Update(deltaTime);

	SwordModel->m_RotAxis = glm::vec3(0, 1, 0);
	SwordModel->m_RotAmount = sin(glfwGetTime());
	//
	SwordModel->m_Location.y = cos(glfwGetTime()) + 5;

	m_testEmitter->update(deltaTime, m_camera->getTransform());

	//generate the grid
	//generateGrid(64, 64);

	
	// ...for now let's add a grid to the gizmos
	//for (int i = 0; i < 21; ++i) 
	//{
	//	Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
	//		i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	//
	//	Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
	//		i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	//}
	


	return true;
}



void GeometryApp::draw()
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tr_TerrainTest->Draw();

	//LucyModel->Draw(m_camera);
	//BunnyModel->Draw(m_camera);
	SwordModel->Draw(m_camera);

	//WaterModelTest->Draw(m_camera);

	//mdl_GrassChunk1->Draw(m_camera);
	//mdl_GrassChunk2->Draw(m_camera);
	//mdl_GrassChunk3->Draw(m_camera);

	//mdl_PalmTree->Draw(m_camera);

	m_testEmitter->draw(m_camera);

	mdl_Sponza->Draw(m_camera);



	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(m_camera->getProjectionView());
}



