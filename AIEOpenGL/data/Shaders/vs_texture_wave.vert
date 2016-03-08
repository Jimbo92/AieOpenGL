#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec2 TexCoord;
layout(location=3) in vec4 Tangent;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vPosition;

uniform mat4 ProjectionView;
uniform sampler2D diffuse;
uniform sampler2D normal;
uniform float time;

void main() 
{
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTexCoord = TexCoord;


	vec3 N = texture(normal, TexCoord * 0.1f).xyz;

	vec4 P = Position;
	//wave pos
	P.y += sin(time * 1.5f + (N.r * 35.f)) * 0.5f;
	//P.y += cos(time * 1.5f + (N.r * 35.f)) * 0.5f;

	vPosition = P;

	//small wave
	P.y += sin((time * 15.f + Position.x) * 0.15f) * 0.2f;
	P.y += cos((time * 15.f + Position.x) * 0.15f) * 0.2f;
	P.y += sin((time * 15.f + Position.z) * 0.15f) * 0.2f;
	P.y += cos((time * 15.f + Position.z) * 0.15f) * 0.2f;

	//big wave
	P.y += sin((time * 10.f + Position.x) * 0.03f) * 1.0f;
	P.y += cos((time * 10.f + Position.x) * 0.03f) * 1.0f;
	P.y += sin((time * 10.f + Position.z) * 0.03f) * 1.0f;
	P.y += cos((time * 10.f + Position.z) * 0.03f) * 1.0f;

	gl_Position = ProjectionView * P;
}
