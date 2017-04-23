#version 330 core

in vec2 UV;

out vec3 color;

// Is it uniform? Look into texture array extension? IDK
uniform sampler2D textureSampler;

void main() {
    color = texture(textureSampler, UV).rgb;
}
