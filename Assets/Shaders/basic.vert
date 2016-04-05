#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUv;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec4 vertexColor;

out vec2 uv;

uniform mat4 mxModel;
uniform mat4 mxView;
uniform mat4 mxProjection;

void main() {
    uv = vertexUv;
    gl_Position = mxProjection * mxView * mxModel * vec4(vertexPosition, 1.0);
}