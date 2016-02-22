#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;
layout(location=2) in vec2 TexCoord;

out vec4 vColor;
out vec2 vTexCoord;
out vec4 vPosition;

uniform mat4 ProjectionView;
uniform vec3 lightposition;
uniform float time;
uniform sampler2D noisemap;

uniform float terrainScaleWeight = 25.f;


void main() 
{
	vPosition = Position;
	vTexCoord = TexCoord;
	vColor = Color;

	vec4 pos = Position;
	//pos.y += texture(noisemap, TexCoord).r * terrainScaleWeight;


	gl_Position = ProjectionView * pos;
}
