#version 430 core
out vec4 FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
    vec2 TexCoords;
	float WaveHeight01;
	float Time;
} fsInData;

layout(location = 2) uniform sampler2D oceanDepth;
layout(location = 3)  uniform sampler2D flowMap;
layout(location = 4)  uniform sampler2D waveNormal;
layout(location = 5)  uniform sampler2D waveNormalAlt;
layout(location = 6)  uniform sampler2D perlin;

layout(location = 7)  uniform vec3 lightPos;
layout(location = 8)  uniform vec3 viewPos;
layout(location = 9)  uniform vec3 waveCrestColor;
layout(location = 10) uniform vec3 waveValleyColor;
layout(location = 11) uniform vec3 skyColor;

void main()
{
	vec3 macroNormal = normalize(fsInData.Normal); //smooth large wave normal used for macro details

	// mix normal maps with respect to time and perlin noise
    vec3 mapNormala = texture2D(waveNormal, fsInData.TexCoords * 8 + vec2(fsInData.Time*-0.1)).rgb;
	mapNormala = normalize(mapNormala*2 - 1);
	vec3 mapNormal2a = texture2D(waveNormalAlt, fsInData.TexCoords * 8 + vec2(fsInData.Time*0.1)).rgb;
	mapNormal2a = normalize(mapNormal2a*2 - 1);
	vec3 mapNormalb = texture2D(waveNormal, fsInData.TexCoords * 8 + vec2(fsInData.Time*-0.1, fsInData.Time*0.1)).rgb;
	mapNormalb = normalize(mapNormalb*2 - 1);
	vec3 mapNormal2b = texture2D(waveNormalAlt, fsInData.TexCoords * 8 + vec2(fsInData.Time*0.1, fsInData.Time*-0.1)).rgb;
	mapNormal2b = normalize(mapNormal2b*2 - 1);
	float perlinVal = texture2D(perlin, fsInData.TexCoords).g;
	mapNormala = mix(normalize(mapNormala), normalize(mapNormal2a), perlinVal);
	mapNormalb = mix(normalize(mapNormalb), normalize(mapNormal2b), 1.0-perlinVal);
	vec3 mapNormal = mix(mapNormala, mapNormalb, 0.5);

	//transform mapNormal to world space with TBN
    mat3 TBN = mat3(
		fsInData.Tangent,
		fsInData.Bitangent,
		fsInData.Normal
		);
	vec3 newNormal = TBN * mapNormal;
	vec3 normal = normalize(newNormal);

	//store depth and flow map values
	vec2 texCoordsFixed = vec2(fsInData.TexCoords.x, 1.0-fsInData.TexCoords.y);
	float depth = texture2D(oceanDepth, texCoordsFixed).r;
	depth = clamp(depth + fsInData.WaveHeight01 * 0.1, 0.0, 1.0);
	float nearShoreBlend = clamp((depth-0.95)*20, 0.0, 1.0);
	normal = mix(normal, macroNormal, nearShoreBlend*0.5);
	//vec2 flow = texture2D(flowMap, texCoordsFixed).rg;

	//wave highlight
	vec3 viewDir = normalize(viewPos - fsInData.FragPos);
	float mixVal = fsInData.WaveHeight01 * pow((1 - abs(dot(viewDir, macroNormal))), 8);
	vec3 color = mix(waveValleyColor, waveCrestColor, mixVal);

	//ambient
	vec3 ambient = 0.1 * color;

	//diffuse
	vec3 lightDir = normalize(lightPos - fsInData.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 100);
	vec3 specular = vec3(0.5) * spec;

	// Fresnel
	float fresnel = 1.0 - abs(dot(viewDir, normal));
	fresnel = pow(fresnel, 3) * 0.65;
	vec3 seaColor = mix(ambient + diffuse, skyColor, 0.2); // base level mixing of sky and sea color
	vec3 finalColor = mix(seaColor, skyColor, fresnel); //fresnel increases sky color

	// Shore transparency
	float alpha = 1 - pow((depth * abs(dot(viewDir, macroNormal))), 4);
	//finalColor = mix(finalColor, vec3(0.9,0.9,0.9), nearShoreBlend*0.5);
	
	//FragColor = vec4(vec3(dot(viewDir, normal)),1);
	FragColor = vec4(finalColor + specular, 0.2 + alpha);
}