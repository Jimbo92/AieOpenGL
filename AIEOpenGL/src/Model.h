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
#include <FBXFile.h>

#include <BoundingObj.h>
#include <Gizmos.h>



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
	Model(Camera* camera, const char* Filepath, unsigned int modeltype = 0, bool isAnimated = false, glm::vec3 InitialLocation = glm::vec3(0), glm::vec3 InitialScale = glm::vec3(1));
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

	void Draw();
	void Update(float DeltaTime);

	Camera* m_camera;

	std::vector<Shader*> ModelShaders;

	std::map<std::string, int> m_TextureMap;

	std::vector<unsigned int> m_Textures;


	//FBX Stuff
	void CreateFBX(FBXFile* fbx);
	void CleanUpFBX(FBXFile* fbx);

	FBXFile* m_FBXModel = nullptr;

	std::string ModelPath;

	BoundingObj Bounds;

	glm::vec4 fplanes[6];

private:



};

