#include "Planet.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Planet::Planet(Planet* Parent, glm::vec3 Location, glm::vec4 Color, float OrbitRate, float Scale, float LocalRotation)
{
	m_Parent = Parent;
	v_Location = Location;
	m_Color = Color;
	f_RotationRate = OrbitRate;
	f_Scale = Scale;
	f_localRotation = LocalRotation;
}

void Planet::Update(float DeltaTime)
{
	f_Rotation += (f_RotationRate / 5) * DeltaTime;

	//local rotations
	m_LocalRotationMatrix = glm::rotate(m_LocalRotationMatrix, f_localRotation * DeltaTime, glm::vec3(0, 1, 0));

	m_LocalMatrix = glm::rotate(f_Rotation, glm::vec3(0, 1, 0));
	m_LocalMatrix = glm::translate(m_LocalMatrix, v_Location);
	

	if (m_Parent == nullptr)
	{
		m_WorldMatrix = m_LocalMatrix;
	}
	else
	{
		m_WorldMatrix = m_Parent->m_WorldMatrix * m_LocalMatrix;
	}
}

void Planet::AddRing(float InnerRadius, float OutRadius, glm::vec3 Offset)
{
	b_hasRing = true;

	f_RingOuterRadius = OutRadius;
	f_RingInnerRadius = InnerRadius;
	v_RingOffset = Offset;
}

void Planet::Draw(bool inWireFrame)
{
	Gizmos::addSphere(m_WorldMatrix[3].xyz, f_Scale, 20, 20, m_Color, &m_LocalRotationMatrix, inWireFrame);

	if (b_hasRing)
	{
		Gizmos::addRing(m_WorldMatrix[3].xyz, f_RingInnerRadius, f_RingOuterRadius, 20, Color_White);
	}

	//Draw Projection
	if (inWireFrame && !b_isMoon)
	{
		Gizmos::addRing(glm::vec3(0), v_Location.x, v_Location.x + 0.05f, 64, glm::vec4(0.25, 0.25, 0.25, 1));
	}
}

Planet::~Planet()
{

}
