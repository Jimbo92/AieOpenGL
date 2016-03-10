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
out vec4 vWaterHeight;

uniform vec4 LocalLocation;
uniform mat4 ProjectionView;
uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D noisemap;
uniform float time;

void main() 
{
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTexCoord = TexCoord;

	vec2 NewCoords = TexCoord;
	NewCoords.x += sin(time * 0.1f + NewCoords.y) * 0.05f;
	NewCoords.y += (cos(time * 0.1f + NewCoords.x) * 0.05f) * -1;

	vec3 N = texture(normal, NewCoords).xyz;

	vec4 P = Position;
	//wave pos
	P.y += sin(time * 5.5f + (N.r * 85.f)) * 0.1f;
	P.y += cos(time * 5.5f + (N.r * 85.f)) * 0.1f;

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

	vWaterHeight = LocalLocation + P;

	gl_Position = ProjectionView * P;
}
