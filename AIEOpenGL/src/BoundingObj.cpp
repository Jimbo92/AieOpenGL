#include "BoundingObj.h"



BoundingObj::BoundingObj(glm::vec3 center, float radius)
{
	m_Sphere.m_center = center;
	m_Sphere.m_radius = radius;
}

void BoundingObj::fit(const std::vector<glm::vec3>& points)
{
	glm::vec3 min(1e37f), max(-1e37f);

	for (auto& p : points)
	{
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;

		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
	}

	m_Sphere.m_center = (min + max) * 0.5f;
	m_Sphere.m_radius = glm::distance(min, m_Sphere.m_center);
}

BoundingObj::~BoundingObj()
{
}
