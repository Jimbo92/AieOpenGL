#version 410
layout(location=0) in vec4 Position; 
layout(location=1) in vec4 Colour; 
//layout(location=2) in vec2 TexCoord; 
out vec4 vColour; 
uniform mat4 ProjectionView; 
uniform float time; 
uniform float heightScale; 
void main()
{ 	
	vColour = Colour; 
	vec4 P = Position;
	gl_Position = ProjectionView * P; 
}
