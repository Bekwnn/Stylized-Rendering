#version 430 core

layout(location = 0) out float fragmentdepth;

in VSData {
	vec2 TexCoords;
} fsIn;

uniform sampler2D diffuse;

void main()
{
	float alpha = texture2D(diffuse, vec2(1) - fsIn.TexCoords).a;
	if (alpha < 0.2) { discard; } //discard transparent pixels
	
	fragmentdepth = gl_FragCoord.z;
}