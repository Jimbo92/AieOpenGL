#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vPosition;

uniform mat4 ProjectionView;
uniform float time;

void main() 
{
	vPosition = Position;
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTexCoord = TexCoord;

	vec4 P = Position;
	P.x += sin((time * Position.y) * 0.05f) * 0.2f;
	P.x += cos((time * Position.y) * 0.05f) * 0.2f;
	P.z += sin((time * Position.y) * 0.05f) * 0.2f;
	P.z += cos((time * Position.y) * 0.05f) * 0.2f;

	gl_Position = ProjectionView * P;
}
