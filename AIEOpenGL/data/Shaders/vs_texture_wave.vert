#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;

uniform mat4 ProjectionView;
uniform float time;

void main() 
{
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTexCoord = TexCoord;

	vec4 P = Position;
	P.y += sin((time + Position.x) * 0.5f) * 0.3f;
	P.y += cos((time + Position.x) * 0.5f) * 0.3f;
	P.y += sin((time + Position.z) * 0.5f) * 0.3f;
	P.y += cos((time + Position.z) * 0.5f) * 0.3f;

	gl_Position = ProjectionView * P;
}
