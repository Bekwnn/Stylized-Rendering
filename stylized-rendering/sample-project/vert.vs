#version 330 core
uniform mat4x4 MVP;
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;

void main(){
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
	//gl_Position = vec4(vertexPosition_modelspace,1);
    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragmentColor = vertexColor;
}