#version 430 core
out vec4 FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
    vec2 TexCoords;
} fsInData;

layout(location = 0) uniform sampler2D oceanDepth;
layout(location = 1) uniform sampler2D flowMap;

layout(location = 2) uniform float waveHeight;
layout(location = 3) uniform float choppiness;

void main()
{
	vec2 texCoordsFixed = vec2(fsInData.TexCoords.x, 1.0-fsInData.TexCoords.y);
	vec3 depth = vec3(0, 0, texture2D(oceanDepth, texCoordsFixed).b);
	vec3 flow = vec3(texture2D(flowMap, texCoordsFixed).rg, 0);

	FragColor = vec4(depth + flow, 1);
}