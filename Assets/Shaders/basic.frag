#version 330 core

in vec2 uv;

out vec4 color;

uniform vec4 ambientLightColor;
uniform vec4 lightColor;
uniform vec3 lightDirection;
uniform float lightPower;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap

void main() {
    normal = texture(normalMap, uv);
    color = texture(diffuseMap, uv);
}