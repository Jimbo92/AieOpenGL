#include "Planet.h"
#include <glm/gtc/matrix_transform.hpp>

Planet::Planet(Planet* Parent, glm::vec3 Location, glm::vec4 Color, float RotationRate, float Scale)
{
	m_Parent = Parent;
	v_Location = Location;
	m_Color = Color;
	f_RotationRate = RotationRate;
	f_Scale = Scale;
}

void Planet::Update(float DeltaTime)
{
	f_Rotation += f_RotationRate * DeltaTime;

	//local rotations
	m_LocalMatrix = glm::rotate(f_Rotation, glm::vec3(0, 1, 0));
	m_LocalMatrix = glm::translate(m_LocalMatrix, v_Location);
	m_LocalMatrix = glm::rotate(m_LocalMatrix, f_RotationRate * DeltaTime, glm::vec3(0, 1, 0));

	if (m_Parent == nullptr)
	{
		m_WorldMatrix = m_LocalMatrix;
	}
	else
	{
		m_WorldMatrix = m_Parent->m_WorldMatrix * m_LocalMatrix;
	}
}

void Planet::Draw()
{
	Gizmos::addSphere(m_WorldMatrix[3].xyz, f_Scale, 20, 20, m_Color, &m_WorldMatrix);
}


Planet::~Planet()
{
}
