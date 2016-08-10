#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out VSData {
	vec2 TexCoords;
} vsData;

layout(location = 0) uniform sampler2D stiffness;
uniform mat4 depthM;
uniform mat4 depthV;
uniform mat4 depthP;
uniform vec3 wind;
uniform float time;

void main()
{
	vec3 worldPos = vec3(depthM * vec4(position, 1.0));
	float windVal = sin(worldPos.x*2 + time)*0.5 + 0.5;
	windVal = windVal * texCoords.y * 0.1;
	vec3 newPos = worldPos + windVal * wind;
    gl_Position = depthP * depthV * vec4(newPos, 1);

	vsData.TexCoords = texCoords;
}