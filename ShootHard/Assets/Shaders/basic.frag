#version 330 core
in vec3 p;
out vec3 color;
void main() {
    color = vec3(p.x, p.y, p.z);
}