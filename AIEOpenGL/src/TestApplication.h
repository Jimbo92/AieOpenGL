#pragma once

#include "BaseApplication.h"
#include "Planet.h"
#include <list>

// only needed for the camera picking
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

class Camera;

class TestApplication : public BaseApplication {
public:

	TestApplication();
	virtual ~TestApplication();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

private:

	Camera*		m_camera;

	std::list<Planet*> l_Planets;

	Planet* Sun;

	Planet* Mercury;

	Planet* Venus;

	Planet* Earth;
	Planet* EarthMoon;

	Planet* Mars;

	Planet* Jupiter;

	Planet* Saturn;

	Planet* Uranus;

	Planet* Neptune;

	Planet* Pluto;

	glm::vec3 m_currentPosition;

	float s_time = 0.f;

	glm::quat m_quatRotTest;

	glm::mat4 m_TransformMatrix = glm::mat4(1);

	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

	// this is an example position for camera picking
	glm::vec3	m_pickPosition;
};