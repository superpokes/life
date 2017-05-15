#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec2 UV;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
	UV.x = vertexPosition_modelspace.x;
    if (vertexPosition_modelspace.y == 1.0) {
        UV.y = 0.0;
    } else {
        UV.y = 1.0;
    }
}
