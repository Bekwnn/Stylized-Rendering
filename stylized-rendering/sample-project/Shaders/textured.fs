#version 430 core
out vec4 FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
    vec2 TexCoords;
	vec3 ShadowCoords;
} fsInData;


layout(location = 0) uniform sampler2D diffuseMap;
layout(location = 1) uniform sampler2D normalMap;  
layout(location = 2) uniform sampler2D shadowMap;  

layout(location = 3) uniform vec3 lightPos;
layout(location = 4) uniform vec3 viewPos;
layout(location = 5) uniform float uvMultiplier;
layout(location = 6) uniform float specPower;
layout(location = 7) uniform float specMultiplier;
layout(location = 8) uniform bool useNormalMapping;
layout(location = 9) uniform bool useShadowMapping;

vec2 poissonDisk[11] = vec2[](
	vec2(0.5827442f, 0.0891116f),
	vec2(0.2502124f, 0.4970801f),
	vec2(0.00655831f, -0.2463339f),
	vec2(0.5999211f, -0.7016115f),
	vec2(-0.2900631f, 0.5406905f),
	vec2(-0.5973006f, -0.4397166f),
	vec2(-0.2528148f, -0.8084832f),
	vec2(-0.9663795f, -0.03635002f),
	vec2(0.7273946f, 0.6772284f),
	vec2(0.1174015f, 0.9925542f),
	vec2(-0.8065577f, 0.4807332f)
);

void main()
{
	// calculate normal from tangent frame and normal map
	vec3 normal = normalize(fsInData.Normal);
    if(useNormalMapping)
	{
        vec3 mapNormal = texture2D(normalMap, fsInData.TexCoords * uvMultiplier).rgb;
		mapNormal = (2*mapNormal)-1;
		mapNormal = normalize(mapNormal);
        mat3 TBN = mat3(
			normalize(fsInData.Tangent),
			normalize(fsInData.Bitangent),
			normalize(fsInData.Normal)
			);
		vec3 newNormal = TBN * mapNormal;
		normal = normalize(newNormal);
    }
	
    vec3 lightDir = normalize(lightPos);
	float shadowMult = 1.0;
	if (useShadowMapping)
	{
		for (int i = 0; i < 11; i++)
		{
			float texZ = texture2D(shadowMap, fsInData.ShadowCoords.xy + poissonDisk[i] * 0.002).z;
			if (texZ  <  fsInData.ShadowCoords.z - 0.001)
			{
				shadowMult -= 0.05;
			}
		}
	}
	
    vec4 texCol = texture2D(diffuseMap, fsInData.TexCoords * uvMultiplier);
	vec3 color = texCol.rgb;
	float alpha = texCol.a;

	// Ambient
    vec3 ambient = 0.1 * color;

    // Diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
	vec3 viewDir = normalize(viewPos - fsInData.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specPower);
	vec3 specular = vec3(specMultiplier) * spec;
    
    FragColor = vec4(ambient + (diffuse + specular) * shadowMult, alpha);
}