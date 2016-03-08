
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
uniform vec3 lightposition;
uniform vec3 lightdirection;
uniform vec4 lightcolor;
uniform float time;
uniform vec3 camerapos;
uniform float alpha;
uniform float specpow;
uniform vec3 ambient = vec3(0.0f, 0.0f, 1.0f);
uniform float lightrange = 50.f;

vec2 nextTextCoord;
vec2 nextTextCoord2;

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

vec4 BoxBlur(sampler2D inTexture)
{
	vec2 texel = 1.0f / textureSize(inTexture, 0).xy;

	// 9-tap box kernel
	vec4 colour = texture(inTexture, vTexCoord);
	colour += texture(inTexture, vTexCoord + vec2(-texel.x, texel.y));
	colour += texture(inTexture, vTexCoord + vec2(-texel.x, 0));
	colour += texture(inTexture, vTexCoord + vec2(-texel.x, -texel.y));
	colour += texture(inTexture, vTexCoord + vec2(0, texel.y));
	colour += texture(inTexture, vTexCoord + vec2(0, -texel.y));
	colour += texture(inTexture, vTexCoord + vec2(texel.x, texel.y));
	colour += texture(inTexture, vTexCoord + vec2(texel.x, 0));
	colour += texture(inTexture, vTexCoord + vec2(texel.x, -texel.y));
	return colour / 9;
}

void main() 
{ 

	nextTextCoord = vTexCoord;
	//flip y axis on texture coords to correct rotation
	nextTextCoord.x += sin(time * 0.1f + nextTextCoord.y  * 0.05f);
	nextTextCoord.y += cos(time * 0.1f + nextTextCoord.x  * 0.05f) * -1;

	nextTextCoord2.x = vTexCoord.x + sin(time * 0.05f) * -1;
	nextTextCoord2.y = vTexCoord.y + cos(time * 0.05f);

	vec4 TextureColor = texture(diffuse, nextTextCoord);
	vec4 TextureColor2 = texture(diffuse, -nextTextCoord2);

	vec3 N = texture(normal, nextTextCoord).xyz * 2 - 1;

	mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal));

	vec3 E = normalize(camerapos - vPosition.xyz);
	vec3 R = reflect(-lightdirection, N.xyz);

	float s = max(0, dot(E, R));
	s = pow(s, specpow); //specpow

	float d = 0;
	d = max(0, dot(N, lightdirection));

	vec4 FinalColor;
	FinalColor = (TextureColor2 * TextureColor);
	FinalColor.rgb = FinalColor.rgb * d + FinalColor.rgb * s;
	FinalColor.a *= alpha;

	vec4 amb = vec4(ambient * TextureColor.xyz, 0);

	vec2 texel = 1.0f / textureSize(diffuse, 0).xy;

	FinalColor = FinalColor + amb;

	FinalColor += vPosition.y;

	FragColor = FinalColor;
}