#include "Planet.h"

Planet::Planet(glm::vec3 Location, glm::vec4 Color, float RotationRate, float Scale,
	glm::vec3 RotationAxis, glm::vec3 OrbitPoint, bool Orbits, bool HasMoons, int NumofMoons)
{
	v_Location = Location;
	m_Color = Color;
	f_RotationRate = RotationRate;
	v_RotationAxis = RotationAxis;
	f_Scale = Scale;
	v_RotateAround = OrbitPoint;
	b_Orbits = Orbits;
	m_NumberofMoons = NumofMoons;
	b_hasMoons = HasMoons;

	if (b_hasMoons)
	{
		for (int i = 0; i < NumofMoons; i++)
		{
			Planet* newMoon = new Planet(v_Location, glm::vec4(1, 1, 1, 1), .3f, 0.3f, glm::vec3(0, 1, 0), glm::vec3(0), true, false, 0);
			newMoon->f_OrbitSpeed = 1.f;
			l_Moons.push_front(newMoon);
		}
	}
}

Planet::Planet(glm::vec3 Location, glm::vec4 Color, float Scale)
{
	v_Location = Location;
	m_Color = Color;
	f_Scale = Scale;
}

void Planet::Update(float DeltaTime)
{
	if (b_hasMoons)
	{
		for each (Planet* var in l_Moons)
		{
			var->Update(DeltaTime);
		}
	}

	//local rotations
	m_LocalMatrix = glm::rotate(m_LocalMatrix, f_RotationRate * DeltaTime, v_RotationAxis);

	if (b_Orbits)
	{
		v_Location.x = cos(-f_OrbitSpeed * DeltaTime) * (v_Location.x - v_RotateAround.x) - sin(-f_OrbitSpeed * DeltaTime) * (v_Location.z - v_RotateAround.z) + v_RotateAround.x;
		v_Location.z = sin(-f_OrbitSpeed * DeltaTime) * (v_Location.x - v_RotateAround.x) + cos(-f_OrbitSpeed * DeltaTime) * (v_Location.z - v_RotateAround.z) + v_RotateAround.z;
	}
}

void Planet::Draw()
{
	Gizmos::addSphere(v_Location, f_Scale, 20, 20, m_Color, &m_LocalMatrix);

	if (b_hasMoons)
	{
		for each (Planet* var in l_Moons)
		{
			var->Draw();
		}
	}
}


Planet::~Planet()
{
	l_Moons.clear();
}
