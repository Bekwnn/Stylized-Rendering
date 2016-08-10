#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;

out VertexData {
    vec3 FragPos;
    vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
    vec2 TexCoords;
	vec3 ShadowCoords;
} vsData;

uniform sampler2D stiffness;
uniform float time;
uniform vec3 wind;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vec3 worldPos = vec3(model * vec4(position, 1.0));
	float windVal = sin(worldPos.x*2 + time)*0.5 + 0.5;
	windVal = windVal * texCoords.y * 0.1;
	vec3 newPos = worldPos + (wind*windVal);
    gl_Position = projection * view * vec4(newPos, 1);

    vsData.FragPos = newPos;
    vsData.Normal = (model * vec4(normal, 1)).xyz;
	vsData.Tangent = (model * vec4(tangent, 1)).xyz;
	vsData.Bitangent = (model * vec4(bitangent, 1)).xyz;
    vsData.TexCoords = texCoords;
}