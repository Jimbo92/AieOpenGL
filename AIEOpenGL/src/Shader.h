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

#include <Light.h>

#include <FBXFile.h>

class Shader
{
public:

	Shader() {}
	Shader(const char *VertexShaderPath, const char *FragmentShaderPath, Texture* TextureFile = nullptr, Texture* NormalMap = nullptr, Texture* SpecMap = nullptr);
	~Shader();

	void DrawShader(Camera* CurrentCamera, glm::vec3 location, glm::vec3 scale = glm::vec3(1), glm::vec3 RotationAxis = glm::vec3(0), float RotationAmount = 0);

	glm::mat4 RotMatrix = glm::mat4(1);
	glm::mat4 ScaleMatrix = glm::mat4(1);

	glm::mat4 LocalMatrix = glm::mat4(1);

	unsigned int m_programID;
	std::string m_VertShader;
	std::string m_FragShader;
	std::string LoadShader(const char *filePath);

	Light* m_light = nullptr;

	Texture* m_textureFile;
	Texture* m_textureNormal;
	Texture* m_textureSpecmap;
	Texture* m_noisemap = nullptr;
	Texture* m_extramap1 = nullptr;

	unsigned int m_rendertargetTexture;

	std::vector<Texture*> m_Textures;

	float m_alpha = 1.f;
	float m_specpow = 0.f;

	float m_fogStart = 75.f;

	void UpdateBones(FBXSkeleton* skeleton);

	glm::vec4 m_ambientlight = glm::vec4(0.25f, 0.25f, 0.25f, 1);

	float m_foamIntensity;

	glm::vec3 m_WaterHeight;
};

