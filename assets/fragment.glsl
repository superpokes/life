#version 450 core

in vec2 UV;

out vec3 color;

//uniform sampler2DArray textureSampler;
uniform sampler2D textureSampler;
uniform float textureLayer;

void main() {
//    color = texture(textureSampler, vec3(UV, textureLayer)).rgb;
    color = texture(textureSampler, UV).rgb;
}

