#pragma once
#include "gl_core_4_4.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <Shader.h>
#include <Camera.h>
#include <Texture.h>

struct Vertex
{
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec2 TexCoord;
};

class Terrain
{
public:
	//perlin data
	int octaves = 6;
	float m_amplitude = 1.0f;
	float m_persistence = 0.3f;
	float m_scaleoffset = 6.f;

	Terrain(Camera* camera = nullptr);
	~Terrain();

	Camera* m_CurrentCamera;

	Shader* m_TerrainShader;
	Texture* m_TerrainNoise;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_rows = 20;
	unsigned int m_cols = 20;

	float* perlin_data;

	void generateGrid();
	void generateGrid(unsigned int rows, unsigned int cols, bool withPerlin = true);

	void Draw();
	void DrawSingle();

	float m_TerrainIntensity = 1.f;

	bool m_RefreshTerrain = false;

	float m_scale = 0.5f;

	bool m_useAlpha = false;

	glm::vec3 m_Location = glm::vec3(0);
};

