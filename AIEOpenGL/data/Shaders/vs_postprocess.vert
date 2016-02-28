#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;

out vec2 vTexCoord;
out vec4 vPosition;

uniform mat4 ProjectionView;
uniform float time;


void main() 
{
	vPosition = Position;
	vTexCoord = TexCoord;

	gl_Position = Position;
}
