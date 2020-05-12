#version 330 core

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

out vec4 FragColor;

void main() {
    FragColor = vec4(0.8549, 0.6470, 0.1254, 1);
    return;
}