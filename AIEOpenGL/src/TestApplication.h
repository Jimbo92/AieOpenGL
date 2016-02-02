#pragma once

#include "BaseApplication.h"
#include "Planet.h"
#include <list>

// only needed for the camera picking
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

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
	Planet* Earth;

	// this is an example position for camera picking
	glm::vec3	m_pickPosition;
};