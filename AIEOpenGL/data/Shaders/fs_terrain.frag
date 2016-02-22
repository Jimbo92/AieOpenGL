#version 410

in vec4 vColor;
in vec2 vTexCoord; 
in vec4 vPosition;

out vec4 FragColor; 

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specmap;
uniform sampler2D noisemap;
uniform float time;

vec2 nextTextCoord;

float alpha = 1.0f;


void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord = vTexCoord;

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec4 NoiseColor = texture(noisemap, nextTextCoord);

	TextureColor *= vColor;

	FragColor = NoiseColor.rrrr;
	FragColor.a = 1;
}