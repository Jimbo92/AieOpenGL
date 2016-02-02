#pragma once
#include <glm\glm.hpp>
#include <BaseApplication.h>
#include <glm/ext.hpp>
#include <Gizmos.h>
#include <list>

class Planet
{
public:
	Planet() {};
	Planet(glm::vec3 Location, glm::vec4 Color, float RotationRate, float Scale,
		glm::vec3 RotationAxis, glm::vec3 OrbitPoint, bool Orbits, bool HasMoons, int NumofMoons);
	Planet(glm::vec3 Location, glm::vec4 Color, float Scale);

	//Vars
	glm::vec3	v_Location		= glm::vec3(0);
	float		f_Scale			= 3.f;
	glm::vec4	m_Color			= glm::vec4(1, 1, 1, 1);
	float		f_RotationRate	= 1.5f;
	glm::vec3	v_RotationAxis	= glm::vec3(0, 1, 0);
	bool		b_Orbits		= false;
	glm::vec3	v_RotateAround	= glm::vec3(0);
	bool		b_hasMoons		= false;
	int			m_NumberofMoons	= 1;
	float		f_OrbitSpeed	= .1f;

private:
	glm::mat4 m_LocalMatrix = glm::mat4(1.f);
	std::list<Planet*> l_Moons;


public:

	void Update(float DeltaTime);

	void Draw();

	~Planet();
};

