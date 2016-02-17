#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec4 BiTangent;
layout(location=4) in vec2 TexCoord;
layout(location=5) in vec4 BoneWeight;
layout(location=6) in vec4 BoneIndices;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;
out vec4 vPosition;

uniform mat4 ProjectionView;
uniform vec3 lightposition;

const int MAX_BONES = 128;
uniform mat4 bones[MAX_BONES];

void main() 
{
	vPosition = Position;
	vNormal = Normal.xyz;
	vTangent = Tangent.xyz;
	vBiTangent = cross(vNormal, vTangent);
	vTexCoord = TexCoord;

	ivec4 index = ivec4(BoneIndices);

	vec4 P = bones[index.x] * Position * BoneWeight.x;
		P += bones[index.y] * Position * BoneWeight.y;
		P += bones[index.z] * Position * BoneWeight.z;
		P += bones[index.w] * Position * BoneWeight.w;

	gl_Position = ProjectionView * P;
}
