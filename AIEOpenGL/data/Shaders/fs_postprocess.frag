#version 410

in vec2 vTexCoord; 
in vec4 vPosition;

out vec4 FragColor; 

uniform sampler2D rendertargetmap;
uniform float time;

vec4 BoxBlur()
{
	vec2 texel = 1.0f / textureSize(rendertargetmap, 0).xy;

	// 9-tap box kernel
	vec4 colour = texture(rendertargetmap, vTexCoord);
	colour += texture(rendertargetmap, vTexCoord + vec2(-texel.x, texel.y));
	colour += texture(rendertargetmap, vTexCoord + vec2(-texel.x, 0));
	colour += texture(rendertargetmap, vTexCoord + vec2(-texel.x, -texel.y));
	colour += texture(rendertargetmap, vTexCoord + vec2(0, texel.y));
	colour += texture(rendertargetmap, vTexCoord + vec2(0, -texel.y));
	colour += texture(rendertargetmap, vTexCoord + vec2(texel.x, texel.y));
	colour += texture(rendertargetmap, vTexCoord + vec2(texel.x, 0));
	colour += texture(rendertargetmap, vTexCoord + vec2(texel.x, -texel.y));
	return colour / 9;
}

vec4 Distort()
{
	vec2 mid = vec2(0.5f);

	float distanceFromCentre = distance(vTexCoord, mid);
	vec2 normalizedCoord = normalize(vTexCoord - mid);
	float bias = distanceFromCentre +
	sin(distanceFromCentre * 15) * 0.02f;

	vec2 newCoord = mid + bias * normalizedCoord;

	return texture(rendertargetmap, newCoord);
}

void main() 
{ 
	vec4 TextureColor = texture(rendertargetmap, vTexCoord);
	FragColor = BoxBlur() + Distort();
}