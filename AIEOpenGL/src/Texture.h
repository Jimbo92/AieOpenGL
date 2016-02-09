#pragma once
#include <stb_image.h>
#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Texture
{
public:
	Texture() {}
	Texture(const char* FilePath);

	unsigned int m_texture;

	~Texture();
};

