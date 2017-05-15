#version 430 core

in vec2 UV;

out vec3 color;

uniform sampler2DArray textureSampler;
uniform int textureLayer;

void main() {
    color = texture(textureSampler, vec3(UV, textureLayer)).rgb;
}

