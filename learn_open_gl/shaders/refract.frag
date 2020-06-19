#version 330 core
in vec3 Normal;
in vec3 WorldPos;

struct Material {
    samplerCube cube1;
};

uniform Material material;

uniform vec3 camera_pos;

out vec4 FragColor;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 incident = normalize(WorldPos - camera_pos);
    vec3 refracted = refract(incident, normalize(Normal), ratio);
    FragColor = vec4(texture(material.cube1, refracted).rgb, 1.0);
}
