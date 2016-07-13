#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;

layout (location = 0) uniform float time;
layout (location = 1) uniform float waveHeight;

out VertexData {
    vec3 FragPos;
    vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
    vec2 TexCoords;
	float WaveHeight01;
	float Time;
} vsData;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

float waveFunc1(float offsetVal)
{
	float sinVal = sin((offsetVal*5) + (time*0.9));
	// make the wave steeper by applying pow to a [0,1] value
	float powVal = pow((sinVal + 1)/2, 2)*2 - 1;
	return powVal * waveHeight * 0.25;
}

float waveFunc2(float offsetVal)
{
	float sinVal = sin((offsetVal*2) + (time*1.4));
	// make the wave steeper by applying pow to a [0,1] value
	float powVal = pow((sinVal + 1)/2, 2)*2 - 1;
	return powVal * waveHeight * 0.25;
}

void main()
{
    vsData.TexCoords = texCoords;
	
	float wave1 = waveFunc1(position.z);
	float wave2 = waveFunc2(position.x + position.z);
	float waveFinal = wave1+wave2;
	vec3 waveAlteredPos = position + vec3(0, waveFinal, 0);
    vsData.FragPos = (model * vec4(waveAlteredPos,  1)).xyz;
	gl_Position = projection * view * model * vec4(waveAlteredPos, 1);

	vsData.WaveHeight01 = (waveFinal + waveHeight/2)/waveHeight;

	vec3 diffx = position + vec3(0.01,0,0);
	float diffxw1 = waveFunc1(diffx.z);
	float diffxw2 = waveFunc2(diffx.x + diffx.z);
	diffx = diffx + vec3(0, diffxw1+diffxw2, 0);

	vec3 diffz = position + vec3(0,0,0.01);
	float diffzw1 = waveFunc1(diffz.z);
	float diffzw2 = waveFunc2(diffz.x + diffz.z);
	diffz = diffz + vec3(0, diffzw1+diffzw2, 0);

	vec3 modifiedNorm = normalize(cross(diffz-waveAlteredPos, diffx-waveAlteredPos));
	vsData.Normal    = (model * vec4(modifiedNorm, 1)).xyz;
	vsData.Tangent   = (model * vec4(normalize(cross(modifiedNorm, bitangent)), 1)).xyz;
	vsData.Bitangent = (model * vec4(normalize(cross(modifiedNorm, tangent)), 1)).xyz;
	vsData.Time = time;
}