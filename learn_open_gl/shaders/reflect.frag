#version 330 core
in vec3 Normal;
in vec3 WorldPos;

struct Material {
    samplerCube cube1;
};

uniform Material material;

uniform vec3 camera_pos;

out vec4 FragColor;

void main(){

    vec3 incident = normalize(WorldPos - camera_pos);
    vec3 reflected = reflect(incident, normalize(Normal));
    FragColor = vec4(texture(material.cube1, reflected).rgb, 1.0);
}
