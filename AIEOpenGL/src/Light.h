#pragma once

#include <stb_image.h>
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Light
{
public:

	Light(glm::vec3 lightdir = glm::vec3(0, 1, 0), glm::vec3 lightpos = glm::vec3(0), glm::vec4 lightcolor = glm::vec4(1), float lightrange = 50);
	~Light();

	glm::vec3 m_lightDir = glm::vec3(0, 1, 0);

	glm::vec3 m_lightPos = glm::vec3(0);

	glm::vec4 m_lightColor = glm::vec4(1);

	float m_lightRange = 50.f;
};

