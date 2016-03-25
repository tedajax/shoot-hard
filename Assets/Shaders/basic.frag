#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D mainTexture;

void main() {
    color = vec4(uv.x, uv.y, 1.0, 1.0);
}