#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;
layout(location=2) in vec2 TexCoord;

out vec4 vColor;
out vec2 vTexCoord;
out vec4 vPosition;

uniform mat4 ProjectionView;
uniform vec3 lightposition;
uniform float time;


void main() 
{
	vPosition = Position;
	vTexCoord = TexCoord;
	vColor = Color;

	vec4 P = Position;
	//P.y += sin((time + Position.x) * 10.f) * 0.5f;
	//P.y += cos((time + Position.x) * 10.f) * 0.5f;
	//P.y += sin((time + Position.z) * 10.f) * 0.5f;
	//P.y += cos((time + Position.z) * 10.f) * 0.5f;
	//P.x += sin((time + Position.z) * 5.f) * 0.1f;
	//P.x += cos((time + Position.z) * 5.f) * 0.1f;
	//P.z += sin((time + Position.x) * 5.f) * 0.1f;
	//P.z += cos((time + Position.x) * 5.f) * 0.1f;

	gl_Position = ProjectionView * P;
}
