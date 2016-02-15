
#version 410

in vec4 vColor;
in vec2 vTexCoord; 
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

out vec4 FragColor; 

uniform sampler2D diffuse; 
uniform sampler2D normal;
uniform vec3 lightdirection;
uniform float time;
uniform vec3 camerapos;
uniform float alpha;
uniform float specpow;
uniform vec3 ambient = vec3(0.25f, 0.25f, 0.25f);

vec2 nextTextCoord;
vec2 nextTextCoord2;

void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord.x = vTexCoord.x + sin(time * 0.01f);
	nextTextCoord.y = vTexCoord.y + cos(time * 0.01f) * -1;

	nextTextCoord2.x = vTexCoord.x + sin(time * 0.01f) * -1;
	nextTextCoord2.y = vTexCoord.y + cos(time * 0.01f);

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec4 TextureColor2 = texture(diffuse, -nextTextCoord2);

	

	vec3 N = texture(normal, nextTextCoord).xyz * 2 - 1;

	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal));

	vec3 E = normalize(camerapos - vPosition.xyz);
	vec3 R = reflect(-lightdirection, N.xyz);

	float s = max(0, dot(E, R));
	s = pow(s, specpow); //specpow

	float d = 0;
	d = max(0, dot(normalize(TBN * N), lightdirection));



	vec4 FinalColor;
	FinalColor = (TextureColor2 * TextureColor);
	FinalColor.rgb = FinalColor.rgb * d + FinalColor.rgb * s;
	FinalColor.a *= alpha;

	vec4 amb = vec4(ambient * TextureColor.xyz, 0);

	FragColor = FinalColor;
}