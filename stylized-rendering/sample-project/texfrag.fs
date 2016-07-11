#version 430 core
out vec4 FragColor;

in VertexData {
    vec3 FragPos;
    vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
    vec2 TexCoords;
} fsInData;


layout(location = 0) uniform sampler2D diffuseMap;
layout(location = 1) uniform sampler2D normalMap;  

layout(location = 2) uniform vec3 lightPos;
layout(location = 3) uniform vec3 viewPos;
layout(location = 4) uniform float uvMultiplier;
layout(location = 5) uniform float specPower;
layout(location = 6) uniform float specMultiplier;
layout(location = 7) uniform bool useNormalMapping;

void main()
{
	// calculate normal from tangent frame and normal map
	vec3 normal = normalize(fsInData.Normal);
    if(useNormalMapping)
	{
        vec3 mapNormal = texture2D(normalMap, fsInData.TexCoords * uvMultiplier).rgb;
		mapNormal = normalize(mapNormal);
        mat3 TBN = mat3(
			normalize(fsInData.Tangent),
			normalize(fsInData.Bitangent),
			normalize(fsInData.Normal)
			);
		vec3 newNormal = TBN * mapNormal;
		normal = normalize(newNormal);
    }
	
    vec3 color = texture2D(diffuseMap, fsInData.TexCoords * uvMultiplier).rgb;

	// Ambient
    vec3 ambient = 0.1 * color;

    // Diffuse
    vec3 lightDir = normalize(lightPos - fsInData.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
	vec3 viewDir = normalize(viewPos - fsInData.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(normal, halfwayDir), 0.0), specPower);
	vec3 specular = vec3(specMultiplier) * spec;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
	
}