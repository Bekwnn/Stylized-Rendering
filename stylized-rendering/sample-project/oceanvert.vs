#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoords;

layout (location = 0) uniform sampler2D oceanDepth;
layout (location = 1) uniform float time;
layout (location = 2) uniform float waveHeight;

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

float hash( vec2 p ) {
	float h = dot(p,vec2(127.1,311.7));	
    return fract(sin(h)*43758.5453123);
}

float noise(vec2 p) {
    vec2 i = floor( p );
    vec2 f = fract( p );	
	vec2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*mix( mix( hash( i + vec2(0.0,0.0) ), 
                     hash( i + vec2(1.0,0.0) ), u.x),
                mix( hash( i + vec2(0.0,1.0) ), 
                     hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

float waveFunc(vec2 uv, float choppy)
{
	uv += noise(uv);
	vec2 wv = 1.0 - abs(sin(uv));
	vec2 swv = abs(cos(uv));
	wv = mix(wv, swv, wv);
	return pow(1.0 - pow(wv.x * wv.y, 0.65), choppy);
}

float loopWaveFunc(vec2 uv)
{
	float amp = waveHeight;
	float choppy = 4.0;
	float freq = 0.4;
	float d, waveFinal = 0.0;
	for (int i = 0; i < 3; i++)
	{
		d = waveFunc((position.xz+time)*freq, choppy);
		d += waveFunc((position.xz-time)*freq, choppy);
		waveFinal += d * amp;
		amp *= 0.4;
		freq *= 1.9;
		uv *= mat2(1.6,1.2,-1.2,1.6);
		choppy = mix(choppy, 1.0, 0.2);
	}
	return waveFinal;
}

void main()
{
    vsData.TexCoords = texCoords;
	
	float waveFinal = loopWaveFunc(position.xz);
	vec3 waveAlteredPos = position + vec3(0, waveFinal, 0);
    vsData.FragPos = (model * vec4(waveAlteredPos,  1)).xyz;
	gl_Position = projection * view * model * vec4(waveAlteredPos, 1);

	vsData.WaveHeight01 = (waveFinal + waveHeight/2)/waveHeight;

	vec3 diffx = position + vec3(0.2,0,0);
	float diffxw = loopWaveFunc(diffx.xz);
	diffx = diffx + vec3(0, diffxw, 0);

	vec3 diffz = position + vec3(0,0,0.2);
	float diffzw = loopWaveFunc(diffz.xz);
	diffz = diffz + vec3(0, diffzw, 0);

	vec3 modifiedNorm = normalize(cross(diffz-waveAlteredPos, diffx-waveAlteredPos));
	vsData.Normal    = normalize((model * vec4(modifiedNorm, 1)).xyz);
	vsData.Tangent   = normalize((model * vec4(normalize(cross(modifiedNorm, bitangent)), 1)).xyz);
	vsData.Bitangent = normalize((model * vec4(normalize(cross(tangent, modifiedNorm)), 1)).xyz);
	vsData.Time = time;
}