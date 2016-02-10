#pragma once

#include <stb_image.h>
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Light
{
public:

	Light(glm::vec3 lightdir = glm::vec3(0, 1, 0));
	~Light();

	glm::vec3 m_lightDir = glm::vec3(0, 1, 0);
};

