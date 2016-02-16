#include "Light.h"



Light::Light(glm::vec3 lightdir, glm::vec3 lightpos, glm::vec4 lightcolor, float lightrange)
{
	m_lightDir = lightdir;
	m_lightPos = lightpos;
	m_lightColor = lightcolor;
	m_lightRange = lightrange;
}


Light::~Light()
{
}
