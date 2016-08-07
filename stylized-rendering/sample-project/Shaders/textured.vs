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
	vec3 ShadowCoords; //stores texture coords in xy, depth in z
} vsData;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 depthBiasMVP;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1);

    vsData.FragPos = vec3(model * vec4(position, 1));
    vsData.Normal = (model * vec4(normal, 1)).xyz;
	vsData.Tangent = (model * vec4(tangent, 1)).xyz;
	vsData.Bitangent = (model * vec4(bitangent, 1)).xyz;
    vsData.TexCoords = texCoords;
	vsData.ShadowCoords = vec3(depthBiasMVP * vec4(position, 1));
}