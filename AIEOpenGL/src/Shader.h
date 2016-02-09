#pragma once
#include "BaseApplication.h"

#include "gl_core_4_4.h"

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <Texture.h>

#include "Camera.h"

class Shader
{
public:

	Shader() {}
	Shader(const char *VertexShaderPath, const char *FragmentShaderPath, Texture* TextureFile = nullptr);
	~Shader();

	void DrawShader(Camera* CurrentCamera, glm::vec3 location, glm::vec3 scale = glm::vec3(1));

	glm::mat4 LocalMatrix = glm::mat4(1);

	unsigned int m_programID;
	std::string m_VertShader;
	std::string m_FragShader;
	std::string LoadShader(const char *filePath);

	Texture* m_textureFile;
};

