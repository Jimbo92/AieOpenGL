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

#include "Camera.h"

class Shader
{
public:

	Shader() {}
	Shader(const char *VertexShaderPath, const char *FragmentShaderPath);
	~Shader();

	void DrawShader(Camera* CurrentCamera, glm::vec3 location);

	unsigned int m_programID;
	std::string m_VertShader;
	std::string m_FragShader;
	std::string LoadShader(const char *filePath);


};

