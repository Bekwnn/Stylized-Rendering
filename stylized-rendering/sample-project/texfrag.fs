#version 330 core
uniform sampler2D tex;
uniform float uvMultiplier;
in vec2 fragUV;

out vec3 color;

void main() {
    color = texture2D(tex, fragUV * uvMultiplier).rgb;
	//color = vec3(fragUV.x, fragUV.y, 0); //test UV
}