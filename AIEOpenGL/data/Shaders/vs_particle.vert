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

	gl_Position = ProjectionView * Position;
}
