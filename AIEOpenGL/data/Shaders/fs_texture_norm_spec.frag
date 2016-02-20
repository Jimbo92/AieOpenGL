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
uniform vec3 lightposition;
uniform vec3 lightdirection;
uniform vec4 lightcolor;
uniform float time;
uniform vec3 camerapos;
uniform float alpha;
uniform float specpow;
uniform vec3 ambient = vec3(0.25f, 0.25f, 0.25f);
uniform float lightrange = 50.f;

vec2 nextTextCoord;

vec4 calcpointlight(vec4 TextureColor, vec3 normal, vec3 worldPos, vec3 lightPos, vec4 color)
{
	vec3 delta = lightPos - worldPos;
	vec3 lightDir = normalize(delta);

	float distance = length(delta);

	float attenuation = (lightrange - distance) / lightrange;
	attenuation = clamp(attenuation, 0, 1);
	attenuation = pow(attenuation, 2);

	float lambert = clamp(dot(normal, lightDir), 0, 1);

	vec4 endValue;
	endValue = TextureColor * color * lambert * attenuation;
	return endValue;
}

void main() 
{ 
	//flip y axis on texture coords to correct rotation
	nextTextCoord = vTexCoord;
	//nextTextCoord.y = vTexCoord.y * -1;

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec3 SM = texture(specmap, nextTextCoord).xyz;
	vec3 N = texture(normal, nextTextCoord).xyz * 2 - 1;

	vec4 amb = vec4(ambient * TextureColor.xyz, 1);

	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal));

	vec3 E = normalize(camerapos - vPosition.xyz);
	vec3 R = reflect(-normalize(lightposition), TBN * SM.xyz);

	float s = pow(max(0.f, dot( E, R )), specpow);

	float d = 0;
	d = max(0, dot(normalize(TBN * N), normalize(lightposition)));

	vec4 pointLight = calcpointlight(TextureColor, N.xyz, vPosition.xyz, lightposition, lightcolor);

	TextureColor.a *= alpha;

	vec3 Specular = TextureColor.xyz * s;

	vec4 LightDirectional = TextureColor * d;

	FragColor = vec4(amb.xyz + TextureColor.xyz + Specular, 1);
}