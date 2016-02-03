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
	Planet(Planet* Parent, glm::vec3 Location, glm::vec4 Color, float RotationRate, float Scale);

	//Vars
	Planet*		m_Parent		= nullptr;
	glm::vec3	v_Location		= glm::vec3(0);
	float		f_Scale			= 3.f;
	glm::vec4	m_Color			= glm::vec4(1, 1, 1, 1);
	float		f_RotationRate	= 1.5f;
	glm::vec3	v_RotationAxis	= glm::vec3(0, 1, 0);
	bool		b_Orbits		= false;
	glm::vec3	v_RotateAround	= glm::vec3(0);
	float		f_OrbitSpeed	= .1f;
	float		f_localRotation = 0.f;
	float		f_Rotation		= 0.f;
	bool		b_hasRing		= false;
	float		f_RingOuterRadius = 0.f;
	float		f_RingInnerRadius = 0.f;
	glm::vec3	v_RingOffset	= glm::vec3(0);

private:
	glm::mat4 m_LocalMatrix = glm::mat4(1.f);
	glm::mat4 m_WorldMatrix = glm::mat4(1.f);


public:

	void Update(float DeltaTime);

	void Draw();

	void AddRing(float InnerRadius, float OutRadius, glm::vec3 Offset);

	~Planet();
};

