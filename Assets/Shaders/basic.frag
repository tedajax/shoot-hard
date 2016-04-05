#version 330 core

in vec2 uv;

out vec4 color;

uniform vec4 ambientLightColor;
uniform vec4 lightColor;
uniform vec3 lightDirection;
uniform float lightPower;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main() {
    vec3 n = normalize(texture(normalMap, uv).xyz);
    vec3 d = lightDirection;
    vec3 l = normalize(lightDirection);

    float cosTheta = clamp(dot(n, l), 0, 1);

    vec4 diffuse = texture(diffuseMap, uv);
    color = vec4(lightColor.rgb * cosTheta, 1);
}