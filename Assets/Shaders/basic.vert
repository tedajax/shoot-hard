#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUv;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec4 vertexColor;

out vec2 uv;

uniform mat4 modelFrom;
uniform mat4 viewFrom;
uniform mat4 projectionFrom;

void main() {
    uv = vertexUv;
    gl_Position = projectionFrom * viewFrom * modelFrom * vec4(vertexPosition, 1.0);
}