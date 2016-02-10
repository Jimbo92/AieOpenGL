#version 410

in vec4 vColor;
in vec2 vTexCoord; 
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;

out vec4 FragColor; 

uniform sampler2D diffuse; 
uniform sampler2D normal;
uniform vec3 lightdirection;

vec2 nextTextCoord;

void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord = vTexCoord;
	nextTextCoord.y = vTexCoord.y * -1;

	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal * 2));

	vec3 N = texture(normal, nextTextCoord).xyz * 2 - 1;

	float d = 0;
	d = max(0, dot(normalize(TBN * N), lightdirection));

	FragColor = texture(diffuse, nextTextCoord) * d;
}