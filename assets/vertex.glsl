#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec2 UV;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
	UV = vertexPosition_modelspace.xy;
}
