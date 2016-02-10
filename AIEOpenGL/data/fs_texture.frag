#version 410

in vec4 vColor;
in vec2 vTexCoord; 

out vec4 FragColor; 

uniform sampler2D diffuse; 

vec2 nextTextCoord;

void main() 
{ 
	nextTextCoord = vTexCoord;
	nextTextCoord.y = vTexCoord.y * -1;

	FragColor = texture(diffuse, nextTextCoord);
}