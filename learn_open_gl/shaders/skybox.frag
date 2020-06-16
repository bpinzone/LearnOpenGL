#version 330 core
in vec3 TexCoords; // vec3 instead of vec2

out vec4 FragColor;

struct Material {
    samplerCube cube1;
};
uniform Material material;

void main() {
    FragColor = texture(material.cube1, TexCoords);
}