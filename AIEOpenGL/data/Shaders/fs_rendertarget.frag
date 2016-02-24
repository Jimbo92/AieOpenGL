#version 410

in vec2 vTexCoord; 
in vec4 vPosition;

out vec4 FragColor; 

uniform sampler2D rendertargetmap;
uniform float time;


void main() 
{ 
	vec4 TextureColor = texture(rendertargetmap, vTexCoord);
	FragColor = TextureColor;
}