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
uniform vec3 ambient = vec3(0.25f, 0.25f, 0.25f);

vec2 nextTextCoord;

float alpha = 1.0f;

void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord = vTexCoord;
	nextTextCoord.y = vTexCoord.y * -1;

	float d = 0;
	d = max(0, dot(normalize(vNormal), lightdirection));

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	TextureColor.a *= alpha;
	TextureColor.rgb *= d;
	FragColor = TextureColor;
}