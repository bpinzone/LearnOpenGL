#version 330 core

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

uniform vec3 camera_pos;

struct Material {
    // colors reflected for each light component
    sampler2D diffuse1;  // serves as diffuse AND ambient color.
    sampler2D specular1;
    float shininess;
};
uniform Material material;

struct Dir_light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};
uniform Dir_light dir_light;

out vec4 FragColor;

vec3 calc_dir_light(Dir_light dir_light, vec3 normal_n, vec3 frag_to_camera_n);

// REQUIRES: dir_light.direction, normal_n, and frag_to_camera_n are normalized.
vec3 calc_dir_light(Dir_light dir_light, vec3 normal_n, vec3 frag_to_camera_n) {

    float diff = max(dot(normal_n, -dir_light.direction), 0.0);

    vec3 light_ref = normalize(reflect(dir_light.direction, normal_n));
    float spec = pow(max(dot(light_ref, frag_to_camera_n), 0.0), material.shininess);

    vec3 ambient  = dir_light.ambient  *        vec3(texture(material.diffuse1, TexCoords));
    vec3 diffuse  = dir_light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
    vec3 specular = dir_light.specular * spec * vec3(texture(material.specular1, TexCoords));
    return ambient + diffuse + specular;
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 frag_to_camera_n = normalize(camera_pos - WorldPos);

    vec3 result = calc_dir_light(dir_light, norm, frag_to_camera_n);
    FragColor = vec4(result, 1.0);
    return;
}