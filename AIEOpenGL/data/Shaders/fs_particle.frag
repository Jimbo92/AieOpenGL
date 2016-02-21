#version 410

in vec4 vColor;
in vec2 vTexCoord; 
in vec4 vPosition;

out vec4 FragColor; 

uniform sampler2D diffuse;
uniform float time;

vec2 nextTextCoord;

float alpha = 1.0f;


void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord = vTexCoord;
	nextTextCoord.y = vTexCoord.y * -1;

	vec4 TextureColor = texture(diffuse, nextTextCoord);

	TextureColor *= vColor;

	FragColor = TextureColor;
}