#pragma once

#include "BaseApplication.h"
#include <list>

// only needed for the camera picking
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <Shader.h>
#include <tiny_obj_loader.h>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

#include "gl_core_4_4.h"

#include <GLFW/glfw3.h>

#include <Shader.h>

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

class Model
{
public:
	Model(const char* Filepath, glm::vec3 InitialLocation = glm::vec3(0), glm::vec3 InitialScale = glm::vec3(1));
	~Model();

	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	std::vector<OpenGLInfo> m_gl_info;

	glm::vec3 m_Location;

	glm::vec3 m_Scale;

	glm::vec3 m_RotAxis = glm::vec3(0);

	float m_RotAmount = 0;

	void Draw(Camera* camera);
	void Update(float DeltaTime);


	std::vector<Shader*> ModelShaders;

	std::map<std::string, int> m_TextureMap;

	std::vector<unsigned int> m_Textures;


private:



};

