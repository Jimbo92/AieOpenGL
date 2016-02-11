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
uniform sampler2D specmap;
uniform vec3 lightdirection;
uniform float time;
uniform vec3 camerapos;
uniform float alpha;
uniform float specpow;

vec2 nextTextCoord;

void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord = vTexCoord;
	nextTextCoord.y = vTexCoord.y * -1;

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec3 SM = texture(specmap, nextTextCoord).xyz;
	vec3 N = texture(normal, nextTextCoord).xyz * 2 - 1;

	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal * 2));

	vec3 E = normalize(camerapos - vPosition.xyz);
	vec3 R = reflect(-lightdirection, (SM.xyz + vNormal.xyz) * 0.5f);

	float s = max(0, dot(E, R));
	s = pow(s, specpow); //specpow

	float d = 0;
	d = max(0, dot(normalize(TBN * N), lightdirection));

	TextureColor.a *= alpha;
	TextureColor.rgb = (TextureColor.rgb * d) + (TextureColor.rgb * s);
	FragColor = TextureColor;
}