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
#include <Texture.h>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

#include <Light.h>

#include <ParticleEmitter.h>

#include <Terrain.h>

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



	Shader vShader;

	Model* LucyModel;
	Model* BunnyModel;
	Model* SwordModel;

	Light* m_testLight;

	Model* WaterModelTest;

	Model* mdl_GrassChunk1;
	Model* mdl_GrassChunk2;
	Model* mdl_GrassChunk3;

	Model* mdl_PalmTree;

	Shader* WaterShader;

	Model* mdl_Sponza;

	Terrain* tr_TerrainTest;

	ParticleEmitter* m_testEmitter;
};

