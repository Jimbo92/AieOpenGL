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
uniform float time;

vec2 nextTextCoord;
vec2 nextTextCoord2;

float alpha = 0.5f;

void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord.x = vTexCoord.x + sin(time * 0.01f);
	nextTextCoord.y = vTexCoord.y + cos(time * 0.01f) * -1;

	nextTextCoord2.x = vTexCoord.x + sin(time * 0.01f) * -1;
	nextTextCoord2.y = vTexCoord.y + cos(time * 0.01f);

	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal * 2));

	vec3 N = texture(normal, nextTextCoord).xyz * 2 - 1;

	float d = 0;
	d = max(0, dot(normalize(TBN * N), lightdirection));

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec4 TextureColor2 = texture(diffuse, -nextTextCoord2);

	vec4 FinalColor;
	FinalColor = (TextureColor2 * TextureColor);
	FinalColor.rgb *= d;
	FinalColor.a *= alpha;

	FragColor = FinalColor;
}