#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D diffuseMap;

void main() {
    vec4 diffuse = texture(diffuseMap, uv);
    color = diffuse;
}