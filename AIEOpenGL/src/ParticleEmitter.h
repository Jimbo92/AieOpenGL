#pragma once

#include <gl_core_4_4.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GLFW\glfw3.h>
#include <Shader.h>
#include <Camera.h>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float size;
	float lifetime;
	float lifespan;
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 textcoords;
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	glm::vec3 m_position;

	float m_emitTimer;
	float m_emitRate;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColor;
	glm::vec4 m_endColor;

	void initalise(unsigned int a_maxParticles, 
		unsigned int a_emitRate, 
		float a_lifetimeMin, float a_lifetimeMax, 
		float a_velocityMin, float a_velocityMax, 
		float a_startSize, float a_endSize, 
		const glm::vec4& a_startColor, const glm::vec4& a_endColor);

	void emit();

	void update(float DeltaTime, const glm::mat4& a_cameraTransform);

	void draw(Camera* currentCamera);

	Shader* m_ParticleShader;

protected:

	Particle* m_Particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

};

