#include "TestApplication.h"
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Camera.h"
#include "Gizmos.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;


struct KeyFrame
{
	glm::vec3 position;
	glm::quat rotation;
};

KeyFrame m_hipFrames[2];
KeyFrame m_kneeFrames[2];
KeyFrame m_ankleFrames[2];

glm::mat4 m_hipBone;
glm::mat4 m_kneeBone;
glm::mat4 m_ankleBone;


TestApplication::TestApplication()
	: m_camera(nullptr) 
{

}

TestApplication::~TestApplication() 
{
	l_Planets.clear();

	delete Sun;
	delete Earth;
	delete Mars;
}

bool TestApplication::startup() 
{

	// create a basic window
	createWindow("AIE OpenGL Application", 1280, 720);

	// start the gizmo system that can draw basic shapes
	Gizmos::create();

	// create a camera
	m_camera = new Camera(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	m_camera->setLookAtFrom(vec3(10, 10, 10), vec3(0));


	m_positions[0] = glm::vec3(10, 5, 10);
	m_positions[1] = glm::vec3(-10, 0, -10);

	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));


	m_hipFrames[0].position = glm::vec3(0, 5, 0);
	m_hipFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_hipFrames[1].position = glm::vec3(0, 5, 0);
	m_hipFrames[1].rotation = glm::quat(glm::vec3(-1, 0, 0));

	m_kneeFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_kneeFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_ankleFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_ankleFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));



	//add planets

	//Sun
	Sun = new Planet(nullptr, glm::vec3(0), Color_Yellow, .5f, 1.f);
	l_Planets.push_back(Sun);

	//Mercury
	Mercury = new Planet(Sun, glm::vec3(-2.3f, 0, 0), Color_White, 4.f, .2f);
	l_Planets.push_back(Mercury);

	//Venus
	Venus = new Planet(Sun, glm::vec3(3.5f, 0, 0), Color_Red, 3.f, .3f);
	l_Planets.push_back(Venus);

	//earth
	Earth = new Planet(Sun, glm::vec3(-6,0,0), Color_Blue, 2.f, .4f);
	l_Planets.push_back(Earth);
	EarthMoon = new Planet(Earth, glm::vec3(1, 0, 0), Color_White, 4.5f, .15f);
	l_Planets.push_back(EarthMoon);

	//Mars
	Mars = new Planet(Sun, glm::vec3(8.f,0,0), Color_Orange, 1.5f, .3f);
	l_Planets.push_back(Mars);

	//Jupiter
	Jupiter = new Planet(Sun, glm::vec3(-10.5f, 0, 0), Color_Orange, 1.5f, 0.7f);
	l_Planets.push_back(Jupiter);

	//Saturn
	Saturn = new Planet(Sun, glm::vec3(14.5f, 0, 0), Color_Orange, 1.2f, 0.55f);
	Saturn->AddRing(.8f, 1.1f, glm::vec3(0));
	l_Planets.push_back(Saturn);

	//Uranus
	Uranus = new Planet(Sun, glm::vec3(-17, 0, 0), Color_Cyan, 1.f, 0.4f);
	l_Planets.push_back(Uranus);

	//Neptune
	Neptune = new Planet(Sun, glm::vec3(20, 0, 0), Color_Blue, 0.7f, 0.4f);
	l_Planets.push_back(Neptune);

	//Pluto
	Pluto = new Planet(Sun, glm::vec3(-24, 0, 0), Color_Blue, 0.5f, 0.2f);
	l_Planets.push_back(Pluto);

	m_pickPosition = glm::vec3(0);



	return true;
}

void TestApplication::shutdown() 
{

	//////////////////////////////////////////////////////////////////////////
	// YOUR SHUTDOWN CODE HERE
	//////////////////////////////////////////////////////////////////////////

	// delete our camera and cleanup gizmos
	delete m_camera;
	Gizmos::destroy();

	// destroy our window properly
	destroyWindow();
}

bool TestApplication::update(float deltaTime) 
{
	
	// close the application if the window closes
	if (glfwWindowShouldClose(m_window) ||
		glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return false;

	// update the camera's movement
	m_camera->update(deltaTime);

	// clear the gizmos out for this frame
	Gizmos::clear();

	s_time = cos((float)glfwGetTime()) * 0.5f + 0.5f;

	m_currentPosition = (1.f - s_time) * m_positions[0] + s_time * m_positions[1];

	m_quatRotTest = glm::slerp(m_rotations[0], m_rotations[1], s_time);

	m_TransformMatrix = glm::translate(m_currentPosition) * glm::toMat4(m_quatRotTest);



	
	glm::vec3 p = (1.f - s_time) * m_hipFrames[0].position + s_time * m_hipFrames[1].position;

	//hip animation
	glm::quat r1 = glm::slerp(m_hipFrames[0].rotation, m_hipFrames[1].rotation, s_time);

	m_hipBone = glm::translate(p) * glm::toMat4(r1);

	//knee animation
	glm::quat r2 = glm::slerp(m_kneeFrames[0].rotation, m_kneeFrames[1].rotation, s_time);

	m_kneeBone = m_hipBone * glm::translate(p) * glm::toMat4(r2);

	//ankle animation
	glm::quat r3 = glm::slerp(m_ankleFrames[0].rotation, m_ankleFrames[1].rotation, s_time);

	m_ankleBone = m_kneeBone * glm::translate(p) * glm::toMat4(r3);



	//Update Planets
	for each (auto SolarBody in l_Planets)
	{
		SolarBody->Update(deltaTime);
	}

	// an example of mouse picking
	if (glfwGetMouseButton(m_window, 0) == GLFW_PRESS) 
	{
		double x = 0, y = 0;
		glfwGetCursorPos(m_window, &x, &y);

		// plane represents the ground, with a normal of (0,1,0) and a distance of 0 from (0,0,0)
		glm::vec4 plane(0, 1, 0, 0);
		m_pickPosition = m_camera->pickAgainstPlane((float)x, (float)y, plane);
	}
	Gizmos::addTransform(glm::translate(m_pickPosition));

	// ...for now let's add a grid to the gizmos
	//for (int i = 0; i < 21; ++i) 
	//{
	//	Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10),
	//		i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	//
	//	Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i),
	//		i == 10 ? vec4(1, 1, 1, 1) : vec4(0, 0, 0, 1));
	//}

	// return true, else the application closes
	return true;
}

void TestApplication::draw() 
{
	// clear the screen for this frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::addTransform(m_TransformMatrix);
	Gizmos::addAABBFilled(m_currentPosition, glm::vec3(.5f), Color_Red, &m_TransformMatrix);

	Gizmos::addAABBFilled(m_hipBone[3].xyz, glm::vec3(.5f), Color_Green, &m_hipBone);
	Gizmos::addAABBFilled(m_kneeBone[3].xyz, glm::vec3(.5f), Color_Green, &m_kneeBone);
	Gizmos::addAABBFilled(m_ankleBone[3].xyz, glm::vec3(.5f), Color_Green, &m_ankleBone);

	//Draw Planets
	for each (auto SolarBody in l_Planets)
	{
		SolarBody->Draw();
	}

	// display the 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// get a orthographic projection matrix and draw 2D gizmos
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	mat4 guiMatrix = glm::ortho<float>(0, 0, (float)width, (float)height);

	Gizmos::draw2D(m_camera->getProjectionView());
}