#version 330 core
in vec3 p;
out vec3 color;
void main() {
    vec3 p2 = (p + vec3(1, 1, 1)) / 2;
    color = p2;
}