#version 410

in vec2 vTexCoord; 
in vec4 vPosition;
in vec4 vNormal;

out vec4 FragColor; 

uniform sampler2D diffuse;
uniform sampler2D noisemap;
uniform float time;
uniform vec3 lightdirection;
uniform vec3 lightposition;
uniform vec4 lightcolor;
uniform vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
uniform float lightrange = 512.f;

vec2 nextTextCoord;

float alpha = 1.0f;

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

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec4 NoiseColor = texture(noisemap, nextTextCoord);

	//FragColor = NoiseColor.rrrr;
	//FragColor.a = 1;

	vec4 amb = vec4(ambient * TextureColor.xyz, 1);

	vec4 pointLight = calcpointlight(TextureColor, vNormal.rgb, vPosition.xyz, vec3(sin(time) * 100.f, cos(time) * 100.f, 0), vec4(1, 1, 1, 1));

	float d = 0;
	d = max(0, dot(vNormal.rgb, vec3(sin(time * 0.5f), cos(time * 0.5f), 0)));

	TextureColor.rgb *= d;

	FragColor = TextureColor + amb;
}