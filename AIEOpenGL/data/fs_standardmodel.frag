#version 410
in vec4 vColour;
out vec4 FragColor;
uniform float time;
void main() 
{ 
	FragColor = vColour; 
}