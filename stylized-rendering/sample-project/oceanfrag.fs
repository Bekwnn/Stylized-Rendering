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
layout(location = 3) uniform sampler2D flowMap;
layout(location = 4) uniform sampler2D waveNormal;
layout(location = 5) uniform sampler2D perlin;

layout(location = 6) uniform vec3 lightPos;
layout(location = 7) uniform vec3 viewPos;
layout(location = 8) uniform vec3 waveCrestColor;
layout(location = 9) uniform vec3 waveValleyColor;

void main()
{
	//vec3 normal = normalize(fsInData.Normal); //uncomment to ignore normal map

	// calculate normal from tangent frame and normal map
    vec3 mapNormal = texture2D(waveNormal, fsInData.TexCoords * 4 + vec2(fsInData.Time*-0.1)).rgb;
	mapNormal = normalize(mapNormal);
    mat3 TBN = mat3(
		normalize(fsInData.Tangent),
		normalize(fsInData.Bitangent),
		normalize(fsInData.Normal)
		);
	vec3 newNormal = TBN * mapNormal;
	vec3 normal = normalize(newNormal);

	vec2 texCoordsFixed = vec2(fsInData.TexCoords.x, 1.0-fsInData.TexCoords.y);
	float depth = texture2D(oceanDepth, texCoordsFixed).b;
	vec3 flow = vec3(texture2D(flowMap, texCoordsFixed).rg, 0);

	vec3 viewDir = normalize(viewPos - fsInData.FragPos);
	float mixVal = fsInData.WaveHeight01 * pow((1 - max(dot(viewDir, normal), 0.0)), 4);
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
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 70);
	vec3 specular = vec3(0.5) * spec;

	FragColor = vec4(ambient + diffuse + specular, depth);
}