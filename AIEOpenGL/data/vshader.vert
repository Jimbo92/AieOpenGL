#version 410
layout(location=0) in vec4 Position; 
layout(location=1) in vec4 Colour; 
out vec4 vColour; 
uniform mat4 ProjectionView; 
uniform float time; 
uniform float heightScale; 
void main()
{ 	
	vColour = Colour; 
	vec4 P = Position;
	P.y += sin((time + Position.z) * 0.5f) * heightScale;
	P.y += sin((time + Position.x) * 0.5f) * heightScale;
	gl_Position = ProjectionView * P; 
}
