#pragma once
#include "gl_core_4_4.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <Shader.h>
#include <Camera.h>
#include <Texture.h>

struct Sphere 
{
	glm::vec3 m_center;
	float m_radius;
};

class BoundingObj
{
public:
	BoundingObj() {}
	BoundingObj(glm::vec3 center, float radius);
	~BoundingObj();

	Sphere m_Sphere;

	void fit(const std::vector<glm::vec3>& points);
};

