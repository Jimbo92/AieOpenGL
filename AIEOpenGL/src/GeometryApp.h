#pragma once

#include "BaseApplication.h"
#include "Planet.h"
#include <list>

#include <Model.h>

// only needed for the camera picking
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <Shader.h>
#include <tiny_obj_loader.h>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

class Camera;



class GeometryApp : public BaseApplication
{
public:
	GeometryApp();
	virtual ~GeometryApp();

	virtual bool startup();
	virtual void shutdown();

	virtual bool update(float deltaTime);
	virtual void draw();

	void generateGrid(unsigned int rows, unsigned int cols);


private:
	Camera*		m_camera;
	unsigned int m_texture;

	unsigned int m_programID;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_rows = 20;
	unsigned int m_cols = 20;

	Shader vShader;

	Model* LucyModel;
	Model* BunnyModel;


};

