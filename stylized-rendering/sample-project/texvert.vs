#version 330 core
uniform mat4x4 MVP;
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertUV;
out vec2 fragUV;

void main(){
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    fragUV = vertUV;
}