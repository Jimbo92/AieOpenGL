#version 410

in vec2 vTexCoord; 
in vec4 vPosition;
in vec4 vNormal;
in vec4 vEyePos;

out vec4 FragColor; 

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D specmap;
uniform sampler2D noisemap;
uniform float time;
uniform vec3 lightdirection;
uniform vec3 lightposition;
uniform vec4 lightcolor;
uniform vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
uniform float lightrange = 512.f;
uniform vec3 WaterHeight;
uniform vec4 LocalLocation;
uniform float foamIntensity;

//fog stuff
int fogEquation = 0;
uniform float fogStart = 70.f;
float fogEnd = fogStart / 2;
float fogDensity = .04f;
vec4 fogColor = vec4(0.0f, 0.2f, 0.2f, 1.f);

vec2 nextTextCoord;

float alpha = 1.0f;

float setupFog(float FogCoord)
{
	float result = 0.0f;
	if (fogEquation == 0)
	{
	result = (fogEnd - FogCoord) / (fogEnd - fogStart);
	}
	else if (fogEquation == 1)
	{
	result = exp(-fogDensity * FogCoord);
	}
	else if (fogEquation == 2)
	{
	result = exp(-pow(fogDensity * FogCoord, 2.0));
	}

	result = 1.0 - clamp(result, 0.0, 1.0);
	return result;
}

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

	vec4 TextureColor = texture(diffuse, nextTextCoord * 4);
	vec4 TextureColor2 = texture(normal, nextTextCoord * 4);
	vec4 GritMask = texture(specmap, nextTextCoord);
	vec4 NoiseColor = texture(noisemap, nextTextCoord * 0.1f);

	//FragColor = NoiseColor.rrrr;
	//FragColor.a = 1;

	vec4 pointLight = calcpointlight(TextureColor, vNormal.rgb, vPosition.xyz, vec3(20, 20, 20), vec4(1, 1, 1, 1));

	float d = 0;
	d = max(0, dot(vNormal.rgb, lightdirection));

	//TextureColor.rgb *= d;
	
	vec4 invert = vec4(1-NoiseColor.r, 0, 0, 1);

	vec4 EndColor = vec4(NoiseColor.r * TextureColor2.rgb + invert.r * TextureColor.rgb, 1);

	vec4 amb = vec4(ambient * EndColor.rgb, 1);

	//fog
	float fogCoord = abs(vEyePos.y / vEyePos.w);

	float NoiseYPos;
	NoiseYPos += NoiseColor.r * (512.f * 0.07f) * foamIntensity;

	float NoiseYPosWet;
	NoiseYPosWet += NoiseColor.r * (512.f * 0.085f) * foamIntensity;

	float ShoreLine = clamp((WaterHeight.y - NoiseYPos) * 0.1f, 0.0, 0.7);

	float ShoreLineWet = clamp((WaterHeight.y - NoiseYPosWet) * 0.1f, 0.0, 0.7);

	vec4 ShoreSand = TextureColor * ShoreLine;

	vec4 ShoreSandWet = (EndColor - EndColor * 4.f) * ShoreLineWet;

	EndColor = EndColor + ShoreSandWet + ShoreSand;

	FragColor = mix(vec4(EndColor.rgb, 1) * d + amb, fogColor, setupFog(vPosition.y));
}