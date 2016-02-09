#pragma once

#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Shader
{
public:
	Shader();
	~Shader();

	std::string ReadShader(const char *filePath);

	void LoadShader();
};

